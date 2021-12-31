/* FluidSynth - A Software Synthesizer
 *
 * Copyright (C) 2003  Peter Hanappe and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307, USA
 */

#ifndef _FLUIDSYNTH_MIDI_H
#define _FLUIDSYNTH_MIDI_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 
 MIDI event tables
 */

enum fluid_midi_control_change {
  BANK_SELECT_MSB = 0x00,
  MODULATION_MSB = 0x01,
  BREATH_MSB = 0x02,
  FOOT_MSB = 0x04,
  PORTAMENTO_TIME_MSB = 0x05,
  DATA_ENTRY_MSB = 0x06,
  VOLUME_MSB = 0x07,
  BALANCE_MSB = 0x08,
  PAN_MSB = 0x0A,
  EXPRESSION_MSB = 0x0B,
  EFFECTS1_MSB = 0x0C,
  EFFECTS2_MSB = 0x0D,
  GPC1_MSB = 0x10, /* general purpose controller */
  GPC2_MSB = 0x11,
  GPC3_MSB = 0x12,
  GPC4_MSB = 0x13,
  BANK_SELECT_LSB = 0x20,
  MODULATION_WHEEL_LSB = 0x21,
  BREATH_LSB = 0x22,
  FOOT_LSB = 0x24,
  PORTAMENTO_TIME_LSB = 0x25,
  DATA_ENTRY_LSB = 0x26,
  VOLUME_LSB = 0x27,
  BALANCE_LSB = 0x28,
  PAN_LSB = 0x2A,
  EXPRESSION_LSB = 0x2B,
  EFFECTS1_LSB = 0x2C,
  EFFECTS2_LSB = 0x2D,
  GPC1_LSB = 0x30,
  GPC2_LSB = 0x31,
  GPC3_LSB = 0x32,
  GPC4_LSB = 0x33,
  SUSTAIN_SWITCH = 0x40,
  PORTAMENTO_SWITCH = 0x41,
  SOSTENUTO_SWITCH = 0x42,
  SOFT_PEDAL_SWITCH = 0x43,
  LEGATO_SWITCH = 0x45,
  HOLD2_SWITCH = 0x45,
  SOUND_CTRL1 = 0x46,
  SOUND_CTRL2 = 0x47,
  SOUND_CTRL3 = 0x48,
  SOUND_CTRL4 = 0x49,
  SOUND_CTRL5 = 0x4A,
  SOUND_CTRL6 = 0x4B,
  SOUND_CTRL7 = 0x4C,
  SOUND_CTRL8 = 0x4D,
  SOUND_CTRL9 = 0x4E,
  SOUND_CTRL10 = 0x4F,
  GPC5 = 0x50,
  GPC6 = 0x51,
  GPC7 = 0x52,
  GPC8 = 0x53,
  PORTAMENTO_CTRL = 0x54,
  EFFECTS_DEPTH1 = 0x5B,
  EFFECTS_DEPTH2 = 0x5C,
  EFFECTS_DEPTH3 = 0x5D,
  EFFECTS_DEPTH4 = 0x5E,
  EFFECTS_DEPTH5 = 0x5F,
  DATA_ENTRY_INCR = 0x60,
  DATA_ENTRY_DECR = 0x61,
  NRPN_LSB = 0x62,
  NRPN_MSB = 0x63,
  RPN_LSB = 0x64,
  RPN_MSB = 0x65,
  ALL_SOUND_OFF = 0x78,
  ALL_CTRL_OFF = 0x79,
  LOCAL_CONTROL = 0x7A,
  ALL_NOTES_OFF = 0x7B,
  OMNI_OFF = 0x7C,
  OMNI_ON = 0x7D,
  POLY_OFF = 0x7E,
  POLY_ON = 0x7F
};

/* General MIDI RPN event numbers (LSB, MSB = 0) */
enum midi_rpn_event {
  RPN_PITCH_BEND_RANGE = 0x00,
  RPN_CHANNEL_FINE_TUNE = 0x01,
  RPN_CHANNEL_COARSE_TUNE = 0x02,
  RPN_TUNING_PROGRAM_CHANGE = 0x03,
  RPN_TUNING_BANK_SELECT = 0x04,
  RPN_MODULATION_DEPTH_RANGE = 0x05
};

#ifdef __cplusplus
}
#endif
#endif /* _FLUIDSYNTH_MOD_H */

