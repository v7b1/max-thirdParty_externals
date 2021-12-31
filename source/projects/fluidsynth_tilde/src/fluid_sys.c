/***************************************************************************************
 *
 *  fluidsynth~
 *
 *  Fluid Synth soundfont synthesizer for Max/MSP.
 *
 *  Fluid Synth is written by Peter Hanappe et al.
 *  see http://www.fluidsynth.org/
 *
 *  Max/MSP integration by Norbert Schnell ATR IRCAM - Centre Pompidou
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *  
 *  See file COPYING.LIB for further informations on licensing terms.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */
/*
 *  This file contains the (mostly empty) implementation of some functions without 
 *  which Fluidsynth wouldn't compile.
 *
 *  The Max/MSP version of FluidSynth tries to link only with a minimum of needed 
 *  files. Some of the linked files depend on functions that are not necessary
 *  and that are implemnted in files we don't want to include. So, alternate or empty
 *  implemntations of these functions are provided here.
 * 
 */
#include "ext.h"
#include "fluidsynth.h"
#include "fluidsynth_priv.h"
#include "fluid_settings.h"

typedef void fluid_timer_t;
typedef int(*fluid_timer_callback_t)(void* data, unsigned int msec);

fluid_timer_t * 
new_fluid_timer(int msec, fluid_timer_callback_t callback, void* data, int new_thread, int auto_destroy)
{
  /* just call it right away */
  (*callback)(data, msec);
  
  return NULL;
}

/* This code is (unelegantly) copied from fluid_sys.c since other parts of fluid_sys.c 
 * that we don't need here depend on many other things that we don't want here. */
char *
fluid_strtok (char **str, char *delim)
{
  char *s, *d, *token;
  char c;
  
  if (str == NULL || delim == NULL || !*delim)
  {
    FLUID_LOG(FLUID_ERR, "Null pointer");
    return NULL;
  }
  
  s = *str;
  if (!s) return NULL;	/* str points to a NULL pointer? (tokenize already ended) */
  
  /* skip delimiter chars at beginning of token */
  do
  {
    c = *s;
    if (!c)	/* end of source string? */
    {
      *str = NULL;
      return NULL;
    }
    
    for (d = delim; *d; d++)	/* is source char a token char? */
    {
      if (c == *d)	/* token char match? */
      {
        s++;		/* advance to next source char */
        break;
      }
    }
  } while (*d);		/* while token char match */
  
  token = s;		/* start of token found */
  
  /* search for next token char or end of source string */
  for (s = s+1; *s; s++)
  {
    c = *s;
    
    for (d = delim; *d; d++)	/* is source char a token char? */
    {
      if (c == *d)	/* token char match? */
      {
        *s = '\0';	/* overwrite token char with zero byte to terminate token */
        *str = s+1;	/* update str to point to beginning of next token */
        return token;
      }
    }
  }
  
  /* we get here only if source string ended */
  *str = NULL;
  return token;
}

int 
fluid_log(int level, char* fmt, ...)
{
  char buf[1024];
  
  va_list args; 
  va_start (args, fmt); 
  vsprintf(buf, fmt, args); 
  va_end (args); 

  if ((level > 0) && (level < LAST_LOG_LEVEL)) 
    post("fluidsynth~ core (level %d): %s", level, buf);

  return -1; 
}
