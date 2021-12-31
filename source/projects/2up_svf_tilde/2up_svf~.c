#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"
#include "ext_common.h"

//
// 2up_svf~.c
// state-variable filter module for Max/MSP. 
// based on original SVF algorithm by Hal Chamberlin. 
// oversampled and with other tweaks for musical sound.
// by Randy Jones rej@2uptech.com.
// 
// who	when 		what
// rej	11/22/00	created
// rej	7/29/03	Carbon OS X compatible, fast sin approx, improved math

// vb	08/27/13	updated to max sdk 6.1 and 64bit 
// vb	07/07/15	updated to use max sdk 6.1.4


static t_class *svf_class;

typedef struct _svf
{
	t_pxobject x_obj;
	t_double freq;
	t_double q;
	int mode;
	long freq_connected;	// is freq. lead connected to signal?
	long q_connected;		// how about q?
	t_double b0;
	t_double b1;
	t_double b2;
	t_double fs;
	t_double one_over_fs;
	t_double qc;
} t_svf;


void *svf_new(t_symbol *s, long ac, t_atom *av);
//t_int *offset_perform(t_int *w);
t_int *svf_perform(t_int *w);
void svf_float(t_svf *x, double f);
void svf_freq(t_svf *x, double f);
void svf_q(t_svf *x, double f);
void svf_mode(t_svf *x, long n);
void svf_clear(t_svf *x);
void svf_dsp(t_svf *x, t_signal **sp, short *count);
void svf_assist(t_svf *x, void *b, long m, long a, char *s);

// 64bit DSP methods
void svf_dsp64(t_svf *x, t_object *dsp64, short *count, double samplerate, 
				 long maxvectorsize, long flags);
void svf_perform64(t_svf *x, t_object *dsp64, double **ins, long numins, 
					 double **outs, long numouts, long sampleframes, long flags, void *userparam);



int C74_EXPORT main(void) {
	t_class *c;
	
	c = class_new("2up_svf~", (method)svf_new, (method)dsp_free, (short)sizeof(t_svf), 0L, 
				  A_GIMME, 0L);
	class_addmethod(c, (method)svf_dsp, "dsp", A_CANT, 0);
	class_addmethod(c, (method)svf_dsp64, "dsp64", A_CANT, 0);
	
	class_addmethod(c, (method)svf_float, "float", A_FLOAT, 0);	
	class_addmethod(c, (method)svf_mode, "int", A_LONG, 0);
	
	class_addmethod(c, (method)svf_assist, "assist", A_CANT,0);
	class_addmethod(c, (method)svf_clear, "clear", 0);
	class_dspinit(c);
	class_register(CLASS_BOX, c);
	svf_class = c;
	
	return 0;
}



void svf_assist(t_svf *x, void *b, long m, long a, char *s) {
	if (m == ASSIST_INLET) {
		switch(a) {
			case 0: sprintf (s,"(signal) audio in"); break;
			case 1: sprintf (s,"(signal/float) cutoff frequency"); break;
			case 2: sprintf (s,"(signal/float) q"); break;
		}
	}
	else {
		switch(a) {
			case 0: sprintf (s,"(signal) signal out"); break;
		}
	}
}


void *svf_new(t_symbol *s, long ac, t_atom *av)
{
	t_svf *x = object_alloc(svf_class);
	if(x) {
		dsp_setup((t_pxobject*)x, 3);			// one signal inlet
		outlet_new((t_pxobject*)x, "signal"); 
		
		// defaults
		x->freq = 0.;
		x->q = 0.0;
		x->mode = 0;
		svf_clear(x);
		
		if (ac > 0)
		 {
			switch (atom_gettype(av)) {
				case A_FLOAT:
					x->freq = atom_getfloat(av);	//av[0].a_w.w_float;
					break;
				case A_LONG:
					x->freq = atom_getlong(av);	//av[0].a_w.w_long;
					break;
				default:
					object_post((t_object *)x, "expected number for param 1 (frequency)");
					break;
			}
		 }
		if (ac > 1)
		 {
			switch (atom_gettype(av+1)) {
				case A_FLOAT:
					x->q = atom_getfloat(av+1);	//av[1].a_w.w_float;
					break;
				default:
					object_post((t_object *)x, "expected float for param 2 (resonance)");
					break;
			}
		 }
	}
	
	else {
		object_free(x);
		x = NULL;
	}
	
	return x;
}



