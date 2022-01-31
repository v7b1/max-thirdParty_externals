/*****************************************************************************/
/*                                                                           */
/* Module:  TIA Chip Sound Simulator                                         */
/* Purpose: To emulate the sound generation hardware of the Atari TIA chip.  */
/* Author:  Ron Fries                                                        */
/*                                                                           */
/* Revision History:                                                         */
/*    10-Sep-96 - V1.0 - Initial Release                                     */
/*    14-Jan-97 - V1.1 - Cleaned up sound output by eliminating counter      */
/*                       reset.                                              */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                 License Information and Copyright Notice                  */
/*                 ========================================                  */
/*                                                                           */
/* TiaSound is Copyright(c) 1996 by Ron Fries                                */
/*                                                                           */
/* This library is free software; you can redistribute it and/or modify it   */
/* under the terms of version 2 of the GNU Library General Public License    */
/* as published by the Free Software Foundation.                             */
/*                                                                           */
/* This library is distributed in the hope that it will be useful, but       */
/* WITHOUT ANY WARRANTY; without even the implied warranty of                */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library */
/* General Public License for more details.                                  */
/* To obtain a copy of the GNU Library General Public License, write to the  */
/* Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.   */
/*                                                                           */
/* Any permitted reproduction of these routines, in whole or in part, must   */
/* bear this legend.                                                         */
/*                                                                           */
/*****************************************************************************/

// ported for maxmsp by volker boehm, august 2018


#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"

static t_class *atari2600_class;  


/* define some data types to keep it platform independent */

#ifdef WIN32
#define int8  char
#define int16 short
#define int32 int
#else
#define int8  char
#define int16 int
#define int32 long
#endif


#define uint8  unsigned int8
#define uint16 unsigned int16
#define uint32 unsigned int32


// definitions for AUDCx (15, 16)
#define SET_TO_1     0x00      /* 0000 */
#define POLY4        0x01      /* 0001 */
#define DIV31_POLY4  0x02      /* 0010 */
#define POLY5_POLY4  0x03      /* 0011 */
#define PURE         0x04      /* 0100 */
#define PURE2        0x05      /* 0101 */
#define DIV31_PURE   0x06      /* 0110 */
#define POLY5_2      0x07      /* 0111 */
#define POLY9        0x08      /* 1000 */
#define POLY5        0x09      /* 1001 */
#define DIV31_POLY5  0x0a      /* 1010 */
#define POLY5_POLY5  0x0b      /* 1011 */
#define DIV3_PURE    0x0c      /* 1100 */
#define DIV3_PURE2   0x0d      /* 1101 */
#define DIV93_PURE   0x0e      /* 1110 */
#define DIV3_POLY5   0x0f      /* 1111 */

#define DIV3_MASK    0x0c                 

#define AUDC0        0x15
#define AUDC1        0x16
#define AUDF0        0x17
#define AUDF1        0x18
#define AUDV0        0x19
#define AUDV1        0x1a

/* the size (in entries) of the 4 polynomial tables */
#define POLY4_SIZE  0x000f
#define POLY5_SIZE  0x001f
#define POLY9_SIZE  0x01ff

/* channel definitions */
#define CHAN1       0
#define CHAN2       1


/* Initialze the bit patterns for the polynomials. */

/* The 4bit and 5bit patterns are the identical ones used in the tia chip. */
/* Though the patterns could be packed with 8 bits per byte, using only a */
/* single bit per byte keeps the math simple, which is important for */
/* efficient processing. */

static uint8 Bit4[POLY4_SIZE] =
      { 1,1,0,1,1,1,0,0,0,0,1,0,1,0,0 };

static uint8 Bit5[POLY5_SIZE] =
      { 0,0,1,0,1,1,0,0,1,1,1,1,1,0,0,0,1,1,0,1,1,1,0,1,0,1,0,0,0,0,1 };

/* I've treated the 'Div by 31' counter as another polynomial because of */
/* the way it operates.  It does not have a 50% duty cycle, but instead */
/* has a 13:18 ratio (of course, 13+18 = 31).  This could also be */
/* implemented by using counters. */

static uint8 Div31[POLY5_SIZE] =
      { 0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0 };

