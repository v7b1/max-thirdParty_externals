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
 
/************************************************************************
 *
 *  Max integration by Norbert.Schnell@ircam.fr
 *
 *  versions:
 *   (20): fixed humming when starting DSP and crash on "info soundfont" message (08/2015)
 *   (18): adapted to Max 6.1 32/64-bit API (12/2013)
 *   (17): added "generator" message, adjusted to Max 5 API (03/2013)
 *   (16): no changes (11/2012)
 *   (15): fixed 'tuning-octave' message
 *   (14): adapted to latest code base
 *   (13): adapted to current FluidSynth code base (after years without activity)
 *   (12): fixed voice stealing
 *   (11): fixed arguments of fluidmax_tuning_octave() (third has to be float)
 *   (10): added micro-tuning methods
 *    (9): bug fix: now polyphony and # of midi channel arguments take effect
 *    (8): added message resample permitting to chose resampling interpolation method
 *    (7): added names for soundfonts (== file name without path and postfix)
 *    (6): added message 'info'
 *    (5): fixed bogus path translation at file loading
 * 
 */
#include "fluidsynth.h"
#include "fluid_synth.h"
#include "fluid_sfont.h"
#include "fluid_chan.h"
#include "ext.h"
#include "z_dsp.h"
#include "ext_obex.h"
#include "ext_dictionary.h"

typedef struct
{
  t_pxobject obj;
  fluid_synth_t *synth;
  fluid_settings_t *settings;
  int reverb;
  int chorus;
  int mute;
  float *left_out;
  float *right_out;
  long out_maxsize;
  void *outlet;
} fluidmax_t;

#define atom_issym(a) ((a)->a_type == A_SYM)
#define atom_isnum(a) ((a)->a_type == A_LONG || (a)->a_type == A_FLOAT)
#define mysneg(s) ((s)->s_name)

static t_symbol *sym_on = NULL;
static t_symbol *sym_off = NULL;
static t_symbol *sym_undefined = NULL;
static t_symbol *sym_gain = NULL;
static t_symbol *sym_channels = NULL;
static t_symbol *sym_channel = NULL;
static t_symbol *sym_soundfonts = NULL;
static t_symbol *sym_soundfont = NULL;
static t_symbol *sym_presets = NULL;
static t_symbol *sym_preset = NULL;
static t_symbol *sym_reverb = NULL;
static t_symbol *sym_chorus = NULL;
static t_symbol *sym_polyphony = NULL;
static t_symbol *sym_nearest = NULL;
static t_symbol *sym_linear = NULL;
static t_symbol *sym_cubic = NULL;
static t_symbol *sym_sinc = NULL;

/***************************************************************
 *
 *  generators
 *
 */
typedef struct
{
  int index;
  const char *name;
  const char *unit;
} fluidmax_gen_descr_t; 

static fluidmax_gen_descr_t fluidmax_gen_info[] =
{
  {0, "startOffset", "samples"}, 
  {1, "endOffset", "samples"}, 
  {2, "startLoopOffset", "samples"}, 
  {3, "endLoopOffset", "samples"}, 
  {4, "startCoarseOffset", "32k samples"}, 
  {5, "modLfoToPitch", "cent fs"}, 
  {6, "vibLfoToPitch", "cent fs"}, 
  {7, "modEnvToPitch", "cent fs"}, 
  {8, "initialFilterFc", "cent 8.176 Hz"}, 
  {9, "initialFilterQ", "cB"}, 
  {10, "modLfoToFilterFc", "cent fs"}, 
  {11, "modEnvToFilterFc", "cent fs "}, 
  {12, "endCoarseOffset", "32k samples"}, 
  {13, "modLfoToVolume", "cB fs"}, 
  {14, "<unused 1>", ""}, 
  {15, "chorusEffectsSend", "0.1%"}, 
  {16, "reverbEffectsSend", "0.1% "}, 
  {17, "pan", "0.1%"}, 
  {18, "<unused 2>", ""}, 
  {19, "<unused 3>", ""}, 
  {20, "<unused 4>", ""}, 
  {21, "delayModLfo", "timecent"}, 
  {22, "freqModLfo", "cent 8.176 "}, 
  {23, "delayVibLfo", "timecent"}, 
  {24, "freqVibLfo", "cent 8.176"}, 
  {25, "delayModEnv", "timecent"}, 
  {26, "attackModEnv", "timecent "}, 
  {27, "holdModEnv", "timecent"}, 
  {28, "decayModEnv", "timecent"}, 
  {29, "sustainModEnv", "-0.1%"}, 
  {30, "releaseModEnv", "timecent"}, 
  {31, "keynumToModEnvHold", "tcent/key"}, 
  {32, "keynumToModEnvDecay", "tcent/key"}, 
  {33, "delayVolEnv", "timecent"}, 
  {34, "attackVolEnv", "timecent"}, 
  {35, "holdVolEnv", "timecent"}, 
  {36, "decayVolEnv", "timecent"}, 
  {37, "sustainVolEnv", "cB"}, 
  {38, "releaseVolEnv", "timecent "}, 
  {39, "keynumToVolEnvHold", "tcent/key"}, 
  {40, "keynumToVolEnvDecay", "tcent/key "}, 
  {41, "instrument", ""}, 
  {42, "<reserved 1>", ""}, 
  {43, "keyRange", "MIDI"},  
  {44, "velRange", "MIDI"}, 
  {45, "startLoopCoarseOffset", "samples"}, 
  {46, "keynum", "MIDI"}, 
  {47, "velocity", "MIDI"}, 
  {48, "initialAttenuation", "cB"}, 
  {49, "<reserved 2>", ""}, 
  {50, "endLoopCoarseOffset", "samples"}, 
  {51, "coarseTune", "semitone"}, 
  {52, "fineTune", "cent"}, 
  {53, "sampleId", ""}, 
  {54, "sampleModes", "Bit Flags"}, 
  {55, "<reserved 3>", ""}, 
  {56, "scaleTuning", "cent/key"}, 
  {57, "exclusiveClass", "arbitrary#"}, 
  {58, "<unused 5>", ""},
  {59, "<unused 6>", ""},
  {60, "<unused 7>", ""},
  {61, "<unused 8>", ""},
  {62, "<unused 9>", ""},
  {63, "<unused 10>", ""}
};

#define FLUIDMAX_GEN_NUM 64
static t_symbol *fluidmax_gen_symbols[FLUIDMAX_GEN_NUM];
static t_dictionary *fluidmax_gen_dict = NULL;

/***************************************************************
 *
 *  dsp
 *
 */
static void
fluidmax_perform(fluidmax_t *self, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
  double *left_out = outs[0];
  double *right_out = outs[1];
  
  if(self->mute == 0)
  {
    int i;
    
    fluid_synth_write_float(self->synth, sampleframes, self->left_out, 0, 1, self->right_out, 0, 1);

    for(i = 0; i < sampleframes; i++)
    {
      left_out[i] = self->left_out[i];
      right_out[i] = self->right_out[i];
    }
  }
  else
  {
    int i;
    
    for(i = 0; i < sampleframes; i++)
      left_out[i] = right_out[i] = 0.0;
  }
}