void svf_float(t_svf *x, double f)
{
	//int inlet = ((t_pxobject*)x)->z_in;
	int inlet = proxy_getinlet((t_object *)x);
	if (inlet == 1) x->freq = f;
	else if (inlet == 2) x->q = f;
}



void svf_freq(t_svf *x, double f)
{
	x->freq = f;
}

void svf_q(t_svf *x, double f)
{
	x->q = f;
}

void svf_mode(t_svf *x, long n)
{
	x->mode = n;
}

void svf_clear(t_svf *x) 
{
    x->b0 = 0.;
    x->b1 = 0.;
    x->b2 = 0.;
}



// 32-bit dsp method
void svf_dsp(t_svf *x, t_signal **sp, short *count) {

	x->fs = sp[0]->s_sr;
	if(x->fs <= 0) x->fs = 44100;		// check for valid sample rate
	x->one_over_fs = 1.0/x->fs;
	x->freq_connected = count[1];
	x->q_connected = count[2];
	
	// clear filter history
	x->b0 = x->b1 = x->b2 = 0.;
	
	dsp_add(svf_perform, 6, x, sp[0]->s_vec, sp[1]->s_vec, 
			sp[2]->s_vec, sp[3]->s_vec, sp[0]->s_n);

}


//64-bit dsp method
void svf_dsp64(t_svf *x, t_object *dsp64, short *count, double samplerate, 
				 long maxvectorsize, long flags) {
	
	x->fs = samplerate;
	if(x->fs <= 0) x->fs = 44100;		// check for valid sample rate
	x->one_over_fs = 1.0/x->fs;
	x->freq_connected = count[1];
	x->q_connected = count[2];
	
	// clear filter history
	x->b0 = x->b1 = x->b2 = 0.;
	
	object_method(dsp64, gensym("dsp_add64"), x, svf_perform64, 0, NULL);

}


inline double sin_e(double x);
inline double sin_e(double x)
{
    return x - (x*x*x*0.1666666666666);
}


void calc_params(t_svf *x, t_int *w, int offset, double * pm_f, double * p_q);
void calc_params(t_svf *x, t_int *w, int offset, double * pm_f, double * p_q)
{
	double lq, fs, fc;
	double oversample = 1.0 / 2.0; // 1/2 = 2x
	fs = x->fs;
	fc = x->freq_connected ? ((float *)w[3])[offset] : x->freq;
	lq = (x->q_connected ? ((float *)w[4])[offset] : x->q);
	
	//if (fc > fs*0.5) fc = fs*0.5;
	//if (fc < 0) fc = 0;
	fc = CLAMP(fc, 0, fs*0.5);
	lq = 0.99 - lq;
	//if (lq > 0.7) lq = 0.7;	// by experiment, prevent blowups at high fc and low q
	//if (lq < -0.01) lq = -0.01; 
	lq = CLAMP(lq, -0.01, 0.7);
	*p_q = lq;
	*pm_f     = 2.0 * sin_e(PI*fc*x->one_over_fs * oversample);	
    
	// coeff for one-pole
	x->qc = (lq + 0.02)*40000.*x->one_over_fs;
}