typedef struct _atari2600
{ 
    t_pxobject x_obj;

    t_uint8   Bit9[POLY9_SIZE];
    t_uint8   P4[2];         /* Position pointer for the 4-bit POLY array */
    t_uint8   P5[2];         /* Position pointer for the 5-bit POLY array */
    t_uint16  P9[2];         /* Position pointer for the 9-bit POLY array */
    t_uint8   Div_n_cnt[2];  /* Divide by n counter. one for each channel */
    t_uint8   Div_n_max[2];  /* Divide by n maximum, one for each channel */
    t_uint8   AUDC[2];       /* AUDCx (15, 16) */
    t_uint8   AUDF[2];       /* AUDFx (17, 18) */
    t_uint8   AUDV[2];       /* AUDVx (19, 1A) */
    t_uint8   Outvol[2];     /* output volume for each channel */
    long      volume;
    
    t_uint16  Samp_n_max;     /* Sample max, multiplied by 256 */
    t_uint16  Samp_n_cnt;     /* Sample cnt. */
    
    double      playback_rate;
    double      sr;             /* system sample rate */

} t_atari2600;


void atari2600_tone1(t_atari2600 *x, long input);
void atari2600_freq1(t_atari2600 *x, long input);
void atari2600_vol1(t_atari2600 *x, long input);
void atari2600_tone2(t_atari2600 *x, long input);
void atari2600_freq2(t_atari2600 *x, long input);
void atari2600_vol2(t_atari2600 *x, long input);
void atari2600_playback_rate(t_atari2600 *x, double input);

void *atari2600_new( t_symbol *s, long argc, t_atom *argv );
void atari2600_assist(t_atari2600 *x, void *b, long m, long a, char *s);

void atari2600_dsp64(t_atari2600 *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void atari2600_perform64(t_atari2600 *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

void Tia_sound_init(t_atari2600 *x, t_uint16 sample_freq, t_uint16 playback_freq);
void Update_tia_sound(t_atari2600 *x, t_uint16 addr, t_uint8 val);
void Tia_process (t_atari2600 *x, t_double *buffer, t_uint16 n);



void ext_main(void *r) {
    t_class *c;
	c = class_new("atari2600~", (method)atari2600_new, (method)dsp_free, (long)sizeof(t_atari2600), 0L, A_DEFFLOAT, 0L);
	class_addmethod(c, (method)atari2600_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)atari2600_tone1, "int", A_LONG, 0);
    class_addmethod(c, (method)atari2600_freq1, "in1", A_LONG, 0);
    class_addmethod(c, (method)atari2600_vol1, "in2", A_LONG, 0);
    class_addmethod(c, (method)atari2600_tone2, "in3", A_LONG, 0);
    class_addmethod(c, (method)atari2600_freq2, "in4", A_LONG, 0);
    class_addmethod(c, (method)atari2600_vol2, "in5", A_LONG, 0);
    class_addmethod(c, (method)atari2600_playback_rate, "ft6", A_FLOAT, 0);
    class_addmethod(c, (method)atari2600_assist, "assist", A_CANT, 0);
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    atari2600_class = c;
    
    object_post(NULL, "atari2600~ (back in the game, vb!)");
}


void *atari2600_new( t_symbol *s, long argc, t_atom *argv )
{  
    t_atari2600 *x = object_alloc(atari2600_class);
    
    if(x) {

        floatin(x, 6);
        intin(x, 5);
        intin(x, 4);
        intin(x, 3);
        intin(x, 2);
        intin(x, 1);
        
        outlet_new(x, "signal");
        
        x->sr = sys_getsr();
        if(x->sr <= 0) x->sr = 44100;
        
        x->playback_rate = 1.0;

        
        Tia_sound_init(x, 32000, 32000);    // sample rate , playback rate
    }
    else {
        object_free(x);
        x = NULL;
    }
    
    return (x);
}



void atari2600_tone1(t_atari2600 *x, long input) {
    Update_tia_sound(x, 0x15, input);
}

void atari2600_freq1(t_atari2600 *x, long input) {
    Update_tia_sound(x, 0x17, input);
}

void atari2600_vol1(t_atari2600 *x, long input) {
    Update_tia_sound(x, 0x19, input);
}


void atari2600_tone2(t_atari2600 *x, long input) {
    Update_tia_sound(x, 0x16, input);
}

void atari2600_freq2(t_atari2600 *x, long input) {
    Update_tia_sound(x, 0x18, input);
}

void atari2600_vol2(t_atari2600 *x, long input) {
    Update_tia_sound(x, 0x1a, input);
}

void atari2600_playback_rate(t_atari2600 *x, double input) {
    x->playback_rate = input;
}


//64-bit dsp method
void atari2600_dsp64(t_atari2600 *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags) {
    
    object_method(dsp64, gensym("dsp_add64"), x, atari2600_perform64, 0, NULL);
    
    x->sr = samplerate;
    if(x->sr<=0) x->sr = 44100.0;
    
}


// 64 bit signal input version
void atari2600_perform64(t_atari2600 *x, t_object *dsp64, double **ins, long numins,
                     double **outs, long numouts, long sampleframes, long flags, void *userparam){
    
    t_double *out = outs[0];
    long vs = sampleframes;
    
    
    if (x->x_obj.z_disabled)
        return;
    
    x->Samp_n_max = (t_uint16)(((t_uint32)x->sr<<8)/x->sr) * x->playback_rate;

    Tia_process(x, out, (int)vs);
    
}

void atari2600_assist(t_atari2600 *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET) {
        switch(a) {
            case 0: sprintf (s,"(int) tone control (0..15) channel 1"); break;
            case 1: sprintf (s,"(int) freq control (0..31) channel 1"); break;
            case 2: sprintf (s,"(int) volume control (0..15) channel 1"); break;
            case 3: sprintf (s,"(int) tone control (0..15) channel 2"); break;
            case 4: sprintf (s,"(int) freq control (0..31) channel 2"); break;
            case 5: sprintf (s,"(int) volume control (0..15) channel 2"); break;
            case 6: sprintf (s,"(float) playback rate"); break;
        }
    }
    else {
        switch(a) {
            case 0: sprintf (s,"(signal) mixed audio out of channel 1 and 2"); break;
        }
    }
}