static void 
fluidmax_dsp(fluidmax_t *self, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
  if(self->out_maxsize < maxvectorsize)
  {
    self->left_out = realloc(self->left_out, maxvectorsize * sizeof(float));
    self->right_out = realloc(self->right_out, maxvectorsize * sizeof(float));

    self->out_maxsize = maxvectorsize;
  }
  
  dsp_add64(dsp64, (t_object *)self, (t_perfroutine64)fluidmax_perform, 0, 0);
}

/***************************************************************
 *
 *  load utlilities
 *
 */
static char *
fluidmax_translate_fullpath(char *maxpath, char *fullpath)
{
  int i;

  strcpy(fullpath, "/Volumes/");
  
  for(i=0; maxpath[i] != ':'; i++)
    fullpath[i + 9] = maxpath[i];
    
  /* skip ':' */
  i++;
    
  strcpy(fullpath + i + 8, maxpath + i);
  
  return fullpath;
}
    
static t_symbol *
fluidmax_get_stripped_name(const char *fullpath)
{
  char stripped[1024];
  int i;
  
  for(i=strlen(fullpath)-1; i>=0; i--)
  {
    if(fullpath[i] == '/')
      break;
  }
  
  if(i != 0)
    i++;    
  
  strcpy(stripped, fullpath + i);
  
  for(i=0; stripped[i] != '\0'; i++)
  {
    if((stripped[i] == '.') && 
       (stripped[i + 1] == 's' || stripped[i + 1] == 'S') && 
       (stripped[i + 2] == 'f' || stripped[i + 2] == 'F') && 
       (stripped[i + 3] == '2'))
    {
      stripped[i] = '\0';
      break;
    }
  }
  
  return gensym(stripped);
}
    
static t_symbol *
fluidmax_sfont_get_name(fluid_sfont_t *sfont)
{
  return fluidmax_get_stripped_name(fluid_sfont_get_name(sfont));
}
    
static fluid_sfont_t *
fluidmax_sfont_get_by_name(fluidmax_t *self, t_symbol *name)
{
  int n = fluid_synth_sfcount(self->synth);
  int i;
  
  for(i=0; i<n; i++)
  {
    fluid_sfont_t *sfont = fluid_synth_get_sfont(self->synth, i);

    if(fluidmax_sfont_get_name(sfont) == name)
      return sfont;
  }

  return NULL;
}

static void 
fluidmax_do_load(t_object *o, t_symbol *s, short ac, t_atom *at)
{
  fluidmax_t *self = (fluidmax_t *)o;
  
  if(ac > 0 && atom_issym(at))  
  {
    const char *filename = mysneg(atom_getsym(at));
    t_symbol *name = fluidmax_get_stripped_name(filename);
    fluid_sfont_t *sf = fluidmax_sfont_get_by_name(self, name);
    
    if(sf == NULL)
    {
      int id = fluid_synth_sfload(self->synth, filename, 0);
    
      if(id >= 0)
      {
        object_post((t_object *)self, "loaded soundfont '%s' (id %d)", mysneg(name), id);

        fluid_synth_program_reset(self->synth);
        
        outlet_bang(self->outlet);
      }
      else
        object_error((t_object *)self, "cannot load soundfont from file '%s'", filename);
    }
    else
    {
      object_error((t_object *)self, "soundfont named '%s' is already loaded", mysneg(name));
      return;
    }
  }
}

static void
fluidmax_load_with_dialog(t_object *o, t_symbol *s, short argc, t_atom *argv)
{
  char filename[256];
  char maxpath[1024];
  char fullpath[1024];
  t_fourcc type;
  short path;
  
  open_promptset("open SoundFont 2 file");
  
  if(open_dialog(filename, &path, &type, 0, 0))
    return;
    
  if(path_topotentialname(path, filename, maxpath, 0) == 0)
  {
    t_atom a;
    
    atom_setsym(&a, gensym(fluidmax_translate_fullpath(maxpath, fullpath)));
    fluidmax_do_load(o, NULL, 1, &a);
  }
}  

/***************************************************************
 *
 *  user methods
 *
 */
static void 
fluidmax_load(t_object *o, t_symbol *s, short ac, t_atom *at)
{ 
  fluidmax_t *self = (fluidmax_t *)o;
  
  if(ac == 0)
    defer(o, (method)fluidmax_load_with_dialog, NULL, 0, 0);
  else
  {
    if(atom_issym(at))
    {
      t_symbol *name = atom_getsym(at);
      char *string = (char *)mysneg(name);
      
      if(string[0] == '/')
        defer(o, (method)fluidmax_do_load, NULL, ac, at);
      else
      {
        char maxpath[1024];
        char fullpath[1024];
        short path;
        t_fourcc type;
        t_atom a;
        
        if(locatefile_extended(string, &path, &type, 0, 0) || path_topotentialname(path, string, maxpath, 0) != 0)
        {
          object_error((t_object *)self, "cannot find file '%s'", string);
          return;
        }
        
        atom_setsym(&a, gensym(fluidmax_translate_fullpath(maxpath, fullpath)));
        defer(o, (method)fluidmax_do_load, NULL, 1, &a);
      }
    }
  }
}

static void 
fluidmax_unload(t_object *o, t_symbol *s, short ac, t_atom *at)
{
  fluidmax_t *self = (fluidmax_t *)o;
  
  if(ac > 0)
  {
    if(atom_isnum(at))  
    {
      int id = atom_getlong(at);
      fluid_sfont_t *sf = fluid_synth_get_sfont_by_id(self->synth, id);
      
      if(sf != NULL)
      {  
        t_symbol *name = fluidmax_sfont_get_name(sf);
        
        if(fluid_synth_sfunload(self->synth, id, 0) >= 0)
        {
          object_post((t_object *)self, "unloaded soundfont '%s' (id %d)", mysneg(name), id);
          return;
        }
      }
            
      object_error((t_object *)self, "cannot unload soundfont %d", id);
    }
    else if (atom_issym(at))
    {
      t_symbol *sym = atom_getsym(at);
      
      if(sym == gensym("all"))
      {
        fluid_sfont_t *sf = fluid_synth_get_sfont(self->synth, 0);
        
        fluid_synth_system_reset(self->synth);

        while(sf != NULL)
        {
          t_symbol *name = fluidmax_sfont_get_name(sf);
          unsigned int id = fluid_sfont_get_id(sf);
        
          if(fluid_synth_sfunload(self->synth, id, 0) >= 0)
            object_post((t_object *)self, "unloaded soundfont '%s' (id %d)", mysneg(name), id);
          else
            object_error((t_object *)self, "cannot unload soundfont '%s' (id %d)", mysneg(name), id);
        
          sf = fluid_synth_get_sfont(self->synth, 0);
        }
      }
      else
      {
        fluid_sfont_t *sf = fluidmax_sfont_get_by_name(self, sym);
        
        if(sf != NULL)
        {
          unsigned int id = fluid_sfont_get_id(sf);
          
          if(fluid_synth_sfunload(self->synth, id, 0) >= 0)
          {
            object_post((t_object *)self, "unloaded soundfont '%s' (id %d)", mysneg(sym), id);
            return;
          }
        }
        
        object_error((t_object *)self, "cannot unload soundfont '%s'", mysneg(sym));
      }
    }
  }
}

