/* paf -- version of the paf generator for en echo, which reproduces a
bug in the old 4X version -- search for "4XBUG" */

/*  Copyright 1997-2020 Miller Puckette.
Permission is granted to use this software for any purpose provided you
keep this copyright notice intact.

THE AUTHOR AND HIS EMPLOYERS MAKE NO WARRANTY, EXPRESS OR IMPLIED,
IN CONNECTION WITH THIS SOFTWARE.

This file is downloadable from msp.ucsd.edu .

*/

// port to maxmsp by vboehm
// march 2026


static char *paf_version = "paf version 0.07";

#include <stdlib.h>
#include <math.h>

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"

#define LOGTABSIZE 9
#define TABSIZE (1 << LOGTABSIZE)
#define TABRANGE 3

//#include <machine/endian.h>

typedef struct _tabpoint
{
    double p_y;
    double p_diff;
} t_tabpoint;

static t_tabpoint paf_gauss[TABSIZE];
static t_tabpoint paf_cauchy[TABSIZE];

typedef struct _linenv
{
    double l_current;
    double l_biginc;
    double l_1overn;
    double l_target;
    double l_msectodsptick;
    int l_ticks;
} t_linenv;

typedef struct _pafctl
{
    t_linenv x_freqenv;
    t_linenv x_cfenv;
    t_linenv x_bwenv;
    t_linenv x_ampenv;
    t_linenv x_vibenv;
    t_linenv x_vfrenv;
    t_linenv x_shiftenv;
    double x_isr;
    double x_held_freq;
    double x_held_intcar;
    double x_held_fraccar;
    double x_held_bwquotient;
    double x_phase;
    double x_shiftphase;
    double x_vibphase;
    int x_triggerme;
    int x_cauchy;
    int x_4xcompat;
} t_pafctl;

static void linenv_debug(t_linenv *l, char *s)
{
#ifdef DEBUG
    post("%s: current %f, target %f", s, l->l_current, l->l_target);
#endif
}

static void linenv_init(t_linenv *l)
{
    l->l_current = l->l_biginc = 0;
    l->l_1overn = l->l_target = l->l_msectodsptick = 0;
    l->l_ticks = 0;
}

static void linenv_setsr(t_linenv *l, double sr, int vecsize)
{
    l->l_msectodsptick = sr / (1000.0 * ((double)vecsize));
    l->l_1overn = 1./(double)vecsize;
}

static void linenv_set(t_linenv *l, double target, long timdel)
{
    if (timdel > 0)
    {
	l->l_ticks = ((double)timdel) * l->l_msectodsptick;
	if (!l->l_ticks) l->l_ticks = 1;
	l->l_target = target;
	l->l_biginc = (l->l_target - l->l_current)/l->l_ticks;
    }
    else
    {
	l->l_ticks = 0;
	l->l_current = l->l_target = target;
	l->l_biginc = 0;
    }
}

#define LINENV_RUN(linenv, current, incr) 		\
    if (linenv.l_ticks > 0)				\
    {							\
    	current = linenv.l_current;			\
    	incr = linenv.l_biginc * linenv.l_1overn;	\
	linenv.l_ticks--;				\
	linenv.l_current += linenv.l_biginc;		\
    }							\
    else						\
    {							\
    	linenv.l_current = current = linenv.l_target;	\
	incr = 0;					\
    }

#define UNITBIT32 1572864.		/* 3*2^19 -- bit 32 has value 1 */
#define TABFRACSHIFT (UNITBIT32/TABSIZE) 


# define HIOFFSET 1                                                              
# define LOWOFFSET 0                                                             
//
//# define HIOFFSET 0    /* word offset to find MSB */                             
//# define LOWOFFSET 1    /* word offset to find LSB */                            


union tabfudge
{
    double tf_d;
    int32_t tf_i[2];
};

#define A1 ((double)(4 * (3.14159265/2)))
#define A3 ((double)(64 * (2.5 - 3.14159265)))
#define A5 ((double)(1024 * ((3.14159265/2) - 1.5)))