t_int *svf_perform(t_int *w) {
	
	t_svf *x = (t_svf *)(w[1]);
   	t_float *p_in = (t_float *)(w[2]);
	t_float *p_out = (t_float *)(w[5]);
	long n = w[6];
	int i = 0;
	int do_signal_params = 0;
	
	// tiny bit of noise added to prevent denormals
	static const double denorm_noise[3] = {1e-10, 0, -1e-10};
	long nk = 0;
	
	t_double in, m_in, wc; 
	t_double m_h, m_b, m_l; 
	t_double m_f, q;
	
	if (x->x_obj.z_disabled)
		goto bail;
	
	do_signal_params = x->freq_connected || x->q_connected;
	if (!do_signal_params)
		calc_params(x, w, 0, &m_f, &q);
	
	// restore history
 	m_b = x->b0;
	m_l = x->b1;
	m_in = x->b2;
	
	// intermediates
	wc = 4.*x->one_over_fs;
	
	switch (x->mode)
	{
	 case 0: // lowpass
		while(n--)
		 {	
			 in = (p_in[i]);	 
			 
			 if ( (0 == (i & 0xF)) && do_signal_params )
				 calc_params(x, w, i, &m_f, &q);
			 
			 // one pole LP filter input, based on q
			 m_in = m_in*(1. - x->qc) + in*(x->qc);
			 
			 // regular state variable code here
			 m_l = m_l + (m_f * m_b);
			 m_h = m_in - m_l - q * m_b;
			 m_b = m_b + m_f * m_h;
			 
			 // repeat for stability (2x oversampling).
			 m_l = m_l + (m_f * m_b);
			 m_h = m_in - m_l - q * m_b;
			 m_b = m_b + m_f * m_h;
			 
			 m_l += denorm_noise[nk++];
			 if (nk >= 3) nk=0;
			 
			 // waveshape
			 m_b = m_b - m_b*m_b*m_b*wc;
			 
			 p_out[i] = m_l;
			 
			 i++;
		 }
		break;
	 case 1: // highpass
		while(n--)
		 {	
			 in = (p_in[i]);	
			 
			 if (do_signal_params && (0 == (i & 0xF)))
				 calc_params(x, w, i, &m_f, &q);
			 
			 m_in = m_in*(1. - x->qc) + in*(x->qc);
			 
			 m_l = m_l + (m_f * m_b);
			 m_h = m_in - m_l - q * m_b;
			 m_b = m_b + m_f * m_h;
			 
			 m_l = m_l + (m_f * m_b);
			 m_h = m_in - m_l - q * m_b;
			 m_b = m_b + m_f * m_h;
			 
			 m_l += denorm_noise[nk++];
			 if (nk >= 3) nk=0;
			 
			 // waveshape
			 m_b = m_b - m_b*m_b*m_b*wc;
			 
			 p_out[i] = m_h;
			 i++;
		 }
		break;
	 case 2: // bandpass
		while(n--)
		 {	
			 in = (p_in[i]);	
			 
			 if (do_signal_params && (0 == (i & 0xF)))
				 calc_params(x, w, i, &m_f, &q);
			 
			 m_in = m_in*(1. - x->qc) + in*(x->qc);
			 
			 m_l = m_l + (m_f * m_b);
			 m_h = m_in - m_l - q * m_b;
			 m_b = m_b + m_f * m_h;
			 
			 m_l = m_l + (m_f * m_b);
			 m_h = m_in - m_l - q * m_b;
			 m_b = m_b + m_f * m_h;
			 
			 m_l += denorm_noise[nk++];
			 if (nk >= 3) nk=0;
			 
			 // waveshape
			 m_b = m_b - m_b*m_b*m_b*wc;
			 
			 p_out[i] = m_b;
			 i++;
		 }
		break;
	 case 3: // notch
		while(n--)
		 {	
			 in = (p_in[i]);
			 
			 if (do_signal_params && (0 == (i & 0xF)))
				 calc_params(x, w, i, &m_f, &q);
			 
			 m_in = m_in*(1. - x->qc) + in*(x->qc);
			 
			 m_l = m_l + (m_f * m_b);
			 m_h = m_in - m_l - q * m_b;
			 m_b = m_b + m_f * m_h;
			 
			 m_l = m_l + (m_f * m_b);
			 m_h = m_in - m_l - q * m_b;
			 m_b = m_b + m_f * m_h;
			 
			 m_l += denorm_noise[nk++];
			 if (nk >= 3) nk=0;
			 
			 // waveshape
			 m_b = m_b - m_b*m_b*m_b*wc;
			 
			 p_out[i] = m_h + m_l;
			 i++;
		 }
		break;
	}	
	
	// save history
	x->b0 = m_b;
	x->b1 = m_l;
	x->b2 = m_in;
	
bail:
	return (w+7);
}



void calc_paramsD(t_svf *x, double **ins, int offset, double * pm_f, double * p_q);
void calc_paramsD(t_svf *x, double **ins, int offset, double * pm_f, double * p_q)
{
	double lq, fs, fc;
	double oversample = 0.5; // 1/2 = 2x
	fs = x->fs;
	fc = x->freq_connected ? ins[1][offset] : x->freq;
	lq = x->q_connected ? ins[2][offset] : x->q;
	
	fc = CLAMP(fc, 0, fs*0.5);
	lq = 0.99 - lq; 
	lq = CLAMP(lq, -0.01, 0.7);
	*p_q = lq;
	*pm_f     = 2.0 * sin_e(PI*fc*x->one_over_fs * oversample);	
    
	// coeff for one-pole
	x->qc = (lq + 0.02)*40000.*x->one_over_fs;
}


