#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"
#include "ext_common.h"

/* -------------------------- freeverb~ --------------------------------------- */
/*                                                                              */
/* Tilde object that implements the Schroeder/Moorer reverb model.              */
/* Written by Olaf Matthes <olaf.matthes@gmx.de>.                               */
/*                                                                              */
/*                                                                              */
/* This program is free software; you can redistribute it and/or                */
/* modify it under the terms of the GNU General Public License                  */
/* as published by the Free Software Foundation; either version 2               */
/* of the License, or (at your option) any later version.                       */
/*                                                                              */
/* This program is distributed in the hope that it will be useful,              */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                */
/* GNU General Public License for more details.                                 */
/*                                                                              */
/* You should have received a copy of the GNU General Public License            */
/* along with this program; if not, write to the Free Software                  */
/* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.  */
/*                                                                              */
/* ---------------------------------------------------------------------------- */


/************* 64 bit version by volker boehm - http://vboehm.net ***************/
/********************************************************************************/



#define LOGTEN 2.302585092994

#define	numcombs		8
#define	numallpasses	4
#define	muted			0
#define	fixedgain		0.015
#define scalewet		3.0
#define scaledry		2.0
#define scaledamp		0.4
#define scaleroom		0.28
#define offsetroom		0.7
#define initialroom		0.5
#define initialdamp		0.5
#define initialwet		1.0/scalewet
#define initialdry		0.0
#define initialwidth	1.0
#define initialmode		0
#define initialbypass   0
#define freezemode		0.5
#define	stereospread	23

/* these values assume 44.1KHz sample rate
 they will probably be OK for 48KHz sample rate
 but would need scaling for 96KHz (or other) sample rates.
 the values were obtained by listening tests.                */
static const int combtuningL[numcombs] = { 1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617 };
static const int combtuningR[numcombs] = { 1116+stereospread, 1188+stereospread, 1277+stereospread, 1356+stereospread, 1422+stereospread, 1491+stereospread, 1557+stereospread, 1617+stereospread };

static const int allpasstuningL[numallpasses] = { 556, 441, 341, 225 };
//static const int allpasstuningR[numallpasses] = { 556+stereospread, 441+stereospread, 341+stereospread, 225+stereospread };

static char *version = "freeverb~ v1.2.2 -- 64 bit";


typedef struct {
	t_pxobject x_obj;
	double	sr;
    
    double	x_gain;
    double	x_roomsize,x_roomsize1;
    double	x_damp,x_damp1;
    double	x_wet,x_wet1,x_wet2;
    double	x_dry;
    double	x_width;
    short	x_mode;
    short   x_bypass;
    int     x_skip;
    
    double	x_allpassfeedback;			/* feedback of allpass filters */
    double	x_combfeedback;				/* feedback of comb filters */
    double x_combdamp1;
    double x_combdamp2;
    double x_filterstoreL[numcombs];	/* stores last sample value */
    double x_filterstoreR[numcombs];
    
    /* buffers for the combs */
    double	*x_bufcombL[numcombs];
    double	*x_bufcombR[numcombs];
    int x_combidxL[numcombs];
    int x_combidxR[numcombs];
    
    /* buffers for the allpasses */
    double	*x_bufallpassL[numallpasses];
    double	*x_bufallpassR[numallpasses];
    int x_allpassidxL[numallpasses];
    int x_allpassidxR[numallpasses];
    
    /* we'll make local copies adjusted to fit our sample rate */
    int x_combtuningL[numcombs];
    int x_combtuningR[numcombs];
    
    int x_allpasstuningL[numallpasses];
    int x_allpasstuningR[numallpasses];

} t_freeverb;


static t_class *freeverb_class;


static void comb_setdamp(t_freeverb *x, double val);
static void comb_setfeedback(t_freeverb *x, double val);
static inline double comb_processL(t_freeverb *x, int filteridx, double input);
static inline double comb_processR(t_freeverb *x, int filteridx, double input);
//static void allpass_setfeedback(t_freeverb *x, double val);
static inline double allpass_processL(t_freeverb *x, int filteridx, double input);
static inline double allpass_processR(t_freeverb *x, int filteridx, double input);