static void 
fluidmax_reload(t_object *o, t_symbol *s, short ac, t_atom *at)
{
  fluidmax_t *self = (fluidmax_t *)o;
  
  if(ac > 0)
  {
    if(atom_isnum(at))  
    {
      int id = atom_getlong(at);
      fluid_sfont_t *sf = fluid_synth_get_sfont_by_id(self->synth, id);
      
      if(sf != NULL)
      {
        if (fluid_synth_sfreload(self->synth, id) >= 0)
        {
          object_post((t_object *)self, "reloaded soundfont '%s' (id %d)", id);
          return;
        }

        object_error((t_object *)self, "cannot reload soundfont %d", id);
      }
    }
    else if (atom_issym(at))
    {
      t_symbol *sym = atom_getsym(at);
      
      if(sym == gensym("all"))
      {
        int n = fluid_synth_sfcount(self->synth);
        int i;
        
        fluid_synth_system_reset(self->synth);

        for(i=0; i<n; i++)
        {
          fluid_sfont_t *sf = fluid_synth_get_sfont(self->synth, i);
          t_symbol *name = fluidmax_sfont_get_name(sf);
          unsigned int id = fluid_sfont_get_id(sf);
          
        
          if (fluid_synth_sfreload(self->synth, id) >= 0)
            object_post((t_object *)self, "reloaded soundfont '%s' (id %d)", mysneg(name), id);
          else
            object_error((t_object *)self, "cannot reload soundfont '%s' (id %d)", mysneg(name), id);
        }
      }
      else
      {
        fluid_sfont_t *sf = fluidmax_sfont_get_by_name(self, sym);
        
        if(sf != NULL)
        {
          unsigned int id = fluid_sfont_get_id(sf);
          
          if(fluid_synth_sfreload(self->synth, id) >= 0)
          {
            object_post((t_object *)self, "reloaded soundfont '%s' (id %d)", mysneg(sym), id);
            return;
          }
        }
        
        object_error((t_object *)self, "cannot reload soundfont '%s'", mysneg(sym));
      }
    }
  }
}

static void 
fluidmax_note(t_object *o, t_symbol *s, short ac, t_atom *at)
{
  fluidmax_t *self = (fluidmax_t *)o;
  
  if(ac > 0 && atom_isnum(at))
  {
    int velocity = 64;
    int channel = 1;
    
    switch(ac)
    {
      default:
      case 3:
        if(atom_isnum(at + 2))
        {
          channel = atom_getlong(at + 2);
                    
          if(channel < 1)
            channel = 1;
          else if(channel > fluid_synth_count_midi_channels(self->synth))
            channel = fluid_synth_count_midi_channels(self->synth);
        }
      case 2:
        if(atom_isnum(at + 1))
          velocity = atom_getlong(at + 1);
      case 1:
        fluid_synth_noteon(self->synth, channel - 1, atom_getlong(at), velocity);
      case 0:
        break;
    }
  }
}

static void 
fluidmax_list(t_object *o, t_symbol *s, short ac, t_atom *at)
{
  fluidmax_note(o, NULL, ac, at);
}


static void 
fluidmax_control_change(t_object *o, t_symbol *s, short ac, t_atom *at)
{
  fluidmax_t *self = (fluidmax_t *)o;
  
  if(ac > 0 && atom_isnum(at))
  {
    int value = 64;
    int channel = 1;
    
    switch(ac)
    {
      default:
      case 3:
        if(atom_isnum(at + 2))
        {
          channel = atom_getlong(at + 2);
          
          if(channel < 1)
            channel = 1;
          else if(channel > fluid_synth_count_midi_channels(self->synth))
            channel = fluid_synth_count_midi_channels(self->synth);
        }
      case 2:
        if(atom_isnum(at + 1))
          value = atom_getlong(at + 1);
      case 1:
        fluid_synth_cc(self->synth, channel - 1, atom_getlong(at), value);
      case 0:
        break;
    }
  }
}

static void 
fluidmax_mod(t_object *o, t_symbol *s, short ac, t_atom *at)
{
  fluidmax_t *self = (fluidmax_t *)o;
  
  if(ac > 1 && atom_isnum(at) && atom_isnum(at + 1))
  {
    int param = atom_getlong(at);
    float value = atom_getfloat(at + 1);
    int channel = 1;
    
    if(ac > 2 && atom_isnum(at + 2))
    {
      channel = atom_getlong(at + 2);
      
      if(channel < 1)
        channel = 1;
      else if(channel > fluid_synth_count_midi_channels(self->synth))
        channel = fluid_synth_count_midi_channels(self->synth);
    }

    fluid_synth_set_gen(self->synth, channel - 1, param, value);
  }
}

static void 
fluidmax_pitch_bend(t_object *o, t_symbol *s, short ac, t_atom *at)
{  
  fluidmax_t *self = (fluidmax_t *)o;
  
  if(ac > 0 && atom_isnum(at))
  {
    int channel = 1;
    double bend = 0.0;
    
    if(ac > 1 && atom_isnum(at + 1))
    {
      channel = atom_getlong(at + 1);
          
      if(channel < 1)
        channel = 1;
      else if(channel > fluid_synth_count_midi_channels(self->synth))
        channel = fluid_synth_count_midi_channels(self->synth);
    }
      
    bend = atom_getfloat(at);
    
    if(bend < 0.0)
      bend = 0.0;
    else if(bend > 127.0)
      bend = 127.0;
  
    fluid_synth_pitch_bend(self->synth, channel - 1, (int)(bend * 128.0));
  }
}

static void 
fluidmax_pitch_bend_wheel(t_object *o, t_symbol *s, short ac, t_atom *at)
{  
  fluidmax_t *self = (fluidmax_t *)o;
  
  if(ac > 0 && atom_isnum(at))
  {
    int channel = 1;
    
    if(ac > 1 && atom_isnum(at + 1))
      channel = atom_getlong(at + 1);
      
    fluid_synth_pitch_wheel_sens(self->synth, channel - 1, atom_getlong(at));
  }
}

