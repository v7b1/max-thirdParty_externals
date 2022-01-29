#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"


// the cverb object. A version of the csound reverb~

// 2018/09/09 - 64-bit version by volker boehm - http://vboehm.net
// ... does this really sound so crappy...?


typedef struct {
    t_pxobject v_obj;
    double v_revtime;
    double v_c1;
    double v_c2;
    double v_c3;
    double v_c4;
    double v_c5;
    double v_c6;
    double *v_p1;
    double *v_p2;
    double *v_p3;
    double *v_p4;
    double *v_p5;
    double *v_p6;
    double *v_adr1;
    double *v_adr2;
    double *v_adr3;
    double *v_adr4;
    double *v_adr5;
    double *v_adr6;
    double *v_endp;
    double *v_memory;
    long v_size;
    double v_sr;
    long v_revsizes[6];
    short v_connected;

} t_cverb;


static t_class *cverb_class;


// DSP methods
void cverb_dsp64(t_cverb *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void cverb_perform64(t_cverb *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
//

void cverb_float(t_cverb *x, double f);
void cverb_int(t_cverb *x, long n);
void cverb_calc(t_cverb *x);
void cverb_alloc(t_cverb *x);
void cverb_clear(t_cverb *x);
void cverb_free(t_cverb *x);
void *cverb_new(double f);
void cverb_assist(t_cverb *x, void *b, long m, long a, char *s);


double revtimes[] = { .0297, .0371, .0411, .0437, .005, .0017};


void ext_main(void *r) {
	t_class *c;
	
	c = class_new("cverb~", (method)cverb_new, (method)cverb_free, (long)sizeof(t_cverb),
                  0L, A_GIMME, 0L);
	//class_addmethod(c, (method)cverb_dsp, "dsp", A_CANT, 0);
	class_addmethod(c, (method)cverb_dsp64, "dsp64", A_CANT, 0);
	class_addmethod(c, (method)cverb_float, "float", A_FLOAT, 0);
	class_addmethod(c, (method)cverb_int, "int", A_LONG, 0);
    class_addmethod(c, (method)cverb_clear, "clear", 0);
	class_addmethod(c, (method)cverb_assist, "assist", A_CANT,0);
	class_dspinit(c);
	class_register(CLASS_BOX, c);
	cverb_class = c;
	
	post("cverb~ --- 64-bit version");

}




//64-bit dsp method
void cverb_dsp64(t_cverb *x, t_object *dsp64, short *count, double samplerate,
				 long maxvectorsize, long flags) {
    
    if (samplerate != x->v_sr) {
        object_post((t_object *)x, "sampling rate changed, reallocating");
        x->v_sr = samplerate;
        cverb_alloc(x);
    }
    if (!x->v_memory)
        return;
    cverb_clear(x);
    cverb_calc(x);
    x->v_connected = count[1];
    
	object_method(dsp64, gensym("dsp_add64"), x, cverb_perform64, 0, NULL);

}



// 64 bit signal input version
void cverb_perform64(t_cverb *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	t_double *in = ins[0];
	t_double *out = outs[0];	
	long n = sampleframes;

    t_double time = *ins[1];
    
    
    t_double c1,c2,c3,c4,c5,c6;
    t_double *p1,*p2,*p3,*p4,*p5,*p6,*endp;
    t_double cmbsum, y1, y2, z, val;
    
    if (x->v_obj.z_disabled)
        return;
    
    if (x->v_connected) {
        t_double revtime = time <= 0. ? .001 : time * 0.001;
        if (revtime != x->v_revtime) {
            x->v_revtime = revtime;
            cverb_calc(x);
        }
    }
    p1 = x->v_p1;
    p2 = x->v_p2;
    p3 = x->v_p3;
    p4 = x->v_p4;
    p5 = x->v_p5;
    p6 = x->v_p6;
    endp = x->v_endp;
    c1 = x->v_c1;
    c2 = x->v_c2;
    c3 = x->v_c3;
    c4 = x->v_c4;
    c5 = x->v_c5;
    c6 = x->v_c6;
    
    double output = 0;
    
    while (n--) {       //--n
        val = *in++;    //*++in;
        cmbsum = *p1 + *p2 + *p3 + *p4;
        *p1 = c1 * *p1 + val;
        *p2 = c2 * *p2 + val;
        *p3 = c3 * *p3 + val;
        *p4 = c4 * *p4 + val;
        p1++; p2++; p3++; p4++;
        y1 = *p5;
        *p5++ = z = c5 * y1 + cmbsum;
        y1 -= c5 * z;
        y2 = *p6;
        *p6++ = z = c6 * y2 + y1;
        output = y2 - c6 * z;
//        *out++ = y2 - c6 * z;       //*++out
        // maybe better check for denormals
        if ( IS_DENORM_DOUBLE(output)) {
            cverb_clear(x);
            output = 0.0;
//            object_post(NULL, "denormal!");
        }
        
        *out++ = output;
        if (p1 >= x->v_adr2)
            p1 = x->v_adr1;
        if (p2 >= x->v_adr3)
            p2 = x->v_adr2;
        if (p3 >= x->v_adr4)
            p3 = x->v_adr3;
        if (p4 >= x->v_adr5)
            p4 = x->v_adr4;
        if (p5 >= x->v_adr6)
            p5 = x->v_adr5;
        if (p6 >= endp)
            p6 = x->v_adr6;
    }
    x->v_p1 = p1;
    x->v_p2 = p2;
    x->v_p3 = p3;
    x->v_p4 = p4;
    x->v_p5 = p5;
    x->v_p6 = p6;

}



void cverb_float(t_cverb *x, double f)
{
    if (x->v_obj.z_in) {
        x->v_revtime = f <= 0. ? .001 : f * 0.001; // convert to seconds
        cverb_calc(x);
    }
}


// int input...
void cverb_int(t_cverb *x, long f)
{
    cverb_float(x, f);
}

void cverb_clear(t_cverb *x)
{
    if (!x->v_memory)
        return;
    set_zero64(x->v_memory, x->v_size);
    x->v_p1 = x->v_adr1;
    x->v_p2 = x->v_adr2;
    x->v_p3 = x->v_adr3;
    x->v_p4 = x->v_adr4;
    x->v_p5 = x->v_adr5;
    x->v_p6 = x->v_adr6;
}


void cverb_alloc(t_cverb *x)
{
    short i;
    long total = 0;
    
    for (i = 0; i < 6; i++) {
        x->v_revsizes[i] = revtimes[i] * x->v_sr;
        total += x->v_revsizes[i];
    }
    //post("total %ld",total);
    x->v_size = total;
    x->v_memory = (double*)sysmem_newptr(total * sizeof(double));
    if (!x->v_memory) {
        object_error((t_object *)x, "cverb: out of memory");
        return;
    }
    x->v_adr1 = x->v_memory;
    x->v_adr2 = x->v_adr1 + x->v_revsizes[0];
    x->v_adr3 = x->v_adr2 + x->v_revsizes[1];
    x->v_adr4 = x->v_adr3 + x->v_revsizes[2];
    x->v_adr5 = x->v_adr4 + x->v_revsizes[3];
    x->v_adr6 = x->v_adr5 + x->v_revsizes[4];
    x->v_endp = x->v_adr6 + x->v_revsizes[5];
    // consistency check:
    if (x->v_size != (x->v_endp - x->v_memory))
        object_post((t_object *)x, "revsizes inconsistent");
}


void cverb_calc(t_cverb *x)
{
    double *lptimp = revtimes;
    double logdrvt = -6.9078 / x->v_revtime;
    x->v_c1 = exp(logdrvt * *lptimp++);
    x->v_c2 = exp(logdrvt * *lptimp++);
    x->v_c3 = exp(logdrvt * *lptimp++);
    x->v_c4 = exp(logdrvt * *lptimp++);
    x->v_c5 = exp(logdrvt * *lptimp++);
    x->v_c6 = exp(logdrvt * *lptimp++);

}

void cverb_free(t_cverb *x)
{
    dsp_free((t_pxobject *)x);
    if (x->v_memory)
        sysmem_freeptr(x->v_memory);
}


void *cverb_new(double f)
{
	t_cverb *x = object_alloc(cverb_class);
	if(x) {
		dsp_setup((t_pxobject*)x, 2);
		
		outlet_new(x, "signal"); 
		
        x->v_memory = 0;
        x->v_size = 0;
        x->v_revtime = f <= 0. ? .001 : f * 0.001;
        x->v_sr = sys_getsr();
        cverb_alloc(x);
        cverb_clear(x);
        cverb_calc(x);

	} else {
		object_free(x);
		x = NULL;
	}
		
	
	return x;
}


void cverb_assist(t_cverb *x, void *b, long m, long a, char *s) {
	if (m == ASSIST_INLET) {
		switch(a) {
			case 0: sprintf (s,"(signal) audio in"); break;
            case 1: sprintf (s,"(signal/float) reverb time"); break;
		}
	}
	else {
		switch(a) {
			case 0: sprintf (s,"(signal) signal out"); break;
		}
		
	}
}