void freeverb_perform64(t_freeverb *x, t_object *dsp64, double **ins, long numins,
                     double **outs, long numouts, long sampleframes, long flags, void *userparam);
void freeverb_perf8(t_freeverb *x, t_object *dsp64, double **ins, long numins,
                    double **outs, long numouts, long sampleframes, long flags, void *userparam);
void freeverb_dsp64(t_freeverb *x, t_object *dsp64, short *count, double samplerate,
                    long maxvectorsize, long flags);


static void freeverb_update(t_freeverb *x);
static void freeverb_setroomsize(t_freeverb *x, double value);
static double freeverb_getroomsize(t_freeverb *x);
static void freeverb_setdamp(t_freeverb *x, double value);
static double freeverb_getdamp(t_freeverb *x);
static void freeverb_setwet(t_freeverb *x, double value);
static double freeverb_getwet(t_freeverb *x);
static void freeverb_setdry(t_freeverb *x, double value);
static double freeverb_getdry(t_freeverb *x);
static void freeverb_setwidth(t_freeverb *x, double value);
//static double freeverb_getwidth(t_freeverb *x);
static void freeverb_setmode(t_freeverb *x, long value);
static double freeverb_getmode(t_freeverb *x);
static void freeverb_setbypass(t_freeverb *x, long value);
static void freeverb_mute(t_freeverb *x);
static double freeverb_getdb(double f);
static void freeverb_print(t_freeverb *x);


void    *freeverb_new( double val );
void    freeverb_assist(t_freeverb *x, void *b, long m, long a, char *s);
static void    freeverb_free(t_freeverb *x);



int C74_EXPORT main(void) {
	t_class *c;
	
	c = class_new("freeverb~", (method)freeverb_new, (method)freeverb_free, (short)sizeof(t_freeverb), 0L, A_DEFFLOAT, 0L);
	class_addmethod(c, (method)freeverb_dsp64, "dsp64", A_CANT, 0);
	class_addmethod(c, (method)freeverb_setroomsize, "roomsize", A_FLOAT, 0);
    class_addmethod(c, (method)freeverb_setdamp, "damping", A_FLOAT, 0);
    class_addmethod(c, (method)freeverb_setwidth, "width", A_FLOAT, 0);
    class_addmethod(c, (method)freeverb_setwet, "wet", A_FLOAT, 0);
    class_addmethod(c, (method)freeverb_setdry, "dry", A_FLOAT, 0);
    class_addmethod(c, (method)freeverb_setmode, "freeze", A_LONG, 0);
    class_addmethod(c, (method)freeverb_setbypass, "bypass", A_LONG, 0);
    
    class_addmethod(c, (method)freeverb_mute, "clear", 0);
    class_addmethod(c, (method)freeverb_print, "print", 0);
    
    class_addmethod(c, (method)freeverb_assist, "assist", A_CANT,0);
	class_dspinit(c);
	class_register(CLASS_BOX, c);
	freeverb_class = c;
	
	
	// attributes ==== noch nicht ganz fertig
	/*
	CLASS_ATTR_LONG(c,"interp", 0, t_myObj, interpol);
	CLASS_ATTR_DEFAULT_SAVE(c, "interp", 0, "1");		// default geht nicht.
	CLASS_ATTR_SAVE(c, "interp", 0);
	CLASS_ATTR_MIN(c, "interp", 0, "0");
	CLASS_ATTR_MAX(c, "interp", 0, "1");
	*/
	
    post(version);
	
	return 0;
}



static void comb_setdamp(t_freeverb *x, double val)
{
    x->x_combdamp1 = val;
    x->x_combdamp2 = 1-val;
}

static void comb_setfeedback(t_freeverb *x, double val)
{
    x->x_combfeedback = val;
}



