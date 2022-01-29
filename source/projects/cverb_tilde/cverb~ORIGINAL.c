#include "ext.h"
#include "z_dsp.h"
#include <math.h>

// the cverb object. A version of the csound reverb~

void *cverb_class;

// eventually make a structure out of the 4 x 6?

typedef struct _cverb
{
	t_pxobject v_obj;
	float v_revtime;
	float v_c1;
	float v_c2;
	float v_c3;
	float v_c4;
	float v_c5;
	float v_c6;
	float *v_p1;
	float *v_p2;
	float *v_p3;
	float *v_p4;
	float *v_p5;
	float *v_p6;
	float *v_adr1;
	float *v_adr2;
	float *v_adr3;
	float *v_adr4;
	float *v_adr5;
	float *v_adr6;
	float *v_endp;
	float *v_memory;
	long v_size;
	float v_sr;
	long v_revsizes[6];
	short v_connected;
} t_cverb;

t_int *cverb_perform(t_int *w);
void cverb_dsp(t_cverb *x, t_signal **sp, short *count);
void cverb_float(t_cverb *x, double f);
void cverb_int(t_cverb *x, long n);
void cverb_calc(t_cverb *x);
void cverb_alloc(t_cverb *x);
void cverb_clear(t_cverb *x);
void cverb_free(t_cverb *x);
void *cverb_new(double f);
void cverb_assist(t_cverb *x, void *b, long m, long a, char *s);

int errno;

float revtimes[] = { .0297, .0371, .0411, .0437, .005, .0017};

void main(void)
{
	setup((t_messlist **)&cverb_class,cverb_new, (method)cverb_free,
		  (short)sizeof(t_cverb), 0L, A_DEFFLOAT, 0);
	addmess((method)cverb_dsp, "dsp", A_CANT, 0);
	addmess((method)cverb_assist, "assist", A_CANT, 0);
	addmess((method)cverb_clear, "clear", 0);
	addfloat((method)cverb_float);
	dsp_initclass();
	rescopy('STR#',3312);
}

void cverb_assist(t_cverb *x, void *b, long m, long a, char *s)
{
	assist_string(3312,m,a,1,3,s);
}

t_int *cverb_perform(t_int *w)
{
	t_float *in = (t_float *)(w[1]);
	t_float *out = (t_float *)(w[2]);
	t_float time = *(t_float *)(w[3]);
	t_cverb *x = (t_cverb *)(w[4]);
	int n = (int)(w[5]);
	float c1,c2,c3,c4,c5,c6;
	float *p1,*p2,*p3,*p4,*p5,*p6,*endp;
	float cmbsum, y1, y2, z, val;
	
	if (x->v_obj.z_disabled)
		goto out;
	
	if (x->v_connected) {
		float revtime = time <= 0. ? .001 : time * 0.001;
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
	
	while (--n) {
		val = *++in;
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
		*++out = y2 - c6 * z;
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
out:
	return (w+6);
}

void cverb_dsp(t_cverb *x, t_signal **sp, short *count)
{
	if (sp[0]->s_sr != x->v_sr) {
		post("sampling rate changed, reallocating");
		x->v_sr = sp[0]->s_sr;
		cverb_alloc(x);
	}
	if (!x->v_memory)
		return;
	cverb_clear(x);
	cverb_calc(x);
	x->v_connected = count[1];
	// in out time
	dsp_add(cverb_perform, 5, sp[0]->s_vec-1, sp[2]->s_vec-1, sp[1]->s_vec, x, sp[0]->s_n+1);
}

void cverb_float(t_cverb *x, double f)
{
	if (x->v_obj.z_in) {
		x->v_revtime = f <= 0. ? .001 : f * 0.001; // convert to seconds
		cverb_calc(x);
	}
}

void cverb_int(t_cverb *x, long n)
{
	cverb_float(x,(double)n);
}

void cverb_clear(t_cverb *x)
{
	if (!x->v_memory)
		return;
	set_zero(x->v_memory,x->v_size);
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
	x->v_memory = t_getbytes(total * sizeof(float));
	if (!x->v_memory) {
		error("cverb: out of memory");
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
		post("revsizes inconsistent");
}

void cverb_calc(t_cverb *x)
{
	float bl;
	
	float *lptimp = revtimes;
	float logdrvt = -6.9078 / x->v_revtime;
	x->v_c1 = exp(logdrvt * *lptimp++);
	x->v_c2 = exp(logdrvt * *lptimp++);
	x->v_c3 = exp(logdrvt * *lptimp++);
	x->v_c4 = exp(logdrvt * *lptimp++);
	x->v_c5 = exp(logdrvt * *lptimp++);
	x->v_c6 = exp(logdrvt * *lptimp++);
}

void cverb_free(t_cverb *x)
{
	if (x->v_memory)
		t_freebytes(x->v_memory,x->v_size * sizeof(float));
	dsp_free((t_pxobject *)x);
}

void *cverb_new(double f)
{
	t_cverb *x = (t_cverb *)newobject(cverb_class);
	dsp_setup((t_pxobject *)x,2);
	x->v_memory = 0;
	x->v_size = 0;
	x->v_revtime = f <= 0. ? .001 : f * 0.001;
	x->v_sr = sys_getsr();
	cverb_alloc(x);
	cverb_clear(x);
	cverb_calc(x);
	outlet_new((t_object *)x, "signal");
	return (x);
}