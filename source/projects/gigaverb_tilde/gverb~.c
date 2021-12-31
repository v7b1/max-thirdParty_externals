/*

        Copyright (C) 1999 Juhana Sadeharju
                       kouhia at nic.funet.fi

		Max/MSP port 2004 by Olaf Matthes, <olaf.matthes@gmx.de>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    */


#include "gverb~.h"
#include "gverbdsp.h"


char *version = "gigaverb~ 64bit, 1999-2006 Juhana Sadeharju / Olaf Matthes / Volker Böhm";


void *gverb_new(t_symbol *s, short argc, t_atom *argv)
{
    
    ty_gverb *p = (ty_gverb *)object_alloc(gverb_class);
    
    
	double maxroomsize = 300.0;
	double roomsize = 50.0;
	double revtime = 7.0;
	double damping = 0.5;
	double spread = 15.0;
	double inputbandwidth = 0.5;
	double drylevel = 0.0; //-1.9832f;
	double earlylevel = 0.0; //-1.9832f;
	double taillevel = 0.0;

	double ga,gb,gt;
	unsigned int i;
	int n;
	double r;
	double diffscale;
	int a,b,c,cc,d,dd,e;
	double spread1,spread2;

	if(argc >= 1)
	{
		// get maxroomsize
        switch(atom_gettype(argv)) {
            case A_FLOAT:
                maxroomsize = CLAMP(atom_getlong(argv), 0.1, 10000.);
                break;
            case A_LONG:
                maxroomsize = CLAMP(atom_getfloat(argv), 0.1, 10000.);
                break;
            default:
                object_post((t_object *)p, "expected number for param 1 (maxroomsize)");
                break;
        }
		
	}
	else if(argc >= 2)
	{
		// get spreading
        switch(atom_gettype(argv+1)) {
            case A_FLOAT:
                spread = CLAMP( atom_getlong(argv+1), 0., 100.);
                break;
            case A_LONG:
                spread = CLAMP( atom_getfloat(argv+1), 0., 100.);
                break;
            default:
                object_post((t_object *)p, "expected number for param 2 (spreading)");
                break;
        }
        
	}


    // zero out the struct, to be careful
    if(p)
    {
    	for(i = sizeof(t_pxobject); i < sizeof(ty_gverb); i++)
    		((char*)p)[i] = 0;
    }
    else {
        object_free(p);
        p = NULL;
    }

	dsp_setup((t_pxobject *)p, 1);
	outlet_new((t_pxobject *)p, "signal");
	outlet_new((t_pxobject *)p, "signal");

	p->rate = (int)sys_getsr();
	p->fdndamping = damping;
	p->maxroomsize = maxroomsize;
	p->roomsize = CLAMP(roomsize, 0.1, maxroomsize);
	p->revtime = revtime;
	p->drylevel = drylevel;
	p->earlylevel = earlylevel;
	p->taillevel = taillevel;

	p->maxdelay = p->rate*p->maxroomsize/340.0;
	p->largestdelay = p->rate*p->roomsize/340.0;
    

	if(p->maxroomsize != 300.0)
		object_post((t_object *)p,"maximum roomsize: %f", p->maxroomsize);

	/* Input damper */

	p->inputbandwidth = inputbandwidth;
	p->inputdamper = damper_make(1.0 - p->inputbandwidth);


    
	/* FDN section */
    
	p->fdndels = (ty_fixeddelay **)sysmem_newptr(FDNORDER*sizeof(ty_fixeddelay *));
	if(!p->fdndels)
	{
		error("gigaverb~: out of memory");
		return (NULL);
	}
	for(i = 0; i < FDNORDER; i++)
	{
		p->fdndels[i] = fixeddelay_make((int)p->maxdelay+1000);
		if(!p->fdndels[i])
		{
			error("gigaverb~: out of memory");
			return (NULL);
		}
	}
    p->fdngains = (double *)sysmem_newptr(FDNORDER*sizeof(double));
	p->fdnlens = (int *)sysmem_newptr(FDNORDER*sizeof(int));
	if(!p->fdngains || !p->fdnlens)
	{
		error("gigaverb~: out of memory");
		return (NULL);
	}

	p->fdndamps = (ty_damper **)sysmem_newptr(FDNORDER*sizeof(ty_damper *));
	if(!p->fdndamps)
	{
		error("gigaverb~: out of memory");
		return (NULL);
	}
	for(i = 0; i < FDNORDER; i++)
	{
		p->fdndamps[i] = damper_make(p->fdndamping);
		if(!p->fdndamps[i])
		{
			error("gigaverb~: out of memory");
			return (NULL);
		}
	}

	ga = 60.0;
	gt = p->revtime;
	ga = pow(10.0,-ga/20.0);
	n = (int)(p->rate*gt);
	p->alpha = pow(ga,1.0/(double)n);


	gb = 0.0;
	for(i = 0; i < FDNORDER; i++)
	{
		if (i == 0) gb = 1.000000*p->largestdelay;
		if (i == 1) gb = 0.816490*p->largestdelay;
		if (i == 2) gb = 0.707100*p->largestdelay;
		if (i == 3) gb = 0.632450*p->largestdelay;

#if 0
		p->fdnlens[i] = nearest_prime((int)gb, 0.5);
#else
		p->fdnlens[i] = (int)gb;
#endif
        
		p->fdngains[i] = -pow(p->alpha,p->fdnlens[i]);
	}

	p->d = (double *)sysmem_newptr(FDNORDER*sizeof(double));
	p->u = (double *)sysmem_newptr(FDNORDER*sizeof(double));
	p->f = (double *)sysmem_newptr(FDNORDER*sizeof(double));
	if(!p->d || !p->u || !p->f)
	{
		error("gigaverb~: out of memory");
		return (NULL);
	}



	/* Diffuser section */


	diffscale = p->fdnlens[3]/(210+159+562+410);
    
	spread1 = spread;
	spread2 = 3.0*spread;

	b = 210;
	r = 0.125541;
	a = (int)(spread1*r);
	c = 210+159+a;
	cc = c-b;
	r = 0.854046;
	a = (int)(spread2*r);
	d = 210+159+562+a;
	dd = d-c;
	e = 1341-d;

	p->ldifs = (ty_diffuser **)sysmem_newptr(4*sizeof(ty_diffuser *));
	if(!p->ldifs)
	{
		error("gigaverb~: out of memory");
		return (NULL);
	}
	p->ldifs[0] = diffuser_make((int)(diffscale*b),0.75);
	p->ldifs[1] = diffuser_make((int)(diffscale*cc),0.75);
	p->ldifs[2] = diffuser_make((int)(diffscale*dd),0.625);
	p->ldifs[3] = diffuser_make((int)(diffscale*e),0.625);
	if(!p->ldifs[0] || !p->ldifs[1] || !p->ldifs[2] || !p->ldifs[3])
	{
		error("gigaverb~: out of memory");
		return (NULL);
	}

	b = 210;
	r = -0.568366;
	a = (int)(spread1*r);
	c = 210+159+a;
	cc = c-b;
	r = -0.126815;
	a = (int)(spread2*r);
	d = 210+159+562+a;
	dd = d-c;
	e = 1341-d;

	p->rdifs = (ty_diffuser **)sysmem_newptr(4*sizeof(ty_diffuser *));
	if(!p->rdifs)
	{
		error("gigaverb~: out of memory");
		return (NULL);
	}
	p->rdifs[0] = diffuser_make((int)(diffscale*b),0.75);
	p->rdifs[1] = diffuser_make((int)(diffscale*cc),0.75);
	p->rdifs[2] = diffuser_make((int)(diffscale*dd),0.625);
	p->rdifs[3] = diffuser_make((int)(diffscale*e),0.625);
	if(!p->rdifs[0] || !p->rdifs[1] || !p->rdifs[2] || !p->rdifs[3])
	{
		error("gigaverb~: out of memory");
		return (NULL);
	}


	/* Tapped delay section */

	p->tapdelay = fixeddelay_make(44000);
	p->taps = (int *)sysmem_newptr(FDNORDER*sizeof(int));
	p->tapgains = (double *)sysmem_newptr(FDNORDER*sizeof(double));
	if(!p->tapdelay || !p->taps || !p->tapgains)
	{
		error("gigaverb~: out of memory");
		return (NULL);
	}

	p->taps[0] = (int)(5+0.410*p->largestdelay);
	p->taps[1] = (int)(5+0.300*p->largestdelay);
	p->taps[2] = (int)(5+0.155*p->largestdelay);
	p->taps[3] = (int)(5+0.000*p->largestdelay);

	for(i = 0; i < FDNORDER; i++)
	{
		p->tapgains[i] = pow(p->alpha,(double)p->taps[i]);
	}

	return(p);
}