/*****************************************************************************/
/* Module:  Tia_sound_init()                                                 */
/* Purpose: to handle the power-up initialization functions                  */
/*          these functions should only be executed on a cold-restart        */
/*                                                                           */
/* Author:  Ron Fries                                                        */
/* Date:    September 10, 1996                                               */
/*                                                                           */
/* Inputs:  sample_freq - the value for the '30 Khz' Tia audio clock         */
/*          playback_freq - the playback frequency in samples per second     */
/*                                                                           */
/* Outputs: Adjusts local globals - no return value                          */
/*                                                                           */
/*****************************************************************************/

void Tia_sound_init (t_atari2600 *x, t_uint16 sample_freq, t_uint16 playback_freq)
{
   t_uint8 chan;
   t_int16 n;

   // fill the 9bit polynomial with random bits
   for (n=0; n<POLY9_SIZE; n++)
   {
      x->Bit9[n] = rand() & 0x01;       // fill poly9 with random bits
   }
    
    /* calculate the sample 'divide by N' value based on the playback freq. */
    x->Samp_n_max = (uint16)(((uint32)sample_freq<<8)/playback_freq);
    x->Samp_n_cnt = 0;  /* initialize all bits of the sample counter */

   // initialize the local globals
   for (chan = CHAN1; chan <= CHAN2; chan++)
   {
      x->Outvol[chan] = 0;
      x->Div_n_cnt[chan] = 0;
      x->Div_n_max[chan] = 0;
      x->AUDC[chan] = 0;
      x->AUDF[chan] = 0;
      x->AUDV[chan] = 0;
      x->P4[chan] = 0;
      x->P5[chan] = 0;
      x->P9[chan] = 0;
   }

}


/*****************************************************************************/
/* Module:  Update_tia_sound()                                               */
/* Purpose: To process the latest control values stored in the AUDF, AUDC,   */
/*          and AUDV registers.  It pre-calculates as much information as    */
/*          possible for better performance.  This routine has not been      */
/*          optimized.                                                       */
/*                                                                           */
/* Author:  Ron Fries                                                        */
/* Date:    January 14, 1997                                                 */
/*                                                                           */
/* Inputs:  addr - the address of the parameter to be changed                */
/*          val - the new value to be placed in the specified address        */
/*                                                                           */
/* Outputs: Adjusts local globals - no return value                          */
/*                                                                           */
/*****************************************************************************/