// Big to inline - but crucial for speed
static inline double comb_processL(t_freeverb *x, int filteridx, double input)
{
    double output;
    int bufidx = x->x_combidxL[filteridx];
    
    output = x->x_bufcombL[filteridx][bufidx];
    FIX_DENORM_NAN_DOUBLE(output);
    
    x->x_filterstoreL[filteridx] = (output*x->x_combdamp2) + (x->x_filterstoreL[filteridx]*x->x_combdamp1);
    FIX_DENORM_NAN_DOUBLE(x->x_filterstoreL[filteridx]);
    
    x->x_bufcombL[filteridx][bufidx] = input + (x->x_filterstoreL[filteridx]*x->x_combfeedback);
    
    if(++x->x_combidxL[filteridx] >= x->x_combtuningL[filteridx]) x->x_combidxL[filteridx] = 0;
    
    return output;
}


static inline double comb_processR(t_freeverb *x, int filteridx, double input)
{
    double output;
    int bufidx = x->x_combidxR[filteridx];
    
    output = x->x_bufcombR[filteridx][bufidx];
    FIX_DENORM_NAN_DOUBLE(output);
    
    x->x_filterstoreR[filteridx] = (output*x->x_combdamp2) + (x->x_filterstoreR[filteridx]*x->x_combdamp1);
    FIX_DENORM_NAN_DOUBLE(x->x_filterstoreR[filteridx]);
    
    x->x_bufcombR[filteridx][bufidx] = input + (x->x_filterstoreR[filteridx]*x->x_combfeedback);
    
    if(++x->x_combidxR[filteridx] >= x->x_combtuningR[filteridx]) x->x_combidxR[filteridx] = 0;
    
    return output;
}


#pragma mark allpass filter stuff  ----------------

/*
static void allpass_setfeedback(t_freeverb *x, double val)
{
    x->x_allpassfeedback = val;
}*/

// Big to inline - but crucial for speed
static inline double allpass_processL(t_freeverb *x, int filteridx, double input)
{
    double output;
    double bufout;
    int bufidx = x->x_allpassidxL[filteridx];
    
    bufout = (t_float)x->x_bufallpassL[filteridx][bufidx];
    FIX_DENORM_NAN_DOUBLE(bufout);
    
    output = -input + bufout;
    x->x_bufallpassL[filteridx][bufidx] = input + (bufout*x->x_allpassfeedback);
    
    if(++x->x_allpassidxL[filteridx] >= x->x_allpasstuningL[filteridx])
        x->x_allpassidxL[filteridx] = 0;
    
    return output;
}

static inline double allpass_processR(t_freeverb *x, int filteridx, double input)
{
    double output;
    double bufout;
    int bufidx = x->x_allpassidxR[filteridx];
    
    bufout = (double)x->x_bufallpassR[filteridx][bufidx];
    FIX_DENORM_NAN_DOUBLE(bufout);
    
    output = -input + bufout;
    x->x_bufallpassR[filteridx][bufidx] = input + (bufout*x->x_allpassfeedback);
    
    if(++x->x_allpassidxR[filteridx] >= x->x_allpasstuningR[filteridx])
        x->x_allpassidxR[filteridx] = 0;
    
    return output;
}





#pragma mark DSP routines ---------------------


//64-bit dsp method
void freeverb_dsp64(t_freeverb *x, t_object *dsp64, short *count, double samplerate,
				 long maxvectorsize, long flags) {
    
    if(maxvectorsize & 7) {	// check whether block size is multiple of 8
        object_method(dsp64, gensym("dsp_add64"), x, freeverb_perform64, 0, NULL);
        //post("is no multiple of 8!");
    }
    else {
        
        object_method(dsp64, gensym("dsp_add64"), x, freeverb_perf8, 0, NULL);
        //post("is multiple of 8!");
    }
	
	x->sr = samplerate;
	if(x->sr<=0) x->sr = 44100.0;

}