static void pafctl_run(t_pafctl *x, double *out1, int n)
{
    double freqval, freqinc;
    double cfval, cfinc;
    double bwval, bwinc;
    double ampval, ampinc;
    double vibval, vibinc;
    double vfrval, vfrinc;
    double shiftval, shiftinc;
    double bwquotient, bwqincr;
    double ub32 = UNITBIT32;
    double phase = x->x_phase + ub32;
    double shiftphase = x->x_shiftphase + ub32;
    double held_freq = x->x_held_freq;
    double held_intcar = x->x_held_intcar;
    double held_fraccar = x->x_held_fraccar;
    double held_bwquotient = x->x_held_bwquotient;
    double sinvib, vibphase;
    union tabfudge tf;
    int32_t hackval, lowbits;

    t_tabpoint *paf_table = (x->x_cauchy ? paf_cauchy : paf_gauss);
    tf.tf_d = ub32;
    hackval = tf.tf_i[HIOFFSET];

    	/* fractional part of shift phase */
    tf.tf_d = shiftphase;
    tf.tf_i[HIOFFSET] = hackval;
    shiftphase = tf.tf_d;

	/* propagate line envelopes */
    LINENV_RUN(x->x_freqenv, freqval, freqinc);
    LINENV_RUN(x->x_cfenv, cfval, cfinc);
    LINENV_RUN(x->x_bwenv, bwval, bwinc);
    LINENV_RUN(x->x_ampenv, ampval, ampinc);
    LINENV_RUN(x->x_vibenv, vibval, vibinc);
    LINENV_RUN(x->x_vfrenv, vfrval, vfrinc);
    LINENV_RUN(x->x_shiftenv, shiftval, shiftinc);

	/* fake line envelope for quotient of bw and frequency */
    bwquotient = bwval/freqval;
    bwqincr = (((double)(x->x_bwenv.l_current))/
    	((double)(x->x_freqenv.l_current)) - bwquotient) *
		x->x_freqenv.l_1overn;

	/* run the vibrato oscillator */
    
    tf.tf_d = ub32 + (x->x_vibphase + n * x->x_isr * vfrval);
    tf.tf_i[HIOFFSET] = hackval;
    vibphase = (x->x_vibphase = tf.tf_d - ub32);
    if (vibphase > 0.5)
    	sinvib = 1.0 - 16.0 * (0.75-vibphase) * (0.75 - vibphase);
    else sinvib = -1.0 + 16.0 * (0.25-vibphase) * (0.25 - vibphase);
    freqval = freqval * (1.0 + vibval * sinvib);

    shiftval *= x->x_isr;
    shiftinc *= x->x_isr;
    
	/* if phase or amplitude is zero, load in new params */
    if (ampval == 0 || phase == ub32 || x->x_triggerme)
    {
    	    /* 4XBUG -- this should be just cfval/freqval: */
    	double cf_over_freq =
	    (x->x_4xcompat ? 2 * cfval/freqval - 1 : cfval/freqval);
    	held_freq = freqval * x->x_isr;
         
    }
    while (n--)
    {
    	double newphase = phase + held_freq;
        double carphase1, carphase2, fracnewphase;
        double fphase, fcarphase1, fcarphase2, carrier;
        double g, g2, g3, cosine1, cosine2, halfsine, mod, tabfrac;
        t_tabpoint *p;
   	    /* put new phase into 64-bit memory location.  Bash upper
	    32 bits to get fractional part (plus "ub32").  */

        tf.tf_d = newphase;
        tf.tf_i[HIOFFSET] = hackval;
        newphase = tf.tf_d;
        fracnewphase = newphase-ub32;
        fphase = 2.0f * ((double)(fracnewphase)) - 1.0;
        if (newphase < phase)
        {
            double cf_over_freq =    	/* 4XBUG */
                (x->x_4xcompat ? 2 * cfval/freqval - 1 : cfval/freqval);
            held_freq = freqval * x->x_isr;
            held_intcar = (double)((int)cf_over_freq);
            held_fraccar = cf_over_freq - held_intcar;
            held_bwquotient = bwquotient;
        }
        phase = newphase;
        tf.tf_d = fracnewphase * held_intcar + shiftphase;
        tf.tf_i[HIOFFSET] = hackval;
        carphase1 = tf.tf_d;
        fcarphase1 = carphase1 - ub32;
        tf.tf_d = carphase1 + fracnewphase;
        tf.tf_i[HIOFFSET] = hackval;
        carphase2 = tf.tf_d;
        fcarphase2 = carphase2 - ub32;
            
        shiftphase += shiftval;
        
        if (fcarphase1 > 0.5)  g = fcarphase1 - 0.75;
        else g = 0.25 - fcarphase1;
        g2 = g * g;
        g3 = g * g2;
        cosine1 = g * A1 + g3 * A3 + g2 * g3 * A5;

        if (fcarphase2 > 0.5)  g = fcarphase2 - 0.75;
        else g = 0.25 - fcarphase2;
        g2 = g * g;
        g3 = g * g2;
        cosine2 = g * A1 + g3 * A3 + g2 * g3 * A5;
        
        carrier = cosine1 + held_fraccar * (cosine2-cosine1);

        ampval += ampinc;
        bwquotient += bwqincr;

        /* printf("bwquotient %f\n", bwquotient); */

        halfsine = held_bwquotient * (1.0 - fphase * fphase);
        if (halfsine >= (double)(0.997 * TABRANGE))
            halfsine = (double)(0.997 * TABRANGE);

    #if 0
        shape = halfsine * halfsine;
        mod = ampval * carrier *
            (1 - bluntval * shape) / (1 + (1 - bluntval) * shape);
    #endif
    #if 0
        shape = halfsine * halfsine;
        mod = ampval * carrier *
            exp(-shape);
    #endif
        halfsine *= (double)(1./TABRANGE);

            /* Get table index for "halfsine".  Bash upper
            32 bits to get fractional part (plus "ub32").  Also grab
            fractional part as a fixed-point number to use as table
            address later. */

        tf.tf_d = halfsine + ub32;
        lowbits = tf.tf_i[LOWOFFSET];

                /* now shift again so that the fractional table address
            appears in the low 32 bits, bash again, and extract this as
            a doubleing point number from 0 to 1. */
        tf.tf_d = halfsine + TABFRACSHIFT;
        tf.tf_i[HIOFFSET] = hackval;
        tabfrac = tf.tf_d - ub32;

        p = paf_table + ((lowbits >> (32 - LOGTABSIZE)) & (TABSIZE-1));
        mod = ampval * carrier * (p->p_y + tabfrac * p->p_diff);
        
        *out1++ = mod;
    }
    x->x_phase = phase - ub32;
    x->x_shiftphase = shiftphase - ub32;
    x->x_held_freq = held_freq;
    x->x_held_intcar = held_intcar;
    x->x_held_fraccar = held_fraccar;
    x->x_held_bwquotient = held_bwquotient;
}