void gverb_free(ty_gverb *p)
{
	int i;
	
	dsp_free((t_pxobject *)p);	// this one must be first!

	damper_free(p->inputdamper);
	for(i = 0; i < FDNORDER; i++)
	{
		fixeddelay_free(p->fdndels[i]);
		damper_free(p->fdndamps[i]);
		diffuser_free(p->ldifs[i]);
		diffuser_free(p->rdifs[i]);
	}
	sysmem_freeptr(p->fdndels);
	sysmem_freeptr(p->fdngains);
	sysmem_freeptr(p->fdnlens);
	sysmem_freeptr(p->fdndamps);
	sysmem_freeptr(p->d);
	sysmem_freeptr(p->u);
	sysmem_freeptr(p->f);
	sysmem_freeptr(p->ldifs);
	sysmem_freeptr(p->rdifs);
	sysmem_freeptr(p->taps);
	sysmem_freeptr(p->tapgains);
    
    /*
     t_freebytes(p->fdndels, FDNORDER*sizeof(ty_fixeddelay *));
     t_freebytes(p->fdngains, FDNORDER*sizeof(double));
     t_freebytes(p->fdnlens, FDNORDER*sizeof(int));
     t_freebytes(p->fdndamps, FDNORDER*sizeof(ty_damper *));
     t_freebytes(p->d, FDNORDER*sizeof(double));
     t_freebytes(p->u, FDNORDER*sizeof(double));
     t_freebytes(p->f, FDNORDER*sizeof(double));
     t_freebytes(p->ldifs, 4*sizeof(ty_diffuser *));
     t_freebytes(p->rdifs, 4*sizeof(ty_diffuser *));
     t_freebytes(p->taps, FDNORDER*sizeof(int));
     t_freebytes(p->tapgains, FDNORDER*sizeof(double));
     */
	fixeddelay_free(p->tapdelay);
}