// 64 bit signal input version
void freeverb_perform64(t_freeverb *x, t_object *dsp64, double **ins, long numins,
					   double **outs, long numouts, long sampleframes, long flags, void *userparam){
	
    t_double *in1 = ins[0];
    t_double *in2 = ins[1];
    t_double *out1 = outs[0];
    t_double *out2 = outs[1];
    
    long n = sampleframes;
    int i;
    t_double outL, outR, inL, inR, input;
    
    
    if (x->x_obj.z_disabled)
        return;

    
    
    if(x->x_bypass)
    {
        // Bypass, so just copy input to output
        while(n--)
        {
            inL = *in1++;	// We have to copy first before we can write to output
            inR = *in2++;	// since this might be at the same memory position
            *out1++ = inL;
            *out2++ = inR;
        }
    }
    else
    {
        // DSP loop
        while(n--)
        {
            outL = outR = 0.;
            inL = *in1++;
            inR = *in2++;
            input = (inL + inR) * x->x_gain;
            
            // Accumulate comb filters in parallel
            for(i=0; i < numcombs; i++)
            {
                outL += comb_processL(x, i, input);
                outR += comb_processR(x, i, input);
            }
            
            // Feed through allpasses in series
            for(i=0; i < numallpasses; i++)
            {
                outL = allpass_processL(x, i, outL);
                outR = allpass_processR(x, i, outR);
            }
            
            // Calculate output REPLACING anything already there
            *out1++ = outL*x->x_wet1 + outR*x->x_wet2 + inL*x->x_dry;
            *out2++ = outR*x->x_wet1 + outL*x->x_wet2 + inR*x->x_dry;
        }
    }

}