static void 
fluidmax_program_change(t_object *o, t_symbol *s, short ac, t_atom *at)
{  
  fluidmax_t *self = (fluidmax_t *)o;
  
  if(ac > 0 && atom_isnum(at))
  {
    int channel = 1;
    
    if(ac > 1 && atom_isnum(at + 1))
    {
      channel = atom_getlong(at + 1);
          
      if(channel < 1)
        channel = 1;
      else if(channel > fluid_synth_count_midi_channels(self->synth))
        channel = fluid_synth_count_midi_channels(self->synth);
    }
      
    fluid_synth_program_change(self->synth, channel - 1, atom_getlong(at));
  }
}

static void 
fluidmax_bank_select(t_object *o, t_symbol *s, short ac, t_atom *at)
{
  fluidmax_t *self = (fluidmax_t *)o;
  
  if(ac > 0 && atom_isnum(at))
  {
    int channel = 1;
    unsigned int sf_id;
    unsigned int bank_num;
    unsigned int prog_num;
    
    if(ac > 1 && atom_isnum(at + 1))
    {
      channel = atom_getlong(at + 1);
          
      if(channel < 1)
        channel = 1;
      else if(channel > fluid_synth_count_midi_channels(self->synth))
        channel = fluid_synth_count_midi_channels(self->synth);
    }
      
    fluid_synth_bank_select(self->synth, channel - 1, atom_getlong(at));
    fluid_synth_get_program(self->synth, channel - 1, &sf_id, &bank_num, &prog_num);
    fluid_synth_program_change(self->synth, channel - 1, prog_num);
  }
}

static void 
fluidmax_select(t_object *o, t_symbol *s, short ac, t_atom *at)
{
  fluidmax_t *self = (fluidmax_t *)o;
  unsigned int bank = 0;
  unsigned int preset = 0;  
  int channel = 1;
  
  switch(ac)
  {
    default:
    case 4:
      if(atom_isnum(at + 3))
        channel = atom_getlong(at + 3);

      if(channel < 1)
        channel = 1;
      else if(channel > fluid_synth_count_midi_channels(self->synth))
        channel = fluid_synth_count_midi_channels(self->synth);
        
    case 3:
      if(atom_isnum(at + 2))
        preset = atom_getlong(at + 2);
        
    case 2:
      if(atom_isnum(at + 1))
        bank = atom_getlong(at + 1);
        
    case 1:
      if(atom_isnum(at))
        fluid_synth_program_select(self->synth, channel - 1, atom_getlong(at), bank, preset);
      else if(atom_issym(at))
      {
        t_symbol *name = atom_getsym(at);
        fluid_sfont_t *sf = fluidmax_sfont_get_by_name(self, name);
        
        if(sf != NULL)
          fluid_synth_program_select(self->synth, channel - 1, fluid_sfont_get_id(sf), bank, preset);
        else
          object_error((t_object *)self, "cannot find soundfont named '%s'", mysneg(name));
      }
    case 0:
      break;
  }
}

static void 
fluidmax_reverb(t_object *o, t_symbol *s, short ac, t_atom *at)
{
  fluidmax_t *self = (fluidmax_t *)o;
  
  if(ac == 0)
  {
    fluid_synth_set_reverb_on(self->synth, 1);
    fluid_revmodel_reset(self->synth->reverb);
    self->reverb = 1;
  }
  else if(atom_isnum(at))
  {
    double room = fluid_synth_get_reverb_roomsize(self->synth);
    double damping = fluid_synth_get_reverb_damp(self->synth);
    double width = fluid_synth_get_reverb_width(self->synth);

    fluid_synth_set_reverb_on(self->synth, 1);
    self->reverb = 1;
    
    switch(ac)
    {
      default:
      case 4:
        if(atom_isnum(at + 3))
          width = atom_getfloat(at + 3);
      case 3:
        if(atom_isnum(at + 2))
          damping = atom_getfloat(at + 2);
      case 2:
        if(atom_isnum(at + 1))
          room = atom_getfloat(at + 1);
      case 1:
        fluid_synth_set_reverb(self->synth, room, damping, width, atom_getfloat(at));
      case 0:
        break;
    }
  }
  else if(atom_issym(at))
  {
    t_symbol *sym = atom_getsym(at);
    
    if(sym == sym_on)
    {
      fluid_synth_set_reverb_on(self->synth, 1);
      self->reverb = 1;
    }
    else if(sym == sym_off)
    {
      fluid_synth_set_reverb_on(self->synth, 0);
      self->reverb = 0;
    }
  }
}

static void 
fluidmax_chorus(t_object *o, t_symbol *s, short ac, t_atom *at)
{
  fluidmax_t *self = (fluidmax_t *)o;
  
  if(ac == 0)
  {
    fluid_synth_set_chorus_on(self->synth, 1);
    fluid_chorus_reset(self->synth->chorus);
    self->chorus = 1;
  }
  else if(atom_isnum(at))
  {
    double speed = fluid_synth_get_chorus_speed_Hz(self->synth);
    double depth = fluid_synth_get_chorus_depth_ms(self->synth);
    int type = fluid_synth_get_chorus_type(self->synth);
    int nr = fluid_synth_get_chorus_nr(self->synth);

    fluid_synth_set_chorus_on(self->synth, 1);
    self->chorus = 1;

    switch(ac)
    {
      default:
      case 5:
        if(atom_isnum(at + 4))
          nr = atom_getlong(at + 4);
      case 4:
        if(atom_isnum(at + 3))
          type = atom_getlong(at + 3);
      case 3:
        if(atom_isnum(at + 2))
          depth = atom_getfloat(at + 2);
      case 2:
        if(atom_isnum(at + 1))
          speed = atom_getfloat(at + 1);
      case 1:
        fluid_synth_set_chorus(self->synth, nr, atom_getfloat(at), speed, depth, type);
      case 0:
        break;
    }
  }
  else if(atom_issym(at))
  {
    t_symbol *sym = atom_getsym(at);
    
    if(sym == sym_on)
    {
      fluid_synth_set_chorus_on(self->synth, 1);
      self->chorus = 1;
    }
    else if(sym == sym_off)
    {
      fluid_synth_set_chorus_on(self->synth, 0);
      self->chorus = 0;
    }
  }
}

static void 
fluidmax_set_gain(t_object *o, t_symbol *s, short ac, t_atom *at)
{
  fluidmax_t *self = (fluidmax_t *)o;
  
  if(ac > 0 && atom_isnum(at))
  {
    float gain = atom_getfloat(at);
    
    fluid_synth_set_gain(self->synth, gain);
  }
}

