#ifndef GVERBDSP_H
#define GVERBDSP_H

#include "ext.h"
#include "z_dsp.h"	// needed for denormalization makros

// Convert a value in dB's to a coefficent
#define DB_CO(g) ((g) > -90.0 ? pow(10.0, (g) * 0.05) : 0.0)
// and back to dB
#define CO_DB(g) ((g) != 0.0 ? 20.0/log(10) * log((g)) : -90.0)

typedef struct {
  int size;
  int idx;
  double *buf;
} ty_fixeddelay;

typedef struct {
  int size;
  double coeff;
  int idx;
  double *buf;
} ty_diffuser;

typedef struct {
  double damping;
  double delay;
} ty_damper;

extern ty_diffuser *diffuser_make(int, double);
extern void diffuser_free(ty_diffuser *);
extern void diffuser_flush(ty_diffuser *);

extern ty_damper *damper_make(double);
extern void damper_free(ty_damper *);
extern void damper_flush(ty_damper *);

extern ty_fixeddelay *fixeddelay_make(int);
extern void fixeddelay_free(ty_fixeddelay *);
extern void fixeddelay_flush(ty_fixeddelay *);

extern int isprime(int);
extern int nearest_prime(int, double);
extern int ff_round(double d);       
//extern int ff_trunc(float f);

static inline double diffuser_do(ty_diffuser *p, double x)
{
	double y,w;

	w = x - p->buf[p->idx]*p->coeff;
	FIX_DENORM_NAN_DOUBLE(w);
	y = p->buf[p->idx] + w*p->coeff;
	p->buf[p->idx] = w;
	p->idx = (p->idx + 1) % p->size;
	return(y);
}

static inline double fixeddelay_read(ty_fixeddelay *p, int n)
{
	int i;

	i = (p->idx - n + p->size) % p->size;
	return(p->buf[i]);
}

static inline void fixeddelay_write(ty_fixeddelay *p, double x)
{
	FIX_DENORM_NAN_DOUBLE(x);
	p->buf[p->idx] = x;
	p->idx = (p->idx + 1) % p->size;
}

static inline void damper_set(ty_damper *p, double damping)
{ 
	p->damping = damping;
} 
  
static inline double damper_do(ty_damper *p, double x)
{ 
	double y;

	y = x*(1.0-p->damping) + p->delay*p->damping;
	p->delay = y;
	return(y);
}

#endif