// This is a hand unrolled version of the perform routine for
// DSP vector sizes that are multiples of 8
void freeverb_perf8(t_freeverb *x, t_object *dsp64, double **ins, long numins,
                        double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    // assign from parameters
    t_double *in1 = ins[0];
    t_double *in2 = ins[1];
    t_double *out1 = outs[0];
    t_double *out2 = outs[1];
    long n = sampleframes;
    int i;
    t_double outL[8], outR[8], inL[8], inR[8], input[8];
    
    double gain = x->x_gain;
    double wet1 = x->x_wet1;
    double wet2 = x->x_wet2;
    double dry = x->x_dry;
    
    
    if (x->x_obj.z_disabled)
        return;

    
    if(x->x_bypass)
    {
        // Bypass, so just copy input to output
        for(; n; n -= 8, out1 += 8, out2 += 8, in1 += 8, in2 += 8)
        {
            inL[0] = in1[0];	// We have to copy first before we can write to output
            inR[0] = in2[0];	// since this might be at the same memory position
            out1[0] = inL[0];
            out2[0] = inR[0];
            inL[1] = in1[1];
            inR[1] = in2[1];
            out1[1] = inL[1];
            out2[1] = inR[1];
            inL[2] = in1[2];
            inR[2] = in2[2];
            out1[2] = inL[2];
            out2[2] = inR[2];
            inL[3] = in1[3];
            inR[3] = in2[3];
            out1[3] = inL[3];
            out2[3] = inR[3];
            inL[4] = in1[4];
            inR[4] = in2[4];
            out1[4] = inL[4];
            out2[4] = inR[4];
            inL[5] = in1[5];
            inR[5] = in2[5];
            out1[5] = inL[5];
            out2[5] = inR[5];
            inL[6] = in1[6];
            inR[6] = in2[6];
            out1[6] = inL[6];
            out2[6] = inR[6];
            inL[7] = in1[7];
            inR[7] = in2[7];
            out1[7] = inL[7];
            out2[7] = inR[7];
        }
    }
    else
    {
        // DSP loop
        for(; n; n -= 8, out1 += 8, out2 += 8, in1 += 8, in2 += 8)
        {
            outL[0] = outR [0]= 0.;
            inL[0] = in1[0];
            inR[0] = in2[0];
            input[0] = (inL[0] + inR[0]) * gain;
            
            outL[1] = outR [1]= 0.;
            inL[1] = in1[1];
            inR[1] = in2[1];
            input[1] = (inL[1] + inR[1]) * gain;
            
            outL[2] = outR [2]= 0.;
            inL[2] = in1[2];
            inR[2] = in2[2];
            input[2] = (inL[2] + inR[2]) * gain;
            
            outL[3] = outR [3]= 0.;
            inL[3] = in1[3];
            inR[3] = in2[3];
            input[3] = (inL[3] + inR[3]) * gain;
            
            outL[4] = outR [4]= 0.;
            inL[4] = in1[4];
            inR[4] = in2[4];
            input[4] = (inL[4] + inR[4]) * gain;
            
            outL[5] = outR [5]= 0.;
            inL[5] = in1[5];
            inR[5] = in2[5];
            input[5] = (inL[5] + inR[5]) * gain;
            
            outL[6] = outR [6]= 0.;
            inL[6] = in1[6];
            inR[6] = in2[6];
            input[6] = (inL[6] + inR[6]) * gain;
            
            outL[7] = outR [7]= 0.;
            inL[7] = in1[7];
            inR[7] = in2[7];
            input[7] = (inL[7] + inR[7]) * gain;
            
            // Accumulate comb filters in parallel
            for(i=0; i < numcombs; i++)
            {
                outL[0] += comb_processL(x, i, input[0]);
                outR[0] += comb_processR(x, i, input[0]);
                outL[1] += comb_processL(x, i, input[1]);
                outR[1] += comb_processR(x, i, input[1]);
                outL[2] += comb_processL(x, i, input[2]);
                outR[2] += comb_processR(x, i, input[2]);
                outL[3] += comb_processL(x, i, input[3]);
                outR[3] += comb_processR(x, i, input[3]);
                outL[4] += comb_processL(x, i, input[4]);
                outR[4] += comb_processR(x, i, input[4]);
                outL[5] += comb_processL(x, i, input[5]);
                outR[5] += comb_processR(x, i, input[5]);
                outL[6] += comb_processL(x, i, input[6]);
                outR[6] += comb_processR(x, i, input[6]);
                outL[7] += comb_processL(x, i, input[7]);
                outR[7] += comb_processR(x, i, input[7]);
            }
            
            // Feed through allpasses in series
            for(i=0; i < numallpasses; i++)
            {
                outL[0] = allpass_processL(x, i, outL[0]);
                outR[0] = allpass_processR(x, i, outR[0]);
                outL[1] = allpass_processL(x, i, outL[1]);
                outR[1] = allpass_processR(x, i, outR[1]);
                outL[2] = allpass_processL(x, i, outL[2]);
                outR[2] = allpass_processR(x, i, outR[2]);
                outL[3] = allpass_processL(x, i, outL[3]);
                outR[3] = allpass_processR(x, i, outR[3]);
                outL[4] = allpass_processL(x, i, outL[4]);
                outR[4] = allpass_processR(x, i, outR[4]);
                outL[5] = allpass_processL(x, i, outL[5]);
                outR[5] = allpass_processR(x, i, outR[5]);
                outL[6] = allpass_processL(x, i, outL[6]);
                outR[6] = allpass_processR(x, i, outR[6]);
                outL[7] = allpass_processL(x, i, outL[7]);
                outR[7] = allpass_processR(x, i, outR[7]);
            }
            
            // Calculate output REPLACING anything already there
            out1[0] = outL[0]*wet1 + outR[0]*wet2 + inL[0]*dry;
            out2[0] = outR[0]*wet1 + outL[0]*wet2 + inR[0]*dry;
            
            out1[1] = outL[1]*wet1 + outR[1]*wet2 + inL[1]*dry;
            out2[1] = outR[1]*wet1 + outL[1]*wet2 + inR[1]*dry;
            out1[2] = outL[2]*wet1 + outR[2]*wet2 + inL[2]*dry;
            out2[2] = outR[2]*wet1 + outL[2]*wet2 + inR[2]*dry;
            out1[3] = outL[3]*wet1 + outR[3]*wet2 + inL[3]*dry;
            out2[3] = outR[3]*wet1 + outL[3]*wet2 + inR[3]*dry;
            out1[4] = outL[4]*wet1 + outR[4]*wet2 + inL[4]*dry;
            out2[4] = outR[4]*wet1 + outL[4]*wet2 + inR[4]*dry;
            out1[5] = outL[5]*wet1 + outR[5]*wet2 + inL[5]*dry;
            out2[5] = outR[5]*wet1 + outL[5]*wet2 + inR[5]*dry;
            out1[6] = outL[6]*wet1 + outR[6]*wet2 + inL[6]*dry;
            out2[6] = outR[6]*wet1 + outL[6]*wet2 + inR[6]*dry;
            out1[7] = outL[7]*wet1 + outR[7]*wet2 + inL[7]*dry;
            out2[7] = outR[7]*wet1 + outL[7]*wet2 + inR[7]*dry;
        }
    }

}