static void 
fluidmax_set_resampling_method(t_object *o, t_symbol *s, short ac, t_atom *at)
{
  fluidmax_t *self = (fluidmax_t *)o;
  
  if(ac > 0)
  {
    if(atom_isnum(at))
    {
      int ip = atom_getlong(at);
      
      if(ip == 0)
        fluid_synth_set_interp_method(self->synth, -1, FLUID_INTERP_NONE);
      else if(ip < 3)
        fluid_synth_set_interp_method(self->synth, -1, FLUID_INTERP_LINEAR);
      else if(ip < 6)
        fluid_synth_set_interp_method(self->synth, -1, FLUID_INTERP_4THORDER);
      else
        fluid_synth_set_interp_method(self->synth, -1, FLUID_INTERP_7THORDER);
    }
    else if(atom_issym(at))
    {
      t_symbol *sym = atom_getsym(at);
      
      if(sym == sym_nearest)
        fluid_synth_set_interp_method(self->synth, -1, FLUID_INTERP_NONE);
      else if(sym == sym_linear)
        fluid_synth_set_interp_method(self->synth, -1, FLUID_INTERP_LINEAR);
      else if(sym == sym_cubic)
        fluid_synth_set_interp_method(self->synth, -1, FLUID_INTERP_4THORDER);
      else if(sym == sym_sinc)
        fluid_synth_set_interp_method(self->synth, -1, FLUID_INTERP_7THORDER);      
      else
        object_error((t_object *)self, "undefined resampling method: %s", mysneg(sym));
    }
  }
}

static void 
fluidmax_panic(t_object *o, t_symbol *s, short ac, t_atom *at)
{
  fluidmax_t *self = (fluidmax_t *)o;

  fluid_synth_system_reset(self->synth);
}

static void 
fluidmax_reset(t_object *o, t_symbol *s, short ac, t_atom *at)
{
  fluidmax_t *self = (fluidmax_t *)o;
  int n = fluid_synth_count_midi_channels(self->synth);
  int i;

  for (i = 0; i < n; i++)
    fluid_channel_reset(self->synth->channel[i]);
}

static void 
fluidmax_mute(t_object *o, t_symbol *s, short ac, t_atom *at)
{
  fluidmax_t *self = (fluidmax_t *)o;
  int mute = 1;
  
  if(ac > 0 && atom_isnum(at))
    mute = (atom_getlong(at) != 0);
  
  fluid_synth_system_reset(self->synth);
   
  self->mute = mute;
}

static void 
fluidmax_unmute(t_object *o)
{
  t_atom a;
  
  atom_setlong(&a, 0);  
  fluidmax_mute(o, NULL, 1, &a);
}

static void 
fluidmax_generator(t_object *o, t_symbol *s, short ac, t_atom *at)
{
  fluidmax_t *self = (fluidmax_t *)o;
  
  if(ac > 1)
  {
    long channel = 1;
    t_atom_long param = -1;
    float value = 0.0;
    
    switch(ac)
    {
      default:
      {
        channel = atom_getlong(at + 2);
        
        if(channel < 1)
          channel = 1;
        else if(channel > fluid_synth_count_midi_channels(self->synth))
          channel = fluid_synth_count_midi_channels(self->synth);
      }
        
      case 2:
      {
        if(atom_isnum(at + 1))
          value = atom_getfloat(at + 1);
      }
        
      case 1:
      {
        if(atom_isnum(at))
        {
          param = atom_getlong(at);
          
          if(param < 0)
            param = 0;
          else if(param >= FLUIDMAX_GEN_NUM)
            param = FLUIDMAX_GEN_NUM - 1;
        }
        else if(atom_issym(at))
        {
          t_symbol *sym = atom_getsym(at);
          
          if(dictionary_getlong(fluidmax_gen_dict, sym, &param) != MAX_ERR_NONE)
            object_error((t_object *)self, "invalid generator '%s'", mysneg(sym));
        }
      }
        
      case 0:
        break;
    }
    
    if(param >= 0)
      fluid_synth_set_gen(self->synth, channel - 1, param, value);
  }
}

/* 
int fluid_synth_count_audio_channels (fluid_synth_t *synth)
int fluid_synth_count_audio_groups (fluid_synth_t *synth)
int fluid_synth_count_effects_channels (fluid_synth_t *synth)
*/

static void 
fluidmax_tuning_octave(t_object *o, t_symbol *s, short ac, t_atom *at)
{
  fluidmax_t *self = (fluidmax_t *)o;
  char *name = NULL;
  int tuning_bank = 0;
  int tuning_prog = 0;
  double pitch[12];
  int i, n;
  
  if(ac > 0 && atom_issym(at))
  {
    name = mysneg(atom_getsym(at));
    at++;
    ac--;
  }
  
  n = ac - 2;
  if(n > 12)
    n = 12;

  if(ac > 0 && atom_isnum(at))
    tuning_bank = atom_getlong(at) % 128;
  
  if(ac > 1 && atom_isnum(at + 1))
    tuning_prog = atom_getlong(at) % 128;
    
  for(i=0; i<n; i++)
  {
    if(atom_isnum(at + i + 2))
      pitch[i] = atom_getfloat(at + i + 2);
    else
      pitch[i] = 0.0;
  }
  
  for(; i<12; n++)
    pitch[i] = 0.0;

  fluid_synth_create_octave_tuning(self->synth, tuning_bank, tuning_prog, name, pitch);
}

static void 
fluidmax_tuning_select(t_object *o, t_symbol *s, short ac, t_atom *at)
{
  fluidmax_t *self = (fluidmax_t *)o;
  int tuning_bank = 0;
  int tuning_prog = 0;
  int channel = 1;
  
  if(ac > 0 && atom_isnum(at))
    tuning_bank = atom_getlong(at) % 128;
    
  if(ac > 1 && atom_isnum(at + 1))
    tuning_prog = atom_getlong(at + 1) % 128;
  
  if(ac > 2 && atom_isnum(at + 2))
    channel = atom_getlong(at + 2);
    
  if(channel < 1)
    channel = 1;
  else if(channel > fluid_synth_count_midi_channels(self->synth))
    channel = fluid_synth_count_midi_channels(self->synth);
    
  fluid_synth_select_tuning(self->synth, channel - 1, tuning_bank, tuning_prog);
}

static void 
fluidmax_tuning_reset(t_object *o, t_symbol *s, short ac, t_atom *at)
{
  fluidmax_t *self = (fluidmax_t *)o;
  int channel = 0;
  
  if(ac > 0 && atom_isnum(at))
    channel = atom_getlong(at);

  if(channel < 1)
    channel = 1;
  else if(channel > fluid_synth_count_midi_channels(self->synth))
    channel = fluid_synth_count_midi_channels(self->synth);
    
  fluid_synth_reset_tuning(self->synth, channel - 1);
}

/* more tuning ??
fluid_synth_create_key_tuning (fluid_synth_t *synth, int tuning_bank, int tuning_prog, char *name, double *pitch)
fluid_synth_tune_notes (fluid_synth_t *synth, int tuning_bank, int tuning_prog, int len, int *keys, double *pitch, int apply)
fluid_synth_tuning_iteration_start (fluid_synth_t *synth)
fluid_synth_tuning_iteration_next (fluid_synth_t *synth, int *bank, int *prog)
fluid_synth_tuning_dump (fluid_synth_t *synth, int bank, int prog, char *name, int len, double *pitch)
*/

extern fluid_gen_info_t fluid_gen_info[];