// 64 bit signal input version
void svf_perform64(t_svf *x, t_object *dsp64, double **ins, long numins, 
					   double **outs, long numouts, long sampleframes, long flags, void *userparam) {
	
	t_double *p_in = ins[0];
	t_double *p_out = outs[0];	
	int n = sampleframes;		
	int i = 0;
	int do_signal_params = 0;
	
	if (x->x_obj.z_disabled)
		return;
	
	// tiny bit of noise added to prevent denormals
	static const double denorm_noise[3] = {1e-10, 0, -1e-10};
	long nk = 0;
	
	t_double in, m_in, wc; 
	t_double m_h, m_b, m_l; 
	t_double m_f, q;
	
	do_signal_params = x->freq_connected || x->q_connected;
	if (!do_signal_params)
		calc_paramsD(x, ins, 0, &m_f, &q);
	
	// restore history
 	m_b = x->b0;
	m_l = x->b1;
	m_in = x->b2;
	
	// intermediates
	wc = 4.*x->one_over_fs;
	
	switch (x->mode)
	{
	 case 0: // lowpass
		while(n--)
		 {	
			 in = (p_in[i]);	 
			 
			 if ( (0 == (i & 0xF)) && do_signal_params )
				 calc_paramsD(x, ins, i, &m_f, &q);
			 
			 // one pole LP filter input, based on q
			 m_in = m_in*(1. - x->qc) + in*(x->qc);
			 
			 // regular state variable code here
			 m_l = m_l + (m_f * m_b);
			 m_h = m_in - m_l - q * m_b;
			 m_b = m_b + m_f * m_h;
			 
			 // repeat for stability (2x oversampling).
			 m_l = m_l + (m_f * m_b);
			 m_h = m_in - m_l - q * m_b;
			 m_b = m_b + m_f * m_h;
			 
			 m_l += denorm_noise[nk++];
			 if (nk >= 3) nk=0;
			 
			 // waveshape
			 m_b = m_b - m_b*m_b*m_b*wc;
			 
			 p_out[i] = m_l;
			 
			 i++;
		 }
		break;
	 case 1: // highpass
		while(n--)
		 {	
			 in = (p_in[i]);	
			 
			 if (do_signal_params && (0 == (i & 0xF)))
				 calc_paramsD(x, ins, i, &m_f, &q);
			 
			 m_in = m_in*(1. - x->qc) + in*(x->qc);
			 
			 m_l = m_l + (m_f * m_b);
			 m_h = m_in - m_l - q * m_b;
			 m_b = m_b + m_f * m_h;
			 
			 m_l = m_l + (m_f * m_b);
			 m_h = m_in - m_l - q * m_b;
			 m_b = m_b + m_f * m_h;
			 
			 m_l += denorm_noise[nk++];
			 if (nk >= 3) nk=0;
			 
			 // waveshape
			 m_b = m_b - m_b*m_b*m_b*wc;
			 
			 p_out[i] = m_h;
			 i++;
		 }
		break;
	 case 2: // bandpass
		while(n--)
		 {	
			 in = (p_in[i]);	
			 
			 if (do_signal_params && (0 == (i & 0xF)))
				 calc_paramsD(x, ins, i, &m_f, &q);
			 
			 m_in = m_in*(1. - x->qc) + in*(x->qc);
			 
			 m_l = m_l + (m_f * m_b);
			 m_h = m_in - m_l - q * m_b;
			 m_b = m_b + m_f * m_h;
			 
			 m_l = m_l + (m_f * m_b);
			 m_h = m_in - m_l - q * m_b;
			 m_b = m_b + m_f * m_h;
			 
			 m_l += denorm_noise[nk++];
			 if (nk >= 3) nk=0;
			 
			 // waveshape
			 m_b = m_b - m_b*m_b*m_b*wc;
			 
			 p_out[i] = m_b;
			 i++;
		 }
		break;
	 case 3: // notch
		while(n--)
		 {	
			 in = (p_in[i]);
			 
			 if (do_signal_params && (0 == (i & 0xF)))
				 calc_paramsD(x, ins, i, &m_f, &q);
			 
			 m_in = m_in*(1. - x->qc) + in*(x->qc);
			 
			 m_l = m_l + (m_f * m_b);
			 m_h = m_in - m_l - q * m_b;
			 m_b = m_b + m_f * m_h;
			 
			 m_l = m_l + (m_f * m_b);
			 m_h = m_in - m_l - q * m_b;
			 m_b = m_b + m_f * m_h;
			 
			 m_l += denorm_noise[nk++];
			 if (nk >= 3) nk=0;
			 
			 // waveshape
			 m_b = m_b - m_b*m_b*m_b*wc;
			 
			 p_out[i] = m_h + m_l;
			 i++;
		 }
		break;
	}
	
	
	// save history
	x->b0 = m_b;
	x->b1 = m_l;
	x->b2 = m_in;

}