# pragma mark general parameter & calculation stuff ----

// recalculate internal values after parameter change
static void freeverb_update(t_freeverb *x)
{
    x->x_wet1 = x->x_wet*(x->x_width/2 + 0.5);
    x->x_wet2 = x->x_wet*((1-x->x_width)/2);
    
    if (x->x_mode >= freezemode)
    {
        x->x_roomsize1 = 1.;
        x->x_damp1 = 0.;
        x->x_gain = muted;
    }
    else
    {
        x->x_roomsize1 = x->x_roomsize;
        x->x_damp1 = x->x_damp;
        x->x_gain = (float)fixedgain;
    }
    
    comb_setfeedback(x, x->x_roomsize1);
    comb_setdamp(x, x->x_damp1);
}

// the following functions set / get the parameters
static void freeverb_setroomsize(t_freeverb *x, double value)
{
    x->x_roomsize = (value*scaleroom) + offsetroom;
    freeverb_update(x);
}

static double freeverb_getroomsize(t_freeverb *x)
{
    return (x->x_roomsize-offsetroom)/scaleroom;
}

static void freeverb_setdamp(t_freeverb *x, double value)
{
    x->x_damp = value*scaledamp;
    freeverb_update(x);
}

static double freeverb_getdamp(t_freeverb *x)
{
    return x->x_damp/scaledamp;
}

static void freeverb_setwet(t_freeverb *x, double value)
{
    x->x_wet = value*scalewet;
    freeverb_update(x);
}

static double freeverb_getwet(t_freeverb *x)
{
    return (x->x_wet/scalewet);
}

static void freeverb_setdry(t_freeverb *x, double value)
{
    x->x_dry = value*scaledry;
}

static double freeverb_getdry(t_freeverb *x)
{
    return (x->x_dry/scaledry);
}

static void freeverb_setwidth(t_freeverb *x, double value)
{
    x->x_width = value;
    freeverb_update(x);
}

/*
static double freeverb_getwidth(t_freeverb *x)
{
    return x->x_width;
}*/

static void freeverb_setmode(t_freeverb *x, long value)
{
    x->x_mode = value;
    freeverb_update(x);
}

static double freeverb_getmode(t_freeverb *x)
{
    if (x->x_mode >= freezemode)
        return 1;
    else
        return 0;
}

static void freeverb_setbypass(t_freeverb *x, long value)
{
    x->x_bypass = value;
    if(x->x_bypass)freeverb_mute(x);
}

// fill delay lines with silence
static void freeverb_mute(t_freeverb *x)
{
    int i;
    
    if (freeverb_getmode(x) >= freezemode)
        return;
    
    for (i=0;i<numcombs;i++)
    {
        memset(x->x_bufcombL[i], 0x0, x->x_combtuningL[i]*sizeof(double));
        memset(x->x_bufcombR[i], 0x0, x->x_combtuningR[i]*sizeof(double));
    }
    for (i=0;i<numallpasses;i++)
    {
        memset(x->x_bufallpassL[i], 0x0, x->x_allpasstuningL[i]*sizeof(double));
        memset(x->x_bufallpassR[i], 0x0, x->x_allpasstuningR[i]*sizeof(double));
    }
}

// convert gain factor into dB
static double freeverb_getdb(double f)
{
    if (f <= 0)	// equation does not work for 0...
    {
        return (-96);	// ...so we output max. damping
    }
    else
    {
        float val = (20./LOGTEN * log(f));
        return (val);
    }
}