static void 
fluidmax_print(t_object *o, t_symbol *s, short ac, t_atom *at)
{
  fluidmax_t *self = (fluidmax_t *)o;
  
  if(ac > 0)
  {
    if(atom_issym(at))
    {
      t_symbol *sym = atom_getsym(at);
      
      if(sym == sym_soundfonts)
      {
        int n = fluid_synth_sfcount(self->synth);
        int i;
        
        if(n > 0)
          object_post((t_object *)self, "soundfonts:");
        else
          object_post((t_object *)self, "no soundfonts loaded");          
        
        for(i=0; i<n; i++)
        {
          fluid_sfont_t *sf = fluid_synth_get_sfont(self->synth, i);
          t_symbol *name = fluidmax_sfont_get_name(sf);
          unsigned int id = fluid_sfont_get_id(sf);
          
          post("  %d: '%s' (id %d)", i, mysneg(name), id);
        }
      }
      else if(sym == sym_presets)
      {
        int n = fluid_synth_sfcount(self->synth);
        
        if(n > 0)
        {
          if(ac > 1)
          {
            fluid_sfont_t *sf = NULL;
            t_symbol *name;
         
            if(atom_issym(at + 1))
            {
              name = atom_getsym(at + 1);
              sf = fluidmax_sfont_get_by_name(self, name);
            }
            else if(atom_isnum(at + 1))
            {
              int id = atom_getlong(at + 1);
              
              sf = fluid_synth_get_sfont_by_id(self->synth, id);
              name = fluidmax_sfont_get_name(sf);
            }
            
            if(sf != NULL)
            {
              fluid_preset_t preset;
              
              fluid_sfont_iteration_start(sf);
              
              object_post((t_object *)self, "presets of soundfont '%s':", mysneg(name));
              
              while(fluid_sfont_iteration_next(sf, &preset) > 0)
              {
                char *preset_str = fluid_preset_get_name(&preset);
                t_symbol *preset_name = gensym(preset_str);
                int bank_num = fluid_preset_get_banknum(&preset);
                int prog_num = fluid_preset_get_num(&preset);
                
                post("  '%s': bank %d, program %d", mysneg(preset_name), bank_num, prog_num);
              }
            }
          }
          else
          {
            int i;

            object_post((t_object *)self, "fluidsynth~ presets:");
            
            for(i=0; i<128; i++)
            {
              int j;
              
              for(j=0; j<128; j++)
              {
                fluid_preset_t *preset = NULL;
                fluid_sfont_t *sf = NULL;
                int k;
                
                for(k=0; k<n; k++)
                {
                  sf = fluid_synth_get_sfont(self->synth, k);                  
                  preset = fluid_sfont_get_preset(sf, i, j);
                  
                  if(preset != NULL)
                    break;
                }
                
                if(preset != NULL)
                {
                  t_symbol *sf_name = fluidmax_sfont_get_name(sf);
                  char *preset_str = fluid_preset_get_name(preset);
                  t_symbol *preset_name = gensym(preset_str);
                  
                  post("  '%s': soundfont '%s', bank %d, program %d", 
                    mysneg(preset_name), mysneg(sf_name), i, j);
                }
              }
            }
          }
        }
        else
          object_post((t_object *)self, "no soundfonts loaded");
      }
      else if(sym == sym_channels)
      {
        int n = fluid_synth_count_midi_channels(self->synth);
        int i;
        
        object_post((t_object *)self, "channels:");
        
        for(i=0; i<n; i++)
        {
          fluid_preset_t *preset = fluid_synth_get_channel_preset(self->synth, i);
          
          if(preset != NULL)
          {
            char *preset_str = fluid_preset_get_name(preset);
            t_symbol *preset_name = gensym(preset_str);
            unsigned int sf_id;
            unsigned int bank_num;
            unsigned int prog_num;
            fluid_sfont_t *sf;
            
            fluid_synth_get_program(self->synth, i, &sf_id, &bank_num, &prog_num);
            sf = fluid_synth_get_sfont_by_id(self->synth, sf_id);
            
            post("  %d: soundfont '%s', bank %d, program %d: '%s'", 
              i + 1, mysneg(fluidmax_sfont_get_name(sf)), bank_num, prog_num, mysneg(preset_name));
          }
          else
            post("  channel %d: no preset", i + 1);
        }
      }
      else if(sym == gensym("generators"))
      {
        int channel = 1;
        int n = GEN_LAST;
        int i;
        
        if(ac > 1 && atom_isnum(at + 1))
          channel = atom_getlong(at + 1);

        if(channel < 1)
          channel = 1;
        else if(channel > fluid_synth_count_midi_channels(self->synth))
          channel = fluid_synth_count_midi_channels(self->synth);
          
        object_post((t_object *)self, "generators of channel %d:", channel);
        
        for(i=0; i<n; i++)
        {
          const char *name = fluidmax_gen_info[i].name;
          const char *unit = fluidmax_gen_info[i].unit;
          double incr = fluid_synth_get_gen(self->synth, channel - 1, i);
          double min = fluid_gen_info[i].min;
          double max = fluid_gen_info[i].max;
          
          post("  %d '%s': %s %g [%g ... %g] (%s)", i, name, (incr >= 0)? "": "-" , fabs(incr), min, max, unit);
        }
      }
      else if(sym == sym_gain)
      {
        double gain = fluid_synth_get_gain(self->synth);

        post("gain: %g", gain);
      }
      else if(sym == sym_reverb)
      {
        double level = fluid_synth_get_reverb_level(self->synth);
        double room = fluid_synth_get_reverb_roomsize(self->synth);
        double damping = fluid_synth_get_reverb_damp(self->synth);
        double width = fluid_synth_get_reverb_width(self->synth);
        
        if(self->reverb != 0)
        {
          object_post((t_object *)self, "current reverb parameters:");
          post("  level: %f", level);
          post("  room size: %f", room);
          post("  damping: %f", damping);
          post("  width: %f", width);
        }
        else
          object_post((t_object *)self, "reverb off");        
      }
      else if(sym == sym_chorus)
      {
        if(self->chorus != 0)
        {
          double level = fluid_synth_get_chorus_level(self->synth);
          double speed = fluid_synth_get_chorus_speed_Hz(self->synth);
          double depth = fluid_synth_get_chorus_depth_ms(self->synth);
          int type = fluid_synth_get_chorus_type(self->synth);
          int nr = fluid_synth_get_chorus_nr(self->synth);
          
          object_post((t_object *)self, "current chorus parameters:");
          post("  level: %f", level);
          post("  speed: %f Hz", speed);
          post("  depth: %f msec", depth);
          post("  type: %d (%s)", type, type? "triangle": "sine");
          post("  %d units", nr);
        }
        else
          object_post((t_object *)self, "chorus off");
      }
    }
  }
}