static void pafctl_init(t_pafctl *x)
{
    linenv_init(&x->x_freqenv);
    linenv_init(&x->x_cfenv);
    linenv_init(&x->x_bwenv);
    linenv_init(&x->x_ampenv);
    linenv_init(&x->x_vibenv);
    linenv_init(&x->x_vfrenv);
    linenv_init(&x->x_shiftenv);
    x->x_freqenv.l_target = x->x_freqenv.l_current = 1.0;
    x->x_isr = (1./44100.);
    x->x_held_freq = 1.;
    x->x_held_intcar = 0.;
    x->x_held_fraccar = 0.;
    x->x_held_bwquotient = 0.;
    x->x_phase = 0.;
    x->x_shiftphase = 0.;
    x->x_vibphase = 0.;
    x->x_triggerme = 0;
    x->x_cauchy = 0;
    x->x_4xcompat = 0;
}

static void pafctl_setsr(t_pafctl *x, double sr, int vecsize)
{
    x->x_isr = 1./sr;
    linenv_setsr(&x->x_freqenv, sr, vecsize);
    linenv_setsr(&x->x_cfenv, sr, vecsize);
    linenv_setsr(&x->x_bwenv, sr, vecsize);
    linenv_setsr(&x->x_ampenv, sr, vecsize);
    linenv_setsr(&x->x_vibenv, sr, vecsize);
    linenv_setsr(&x->x_vfrenv, sr, vecsize);
    linenv_setsr(&x->x_shiftenv, sr, vecsize);
}