static void freeverb_print(t_freeverb *x)
{
    post("freeverb~:");
    if(x->x_bypass) {
        post("  bypass: on");
    } else post("  bypass: off");
    if(!freeverb_getmode(x)) {
        post("  mode: normal");
    } else post("  mode: freeze");
    post("  roomsize: %g", freeverb_getroomsize(x)*scaleroom+offsetroom);
    post("  damping: %g %%", freeverb_getdamp(x)*100);
    post("  width: %g %%", x->x_width * 100);
    post("  wet level: %g dB", freeverb_getdb(freeverb_getwet(x)*scalewet));
    post("  dry level: %g dB", freeverb_getdb(freeverb_getdry(x)*scaledry));
}

// clean up
static void freeverb_free(t_freeverb *x)
{
    int i;
    
    dsp_free((t_pxobject *)x);
    
    // free memory used by delay lines
    for(i = 0; i < numcombs; i++)
    {
        sysmem_freeptr(x->x_bufcombL[i]);
        sysmem_freeptr(x->x_bufcombR[i]);
    }
    
    for(i = 0; i < numallpasses; i++)
    {
        sysmem_freeptr(x->x_bufallpassL[i]);
        sysmem_freeptr(x->x_bufallpassR[i]);
    }
}




void *freeverb_new(double val)
{
    int i;
	t_freeverb *x = object_alloc(freeverb_class);
	if(x) {
		dsp_setup((t_pxobject*)x, 2);			// two signal inlets
		//outlet_new((t_object *)x, "signal"); 
		outlet_new(x, "signal");
        outlet_new(x, "signal");
		
		float sr = sys_getsr();
        
		if(sr <= 0)
			sr = 44100.;
        
        x->sr = sr;
		
        // TODO: zero out struct??
        
        // recalculate the reverb parameters in case we don't run at 44.1kHz
        for(i = 0; i < numcombs; i++)
        {
            x->x_combtuningL[i] = (int)(combtuningL[i] * sr / 44100);
            x->x_combtuningR[i] = (int)(combtuningR[i] * sr / 44100);
        }
        for(i = 0; i < numallpasses; i++)
        {
            x->x_allpasstuningL[i] = (int)(allpasstuningL[i] * sr / 44100);
            x->x_allpasstuningR[i] = (int)(allpasstuningL[i] * sr / 44100);
        }
        
        // get memory for delay lines
        for(i = 0; i < numcombs; i++)
        {
            x->x_bufcombL[i] = (double *) sysmem_newptrclear(x->x_combtuningL[i]*sizeof(double));
            x->x_bufcombR[i] = (double *) sysmem_newptrclear(x->x_combtuningR[i]*sizeof(double));
            x->x_combidxL[i] = 0;
            x->x_combidxR[i] = 0;
        }
        for(i = 0; i < numallpasses; i++)
        {
            x->x_bufallpassL[i] = (double *) sysmem_newptrclear(x->x_allpasstuningL[i]*sizeof(double));
            x->x_bufallpassR[i] = (double *) sysmem_newptrclear(x->x_allpasstuningR[i]*sizeof(double));
            x->x_allpassidxL[i] = 0;
            x->x_allpassidxR[i] = 0;
        }
        
        // set default values
        x->x_allpassfeedback = 0.5;
        x->x_skip = 1;	// we use every sample
        freeverb_setwet(x, initialwet);
        freeverb_setroomsize(x, initialroom);
        freeverb_setdry(x, initialdry);
        freeverb_setdamp(x, initialdamp);
        freeverb_setwidth(x, initialwidth);
        freeverb_setmode(x, initialmode);
        freeverb_setbypass(x, initialbypass);
        
        // buffers will be full of rubbish - so we MUST mute them
        freeverb_mute(x);

		
		//attr_args_process(x, argc, argv);			// process attributes
	}
	
	else {
		object_free(x);
		x = NULL;
	}
		
	
	return x;
}


void freeverb_assist(t_freeverb *x, void *b, long m, long a, char *s) {
    
	if (m == ASSIST_INLET) {
		switch(a) {
			case 0: sprintf (s,"(signal/message) Left Input & Control Messages"); break;
            case 1: sprintf(s, "(signal) Right Input"); break;
		}
	}
	else {
		switch(a) {
			case 0: sprintf(s, "(signal) Left Output"); break;
			case 1: sprintf(s, "(signal) Right Output");; break;
		}
		
	}
}