static void 
fluidmax_info(t_object *o, t_symbol *s, short ac, t_atom *at)
{
  fluidmax_t *self = (fluidmax_t *)o;
  
  if(ac > 0)
  {
    if(atom_issym(at))
    {
      t_symbol *sym = atom_getsym(at);
      
      if(sym == sym_soundfonts)
      {
        int n = fluid_synth_sfcount(self->synth);
        int i;
        
        for(i=0; i<n; i++)
        {
          fluid_sfont_t *sf = fluid_synth_get_sfont(self->synth, i);
          unsigned int id = fluid_sfont_get_id(sf);
          t_atom a[3];
          
          atom_setlong(a, i);
          atom_setsym(a + 1, fluidmax_sfont_get_name(sf));
          atom_setlong(a + 2, id);
          outlet_anything(self->outlet, sym_soundfont, 3, a);
        }
      }
      else if(sym == sym_presets)
      {
        int n = fluid_synth_sfcount(self->synth);
        
        if(n > 0)
        {
          if(ac > 1)
          {
            fluid_sfont_t *sf = NULL;
            t_symbol *sf_name;
         
            if(atom_issym(at + 1))
            {
              sf_name = atom_getsym(at + 1);
              sf = fluidmax_sfont_get_by_name(self, sf_name);
            }
            else if(atom_isnum(at + 1))
            {
              int id = atom_getlong(at + 1);
              
              sf = fluid_synth_get_sfont_by_id(self->synth, id);
              sf_name = fluidmax_sfont_get_name(sf);
            }
            
            if(sf != NULL)
            {
              fluid_preset_t preset;
              
              fluid_sfont_iteration_start(sf);
              
              while(fluid_sfont_iteration_next(sf, &preset) > 0)
              {
                char *preset_str = fluid_preset_get_name(&preset);
                t_symbol *preset_name = gensym(preset_str);
                int bank_num = fluid_preset_get_banknum(&preset);
                int prog_num = fluid_preset_get_num(&preset);
                t_atom a[4];
                
                atom_setsym(a , preset_name);
                atom_setsym(a + 1, sf_name);
                atom_setlong(a + 2, bank_num);
                atom_setlong(a + 3, prog_num);
                outlet_anything(self->outlet, sym_preset, 4, a);
              }
            }
          }
          else
          {
            int i;

            for(i=0; i<128; i++)
            {
              int j;
              
              for(j=0; j<128; j++)
              {
                fluid_preset_t *preset = NULL;
                fluid_sfont_t *sf = NULL;
                int k;
                
                for(k=0; k<n; k++)
                {
                  sf = fluid_synth_get_sfont(self->synth, k);                  
                  preset = fluid_sfont_get_preset(sf, i, j);
                  
                  if(preset != NULL)
                    break;
                }
                
                if(preset != NULL)
                {
                  t_symbol *sf_name = fluidmax_sfont_get_name(sf);
                  char *preset_str = fluid_preset_get_name(preset);
                  t_symbol *preset_name = gensym(preset_str);
                  t_atom a[4];
                  
                  atom_setsym(a , preset_name);
                  atom_setsym(a + 1, sf_name);
                  atom_setlong(a + 2, i);
                  atom_setlong(a + 3, j);
                  outlet_anything(self->outlet, sym_preset, 4, a);
                }
              }
            }
          }
        }
        else
          object_error(o, "no soundfonts loaded");
      }
      else if(sym == sym_channels)
      {
        int n = fluid_synth_count_midi_channels(self->synth);
        int i;
        
        for(i=0; i<n; i++)
        {
          fluid_preset_t *preset = fluid_synth_get_channel_preset(self->synth, i);
          
          if(preset != NULL)
          {
            char *preset_str = fluid_preset_get_name(preset);
            t_symbol *preset_name = gensym(preset_str);
            unsigned int sf_id, bank_num, prog_num;
            fluid_sfont_t *sf;
            t_atom a[5];
            
            fluid_synth_get_program(self->synth, i, &sf_id, &bank_num, &prog_num);
            sf = fluid_synth_get_sfont_by_id(self->synth, sf_id);

            atom_setlong(a, i + 1);
            atom_setsym(a + 1, fluidmax_sfont_get_name(sf));
            atom_setlong(a + 2, bank_num);
            atom_setlong(a + 3, prog_num);
            atom_setsym(a + 4, preset_name);
            outlet_anything(self->outlet, sym_channel, 5, a);
          }
          else
          {
            t_atom a[2];
            
            atom_setlong(a, i + 1);
            atom_setsym(a + 1, sym_undefined);
            outlet_anything(self->outlet, sym_channel, 2, a);
          }
        }
      }
      else if(sym == sym_gain)
      {
        t_atom a;
        double gain = fluid_synth_get_gain(self->synth);

        atom_setfloat(&a, gain);
        outlet_anything(self->outlet, sym_channel, 1, &a);
      }
      else if(sym == sym_reverb)
      {
        if(self->reverb != 0)
        {
          double level = fluid_synth_get_reverb_level(self->synth);
          double room = fluid_synth_get_reverb_roomsize(self->synth);
          double damping = fluid_synth_get_reverb_damp(self->synth);
          double width = fluid_synth_get_reverb_width(self->synth);
          t_atom a[4];
          
          atom_setfloat(a, level);
          atom_setfloat(a + 1, room);
          atom_setfloat(a + 2, damping);
          atom_setfloat(a + 3, width);
          outlet_anything(self->outlet, sym_reverb, 4, a);          
        }
        else
        {
          t_atom a;
          
          atom_setsym(&a, sym_off);
          outlet_anything(self->outlet, sym_reverb, 1, &a);
        }
      }
      else if(sym == sym_chorus)
      {
        if(self->chorus != 0)
        {
          double level = fluid_synth_get_chorus_level(self->synth);
          double speed = fluid_synth_get_chorus_speed_Hz(self->synth);
          double depth = fluid_synth_get_chorus_depth_ms(self->synth);
          int type = fluid_synth_get_chorus_type(self->synth);
          int nr = fluid_synth_get_chorus_nr(self->synth);
          t_atom a[5];
          
          atom_setfloat(a, level);
          atom_setfloat(a + 1, speed);
          atom_setfloat(a + 2, depth);
          atom_setlong(a + 3, type);
          atom_setlong(a + 4, nr);
          outlet_anything(self->outlet, sym_chorus, 5, a);
        }
        else
        {
          t_atom a;
          
          atom_setsym(&a, sym_off);
          outlet_anything(self->outlet, sym_chorus, 1, &a);
        }
      }
      else if(sym == sym_polyphony)
      {
        int polyphony = fluid_synth_get_polyphony(self->synth);
        t_atom a;
        
        atom_setlong(&a, polyphony);
        outlet_anything(self->outlet, sym_polyphony, 1, &a);
      }
    }
  }
}

/***************************************************************
 *
 *  class
 *
 */
static t_class *fluidmax_class = NULL;