static void pafctl_freq(t_pafctl *x, double val, int time)
{
    if (val < 1.) val = 1.;
    if (val > 10000000.) val = 1000000.;
    linenv_set(&x->x_freqenv, val, time);
}

static void pafctl_cf(t_pafctl *x, double val, int time)
{
    linenv_set(&x->x_cfenv, val, time);
}

static void pafctl_bw(t_pafctl *x, double val, int time)
{
    linenv_set(&x->x_bwenv, val, time);
}

static void pafctl_amp(t_pafctl *x, double val, int time)
{
    linenv_set(&x->x_ampenv, val, time);
}

static void pafctl_vib(t_pafctl *x, double val, int time)
{
    linenv_set(&x->x_vibenv, val, time);
}

static void pafctl_vfr(t_pafctl *x, double val, int time)
{
    linenv_set(&x->x_vfrenv, val, time);
}

static void pafctl_shift(t_pafctl *x, double val, int time)
{
    linenv_set(&x->x_shiftenv, val, time);
}

static void pafctl_phase(t_pafctl *x, double mainphase, double shiftphase,
    double vibphase)
{
    x->x_phase = mainphase;
    x->x_shiftphase = shiftphase;
    x->x_vibphase = vibphase;
    x->x_triggerme = 1;
}

    /* value of Cauchy distribution at TABRANGE */
#define CAUCHYVAL (1./ (1. + TABRANGE * TABRANGE))
    /* first derivative of Cauchy distribution at TABRANGE */
#define CAUCHYSLOPE ((-2. * TABRANGE) * CAUCHYVAL * CAUCHYVAL)
#define ADDSQ (- CAUCHYSLOPE / (2 * TABRANGE))

static void paf_dosetup(void)
{
    int i;
    double CAUCHYFAKEAT3  =
    	(CAUCHYVAL + ADDSQ * TABRANGE * TABRANGE);
    double CAUCHYRESIZE = (1./ (1. - CAUCHYFAKEAT3));
    for (i = 0; i <= TABSIZE; i++)
    {
    	double f = i * ((double)TABRANGE/(double)TABSIZE);
        double gauss = exp(-f * f);
        double cauchygenuine = 1. / (1. + f * f);
        double cauchyfake = cauchygenuine + ADDSQ * f * f;
        double cauchyrenorm = (cauchyfake - 1.) * CAUCHYRESIZE + 1.;
        if (i != TABSIZE)
        {
            paf_gauss[i].p_y = gauss;
            paf_cauchy[i].p_y = cauchyrenorm;
            /* post("%f", cauchyrenorm); */
        }
        if (i != 0)
        {
            paf_gauss[i-1].p_diff = gauss - paf_gauss[i-1].p_y;
            paf_cauchy[i-1].p_diff = cauchyrenorm - paf_cauchy[i-1].p_y;
        }
    }
}

#ifdef TESTME

#define BS 64
main()
{
    t_pafctl x;
    double x1[BS];
    int i;
    paf_dosetup();
    pafctl_init(&x);
    pafctl_setsr(&x, 16000., BS);
    pafctl_freq(&x, 1000, 0);
    pafctl_bw(&x, 2000, 0);
    pafctl_amp(&x, 1000, 0);
    pafctl_run(&x, x1, BS);
    for (i = 0; i < BS/4; i++)
    {
	printf("%15.5f %15.5f %15.5f %15.5f\n",
		x1[4*i], x1[4*i+1], x1[4*i+2], x1[4*i+3]);
    }
#if 0
    printf("\n");
    pafctl_bw(&x, 2000, 0);
    pafctl_run(&x, x1, BS);
    for (i = 0; i < BS/4; i++)
    {
	printf("%15.5f %15.5f %15.5f %15.5f\n",
		x1[4*i], x1[4*i+1], x1[4*i+2], x1[4*i+3]);
    }
#endif
}

#endif



typedef struct _paf
{
    t_pxobject  x_obj;
    t_pafctl    x_pafctl;
} t_paf;


static t_class *paf_class = NULL;