t_int *gverb_perform(t_int *w)
{
    t_float *in = (t_float *)(w[1]);
    t_float *out1 = (t_float *)(w[2]);
    t_float *out2 = (t_float *)(w[3]);
	ty_gverb *p = (ty_gverb *)(w[4]);
	int n = (int)(w[5]);
	t_double outL, outR, input;
	float dry = p->drylevel;
		
	if (p->p_obj.z_disabled)
		goto bail;
		
	if(p->bypass)
	{
		// Bypass, so just copy input to output
		while(n--)
		{
			input = *in++;
			*out1++ = input;
			*out2++ = input;
		}
	}
	else
	{
    	// DSP loop
		while(n--)
		{
			input = *in++;
			gverb_do(p, input, &outL, &outR);
			*out1++ = outL + input * dry;
			*out2++ = outR + input * dry;
		}
	}
bail:
    return (w+6);
}

void gverb_dsp(ty_gverb *p, t_signal **sp)
{
	if (p->rate != sp[0]->s_sr)
	{
		p->rate = (int)sp[0]->s_sr;
	}
	
	dsp_add(gverb_perform, 5, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, p, sp[0]->s_n);
}




void gverb_perform64(ty_gverb *p, t_object *dsp64, double **ins, long numins,
            double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    
    t_double *in = ins[0];
    t_double *out1 = outs[0];
    t_double *out2 = outs[1];
    
    long n = sampleframes;
    t_double outL, outR, input;
    t_double dry = p->drylevel;
    
    if (p->p_obj.z_disabled)
        return;
    
    if(p->bypass)
    {
        // Bypass, so just copy input to output
        while(n--)
        {
            input = *in++;
            *out1++ = input;
            *out2++ = input;
        }
    }
    else
    {
        // DSP loop
        while(n--)
        {
            input = *in++;
            gverb_do(p, input, &outL, &outR);
            *out1++ = outL + input * dry;
            *out2++ = outR + input * dry;
        }
    }
    
}