void Update_tia_sound(t_atari2600 *x, t_uint16 addr, t_uint8 val)
{
    t_uint16 new_val = 0;
    t_uint8 chan;

    // determine which address was changed
    switch (addr)
    {
       case AUDC0:
          x->AUDC[0] = val & 0x0f;
          chan = 0;
          break;

       case AUDC1:
          x->AUDC[1] = val & 0x0f;
          chan = 1;
          break;

       case AUDF0:
          x->AUDF[0] = val & 0x1f;
          chan = 0;
          break;

       case AUDF1:
          x->AUDF[1] = val & 0x1f;
          chan = 1;
          break;

       case AUDV0:
          x->AUDV[0] = (val & 0x0f) << 3;
          chan = 0;
          break;

       case AUDV1:
          x->AUDV[1] = (val & 0x0f) << 3;
          chan = 1;
          break;

       default:
          chan = 255;
          break;
    }

    /* if the output value changed */
    if (chan != 255)
    {
       /* an AUDC value of 0 is a special case */
       if (x->AUDC[chan] == SET_TO_1)
       {
          /* indicate the clock is zero so no processing will occur */
          new_val = 0;

          /* and set the output to the selected volume */
          x->Outvol[chan] = x->AUDV[chan];
       }
       else
       {
          /* otherwise calculate the 'divide by N' value */
          new_val = x->AUDF[chan] + 1;

          /* if bits 2 & 3 are set, then multiply the 'div by n' count by 3 */
          if ((x->AUDC[chan] & DIV3_MASK) == DIV3_MASK)
          {
             new_val *= 3;
          }
       }

       /* only reset those channels that have changed */
       if (new_val != x->Div_n_max[chan])
       {
          /* reset the divide by n counters */
          x->Div_n_max[chan] = new_val;

          /* if the channel is now volume only or was volume only */
          if ((x->Div_n_cnt[chan] == 0) || (new_val == 0))
          {
             /* reset the counter (otherwise let it complete the previous) */
             x->Div_n_cnt[chan] = new_val;
          }
       }
    }
	
}



/*****************************************************************************/
/* Module:  Tia_process()                                                    */
/* Purpose: To fill the output buffer with the sound output based on the     */
/*          tia chip parameters.  This routine has been optimized.           */
/*                                                                           */
/* Author:  Ron Fries                                                        */
/* Date:    September 10, 1996                                               */
/*                                                                           */
/* Inputs:  *buffer - pointer to the buffer where the audio output will      */
/*                    be placed                                              */
/*          n - size of the playback buffer                                  */
/*                                                                           */
/* Outputs: the buffer will be filled with n bytes of audio - no return val  */
/*                                                                           */
/*****************************************************************************/