static void *
fluidmax_new(t_symbol *s, short ac, t_atom *at)
{  
  fluidmax_t *self = (fluidmax_t *)object_alloc((t_class *)fluidmax_class);
  
  if(self != NULL) 
  {
    int polyphony = 256;
    int midi_channels = 16;
    
    self->left_out = NULL;
    self->right_out = NULL;
    self->out_maxsize = 0;
    
    self->outlet = outlet_new(self, "anything");
    
    dsp_setup((t_pxobject *)self, 0);
    outlet_new(self, "signal");
    outlet_new(self, "signal");
    
    self->synth = NULL;
    self->settings = new_fluid_settings();
    self->reverb = 0;
    self->chorus = 0;
    self->mute = 0;
    
    if(ac > 0 && atom_isnum(at))
    {
      polyphony = atom_getlong(at);
      ac--;
      at++;
    }
    
    if(ac > 0 && atom_isnum(at))
    {
      midi_channels = atom_getlong(at);
      ac--;
      at++;
    }
    
    if(ac > 0 && atom_issym(at))
    {
      fluidmax_load((t_object *)self, NULL, 1, at);
    }
    
    if(self->settings != NULL)
    {
      fluid_settings_setint(self->settings, "synth.midi-channels", midi_channels);
      fluid_settings_setint(self->settings, "synth.polyphony", polyphony);
      fluid_settings_setnum(self->settings, "synth.gain", 0.600000);
      fluid_settings_setnum(self->settings, "synth.sample-rate", sys_getsr());
      
      self->synth = new_fluid_synth(self->settings);
      
      if(self->synth != NULL)
      {
        fluid_synth_set_reverb_on(self->synth, 0);
        fluid_synth_set_chorus_on(self->synth, 0);      
        
        if(ac > 0 && atom_issym(at))
          fluidmax_load((t_object *)self, NULL, ac, at);
        
        return self;
      }
      
      delete_fluid_settings(self->settings);
    }
    
    object_error((t_object *)self, "cannot create FluidSynth core");
  }
  
  return NULL;
}

static void
fluidmax_free(t_pxobject *o)
{
  fluidmax_t *self = (fluidmax_t *)o;

  if(self->settings != NULL )
    delete_fluid_settings(self->settings);

  if(self->synth != NULL )
    delete_fluid_synth(self->synth);
  
  if(self->left_out != NULL)
    free(self->left_out);
  
  if(self->right_out != NULL)
    free(self->right_out);

  dsp_free(o);
}

int 
main(void)
{
  t_class *c = class_new("fluidsynth~", (method)fluidmax_new, (method)fluidmax_free, (long)sizeof(fluidmax_t), 0L, A_GIMME, 0);
  int i;
  
  class_dspinit(c);

  class_addmethod(c, (method)fluidmax_dsp, "dsp64", A_CANT, 0);
  class_addmethod(c, (method)fluidmax_print, "print", A_GIMME, 0);

  class_addmethod(c, (method)fluidmax_load, "load", A_GIMME, 0);
  class_addmethod(c, (method)fluidmax_unload, "unload", A_GIMME, 0);
  class_addmethod(c, (method)fluidmax_reload, "reload", A_GIMME, 0);
  class_addmethod(c, (method)fluidmax_info, "info", A_GIMME, 0);
  
  class_addmethod(c, (method)fluidmax_panic, "panic", A_GIMME, 0);
  class_addmethod(c, (method)fluidmax_reset, "reset", A_GIMME, 0);
  class_addmethod(c, (method)fluidmax_mute, "mute", A_GIMME, 0);
  class_addmethod(c, (method)fluidmax_unmute, "unmute", 0);

  class_addmethod(c, (method)fluidmax_generator, "generator", A_GIMME, 0);

  /*class_addmethod(c, (method)fluidmax_tuning_keys, "tuning-keys", A_GIMME, 0);*/
  class_addmethod(c, (method)fluidmax_tuning_octave, "tuning-octave", A_GIMME, 0);
  class_addmethod(c, (method)fluidmax_tuning_select, "tuning-select", A_GIMME, 0);
  class_addmethod(c, (method)fluidmax_tuning_reset, "tuning-reset", A_GIMME, 0);

  class_addmethod(c, (method)fluidmax_reverb, "reverb", A_GIMME, 0);
  class_addmethod(c, (method)fluidmax_chorus, "chorus", A_GIMME, 0);  
  class_addmethod(c, (method)fluidmax_set_gain, "gain", A_GIMME, 0);  
  class_addmethod(c, (method)fluidmax_set_resampling_method, "resample", A_GIMME, 0);  
    
  class_addmethod(c, (method)fluidmax_note, "note", A_GIMME, 0);
  class_addmethod(c, (method)fluidmax_list, "list", A_GIMME, 0);

  class_addmethod(c, (method)fluidmax_control_change, "control", A_GIMME, 0);
  class_addmethod(c, (method)fluidmax_mod, "mod", A_GIMME, 0);
  
  class_addmethod(c, (method)fluidmax_pitch_bend, "bend", A_GIMME, 0);
  class_addmethod(c, (method)fluidmax_pitch_bend_wheel, "wheel", A_GIMME, 0);

  class_addmethod(c, (method)fluidmax_program_change, "program", A_GIMME, 0);
  class_addmethod(c, (method)fluidmax_bank_select, "bank", A_GIMME, 0);
  class_addmethod(c, (method)fluidmax_select, "select", A_GIMME, 0);

  sym_on = gensym("on");
  sym_off = gensym("off");
  sym_undefined = gensym("undefined");
  sym_gain = gensym("gain");
  sym_channels = gensym("channels");
  sym_channel = gensym("channel");
  sym_soundfonts = gensym("soundfonts");
  sym_soundfont = gensym("soundfont");
  sym_presets = gensym("presets");
  sym_preset = gensym("preset");
  sym_reverb = gensym("reverb");
  sym_chorus = gensym("chorus");
  sym_polyphony = gensym("polyphony");
  sym_nearest = gensym("nearest");
  sym_linear = gensym("linear");
  sym_cubic = gensym("cubic");
  sym_sinc = gensym("sinc");
  
  fluidmax_gen_dict = dictionary_new();
  
  for(i = 0; i < FLUIDMAX_GEN_NUM; i++)
  {
    const char *name = fluidmax_gen_info[i].name;
    
    if(name[0] != '<')
    {
      t_symbol *sym = gensym(name);
      
      fluidmax_gen_symbols[i] = sym;
      dictionary_appendlong(fluidmax_gen_dict, sym, i);
    }
  }
  
//  post("fluidsynth~, %d/%d (%d), based on FluidSynth 1.0.8", EXT_VERSION_DATE_MONTH, EXT_VERSION_DATE_YEAR, EXT_VERSION_NUM);
  post("  FluidSynth is written by Peter Hanappe et al. (see fluidsynth.org)");
  post("  Max/MSP integration by Norbert Schnell, IRCAM - Centre Pompidou");
  
  class_register(CLASS_BOX, c);
  fluidmax_class = c;

  return 0;
}