//64-bit dsp method
void gverb_dsp64(ty_gverb *p, t_object *dsp64, short *count, double samplerate,
                 long maxvectorsize, long flags) {
    object_method(dsp64, gensym("dsp_add64"), p, gverb_perform64, 0, NULL);
    
    p->rate = samplerate;
    if(p->rate<=0) p->rate = 44100;
    
}



void gverb_set_bypass(ty_gverb *p, long a)
{
    if(a != 0) p->bypass = 1;
    else p->bypass = 0;
	//p->bypass = CLAMP(a, 0, 1);
}

void gverb_flush(ty_gverb *p)
{
	int i;

	damper_flush(p->inputdamper);
	for(i = 0; i < FDNORDER; i++)
	{
		fixeddelay_flush(p->fdndels[i]);
		damper_flush(p->fdndamps[i]);
		diffuser_flush(p->ldifs[i]);
		diffuser_flush(p->rdifs[i]);
	}
	memset(p->d, 0, FDNORDER * sizeof(double));
	memset(p->u, 0, FDNORDER * sizeof(double));
	memset(p->f, 0, FDNORDER * sizeof(double));
	fixeddelay_flush(p->tapdelay);
}

/*	clear the delay lines and other stuff */
void gverb_clear(ty_gverb *p)
{
	int i, k;

	for(i = 0; i < FDNORDER; i++)
	{
		for(k = 0; k < p->fdnlens[i]; k++)
			fixeddelay_write(p->fdndels[i], 0);
	}
}


/*	print internal values */
void gverb_print(ty_gverb *p)
{
	object_post((t_object *)p, "gigaverb~ 1.0:");
	post("    roomsize: %0.0f meters (%0.0f maximum)", p->roomsize, p->maxroomsize);
	post("    reverbtime: %0.02f seconds", p->revtime);
	post("    damping: %0.02f", p->fdndamping);
	post("    input bandwidth: %02.02f", p->inputbandwidth);
	post("    dry signal level: %02.02f dB", CO_DB(p->drylevel));
	post("    early reflection level: %02.02f dB", CO_DB(p->earlylevel));
	post("    reverb tail level: %02.02f dB", CO_DB(p->taillevel));
}

void gverb_assist(ty_gverb *p, void *b, long m, long a, char *s)
{
	if (m==1)	// inlet
	{
		sprintf (s,"(signal) input channel 1, commands...");
	} else if (m==2)	// outlets
	{
		if (a==0) sprintf (s,"(signal) output channel 1");
		if (a==1) sprintf (s,"(signal) output channel 2");
	}
}



int C74_EXPORT main(void)
{
    t_class *c;
    
    c = class_new("gigaverb~", (method)gverb_new, (method)gverb_free, (short)sizeof(ty_gverb),
                  0L, A_GIMME, 0L);
    class_addmethod(c, (method)gverb_dsp, "dsp", A_CANT, 0);
    class_addmethod(c, (method)gverb_dsp64, "dsp64", A_CANT, 0);
    
    class_addmethod(c, (method)gverb_set_roomsize, "roomsize", A_FLOAT, 0);
    class_addmethod(c, (method)gverb_set_revtime, "revtime", A_FLOAT, 0);
    class_addmethod(c, (method)gverb_set_damping, "damping", A_FLOAT, 0);
    class_addmethod(c, (method)gverb_set_inputbandwidth, "bandwidth", A_FLOAT, 0);
    class_addmethod(c, (method)gverb_set_drylevel, "dry", A_FLOAT, 0);
    class_addmethod(c, (method)gverb_set_earlylevel, "early", A_FLOAT, 0);
    class_addmethod(c, (method)gverb_set_taillevel, "tail", A_FLOAT, 0);
    class_addmethod(c, (method)gverb_set_bypass, "bypass", A_LONG, 0);
    class_addmethod(c, (method)gverb_flush, "clear", 0);
    class_addmethod(c, (method)gverb_print, "print", 0);
    class_addmethod(c, (method)gverb_assist, "assist", A_CANT,0);
    
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    gverb_class = c;
    
    
    post(version);
    
    
    return 0;
}