void Tia_process(t_atari2600 *x, t_double *buffer, t_uint16 n)
{
    t_uint16 Samp_n_max = x->Samp_n_max;
    t_uint16 Samp_n_cnt = x->Samp_n_cnt;
    

    t_uint8 audc0, audv0, audc1, audv1;
    t_uint8 div_n_cnt0, div_n_cnt1;
    t_uint8 p5_0, p5_1, outvol_0, outvol_1;

    audc0 = x->AUDC[0];
    audv0 = x->AUDV[0];
    audc1 = x->AUDC[1];
    audv1 = x->AUDV[1];

    // make temporary local copy
    p5_0 = x->P5[0];
    p5_1 = x->P5[1];
    outvol_0 = x->Outvol[0];
    outvol_1 = x->Outvol[1];
    div_n_cnt0 = x->Div_n_cnt[0];
    div_n_cnt1 = x->Div_n_cnt[1];

    // loop until the buffer is filled
    while (n)
    {
       // Process channel 0
       if (div_n_cnt0 > 1)
       {
          div_n_cnt0--;
       }
       else if (div_n_cnt0 == 1)
       {
          div_n_cnt0 = x->Div_n_max[0];

          /* the P5 counter has multiple uses, so we inc it here */
          p5_0++;
          if (p5_0 == POLY5_SIZE)
             p5_0 = 0;

          /* check clock modifier for clock tick */
          if  (((audc0 & 0x02) == 0) ||
              (((audc0 & 0x01) == 0) && Div31[p5_0]) ||
              (((audc0 & 0x01) == 1) &&  Bit5[p5_0]))
          {
		  
             if (audc0 & 0x04)       /* pure modified clock selected */
             {
                if (outvol_0)        /* if the output was set */
                   outvol_0 = 0;     /* turn it off */
                else {
                   outvol_0 = audv0; /* else turn it on */
				}
             }
             else if (audc0 & 0x08)    /* check for p5/p9 */
             {
                if (audc0 == POLY9)    /* check for poly9 */
                {
                   /* inc the poly9 counter */
                   x->P9[0]++;
                   if (x->P9[0] == POLY9_SIZE)
                      x->P9[0] = 0;

                   if (x->Bit9[x->P9[0]]) {
                      outvol_0 = audv0;
					}
                   else
                      outvol_0 = 0;
                }
                else                        /* must be poly5 */
                {
                   if (Bit5[p5_0]) {
                      outvol_0 = audv0;
					  }
                   else
                      outvol_0 = 0;
                }
             }
             else  /* poly4 is the only remaining option */
             {
                /* inc the poly4 counter */
                x->P4[0]++;
                if (x->P4[0] == POLY4_SIZE)
                   x->P4[0] = 0;

                if (Bit4[x->P4[0]]) {
                   outvol_0 = audv0;
				   }
                else
                   outvol_0 = 0;
             }
          }
       }

       /* Process channel 1 */
       if (div_n_cnt1 > 1)
       {
          div_n_cnt1--;
       }
       else if (div_n_cnt1 == 1)
       {
          div_n_cnt1 = x->Div_n_max[1];

          /* the P5 counter has multiple uses, so we inc it here */
          p5_1++;
          if (p5_1 == POLY5_SIZE)
             p5_1 = 0;

          /* check clock modifier for clock tick */
          if  (((audc1 & 0x02) == 0) ||
              (((audc1 & 0x01) == 0) && Div31[p5_1]) ||
              (((audc1 & 0x01) == 1) &&  Bit5[p5_1]))
          {
             if (audc1 & 0x04)       /* pure modified clock selected */
             {
                if (outvol_1)        /* if the output was set */
                   outvol_1 = 0;     /* turn it off */
                else
                   outvol_1 = audv1; /* else turn it on */
             }
             else if (audc1 & 0x08)    /* check for p5/p9 */
             {
                if (audc1 == POLY9)    /* check for poly9 */
                {
                   /* inc the poly9 counter */
                   x->P9[1]++;
                   if (x->P9[1] == POLY9_SIZE)
                      x->P9[1] = 0;

                   if (x->Bit9[x->P9[1]])
                      outvol_1 = audv1;
                   else
                      outvol_1 = 0;
                }
                else                        /* must be poly5 */
                {
                   if (Bit5[p5_1])
                      outvol_1 = audv1;
                   else
                      outvol_1 = 0;
                }
             }
             else  /* poly4 is the only remaining option */
             {
                /* inc the poly4 counter */
                x->P4[1]++;
                if (x->P4[1] == POLY4_SIZE)
                   x->P4[1] = 0;

                if (Bit4[x->P4[1]])
                   outvol_1 = audv1;
                else
                   outvol_1 = 0;
             }
          }
       }
        
        /* decrement the sample counter - value is 256 since the lower
         byte contains the fractional part */
        
        Samp_n_cnt -= 256;
        
        // if the count down has reached zero
        if (Samp_n_cnt < 256)
        {
            // adjust the sample counter
            Samp_n_cnt += Samp_n_max;
            
            // calculate the latest output value and place in buffer
            *(buffer++) = ((outvol_0 + outvol_1)*0.00390625);
            
            // and indicate one less byte to process
            n--;
        }
        
        
       /*
        uint32 val = ((((uint32)outvol_0 + (uint32)outvol_1)));  // * x->volume)/100);
        float va = (((float)val) - 100.0)/100.0;
        *(buffer++) = va;
        n--;
       */
    }

    /* save for next round */
    x->P5[0] = p5_0;
    x->P5[1] = p5_1;
    x->Outvol[0] = outvol_0;
    x->Outvol[1] = outvol_1;
    x->Div_n_cnt[0] = div_n_cnt0;
    x->Div_n_cnt[1] = div_n_cnt1;
    //x->Samp_n_max = Samp_n_max;
    x->Samp_n_cnt = Samp_n_cnt;
}