static void *paf_new(t_symbol *s, long argc, t_atom *argv)
{
    t_paf *x = (t_paf *)object_alloc(paf_class);
    dsp_setup((t_pxobject *)x, 0);
    pafctl_init(&x->x_pafctl);
    outlet_new((t_pxobject *)x, "signal");
    return (x);
}

void paf_perform64(t_paf *x, t_object *dsp64, double **ins,
                     long numins, double **outs, long numouts,
                     long sampleframes, long flags, void *userparam)
{
    t_double    *out1 = outs[0];
    int n = (int)sampleframes;
    
    
    pafctl_run(&x->x_pafctl, out1, n);
    
}

void paf_free(t_paf *x) {
    dsp_free((t_pxobject *)x);
}


void paf_dsp64(t_paf *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    pafctl_setsr(&x->x_pafctl, samplerate, (int)maxvectorsize);
    object_method(dsp64, gensym("dsp_add64"), x, paf_perform64, 0, NULL);
}


static void paf_freq(t_paf *x, double val, double time)
{
    pafctl_freq(&x->x_pafctl, val, time);
}

static void paf_cf(t_paf *x, double val, double time)
{
    pafctl_cf(&x->x_pafctl, val, time);
}

static void paf_bw(t_paf *x, double val, double time)
{
    pafctl_bw(&x->x_pafctl, val, time);
}

static void paf_amp(t_paf *x, double val, double time)
{
    pafctl_amp(&x->x_pafctl, val, time);
}

static void paf_vib(t_paf *x, double val, double time)
{
    pafctl_vib(&x->x_pafctl, val, time);
}

static void paf_vfr(t_paf *x, double val, double time)
{
    pafctl_vfr(&x->x_pafctl, val, time);
}

static void paf_shift(t_paf *x, double val, double time)
{
    pafctl_shift(&x->x_pafctl, val, time);
}

static void paf_phase(t_paf *x, double mainphase, double shiftphase,
    double vibphase)
{
    pafctl_phase(&x->x_pafctl, mainphase, shiftphase, vibphase);
}

static void paf_setcauchy(t_paf *x, long m)
{
    x->x_pafctl.x_cauchy = (m != 0);
}

static void paf_set4x(t_paf *x, double f)
{
        /* set compatibility with early buggy implementation, useful for
        early Manoury (Partition, Neptune, en Echo). */
    if ((x->x_pafctl.x_4xcompat = (f != 0)))
        x->x_pafctl.x_cauchy = 1;
}

static void paf_debug(t_paf *x)
{
    /* whatever you want... */
}


void ext_main(void *r)
{
    post(paf_version);
    paf_class = class_new("paf~", (method)paf_new, (method)paf_free, sizeof(t_paf), NULL, A_GIMME, 0);
   
    class_addmethod(paf_class, (method)paf_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(paf_class, (method)paf_freq, "freq", A_FLOAT, A_DEFFLOAT, 0);
    class_addmethod(paf_class, (method)paf_cf, "cf", A_DEFFLOAT, A_DEFFLOAT, 0);
    class_addmethod(paf_class, (method)paf_bw, "bw", A_FLOAT, A_DEFFLOAT, 0);
    class_addmethod(paf_class, (method)paf_amp, "amp", A_FLOAT, A_DEFFLOAT, 0);
    class_addmethod(paf_class, (method)paf_vib, "vib", A_FLOAT, A_DEFFLOAT, 0);
    class_addmethod(paf_class, (method)paf_vfr, "vfr", A_FLOAT, A_DEFFLOAT, 0);
    class_addmethod(paf_class, (method)paf_shift, "shift", A_FLOAT, A_DEFFLOAT, 0);
    class_addmethod(paf_class, (method)paf_phase, "phase", A_FLOAT, A_FLOAT, A_FLOAT, 0);
    class_addmethod(paf_class, (method)paf_setcauchy, "cauchy", A_LONG, 0);
    class_addmethod(paf_class, (method)paf_set4x, "4x", A_FLOAT, 0);
//    class_addmethod(paf_class, (method)paf_debug, "debug", 0);
    paf_dosetup();
    
    class_dspinit(paf_class);
    
    class_register(CLASS_BOX, paf_class);
    
    object_post(NULL, "paf~ by Miller Puckette");
}

