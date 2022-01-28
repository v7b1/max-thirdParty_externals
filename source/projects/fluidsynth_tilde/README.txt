******************************************************************
*
*  This is FluidSynth for Max, a derivative of the Fluid Synth 
*  soundfont synthesizer for Max.
*  This code departs from the FluidSynth sources 1.0.8.
*  It is reduced/modified to what is strictly necessary 
*  for implementing a Max external.
* 
*  Fluid Synth is written by Peter Hanappe et al.
*  See http://www.fluidsynth.org/.
* 
*  Max integration by Norbert Schnell, IRCAM - Centre Pompidou
*  Norbert.Schnell@ircam.fr
*

******************************************************************
*
* License
*

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.
 
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

******************************************************************
*
* Building On Mac OS X
*

The directory "build/xcode" contains an Xcode project to build "fluidsynth~.mxo".
This Xcode project requires the following directories on the same level as FluidSynth for Max (directory of this README.txt). 

 - "common/maxmix/maxapi/max6.1", that contains the Max 6.1 SDK
 - "build-max", where the "fluidsynth~.mxo" binary is installed

******************************************************************
*
* FluidSynth 1.0.8 Acknowledgements
*

We would like to thank the Fondation Daniel Langlois for their funding. Their help made this project to get of the ground. Without it would simply not exist. Many thanks!
(http://www.fondation-langlois.org)

In alphabetic order:

Paul Barton-Davis
Samuel Bianchini <biank at online dot fr>
Raoul Bonisch <jkl345 at gmx dot net>
Rui Nuno Capela <rncbc at rncbc dot org>
Jake Commander <jakec at ukfirst dot co dot uk>
Francois Dechelle <Francois dot Dechelle at ircam dot fr>
Ken Ellinwood <kellinwood at yahoo dot com>
Tim Goetze <tim at quitte dot de>
Anthony Green <green at redhat dot com>
Josh Green <jgreen at users dot sourceforge dot net>
Bob Ham <bob at ham dot org> 
Peter Hanappe <peter at hanappe dot com>
Jezar <jezar at dreampoint dot co dot uk>
Fernando Pablo Lopez-Lezcano <nando at ccrma dot Stanford dot EDU>
Johnathan Lee <jlee at music dot columbia dot edu>
Stephane Letz <letz at grame dot fr>
Ebrahim Mayat <ebmayat at mac dot com>
Juergen Mueller
Markus Nentwig <nentwig at users dot sourceforge dot net>
David Olofson <david at olofson dot net>
Sergey Pavlishin <xlogo at mail dot ru>
Dave Phillips
Daniel Pressnitzer <pressnit at ircam dot fr>
Gerald Pye <mail at edenyard dot co dot uk>
Norbert Schnell <Norbert dot Schnell at ircam dot fr>
Joshua Scholar
Antoine Schmitt <as at gratin dot org>
Werner Schweer <ws at seh dot de>
Stephan Tassart <Stephan dot Tassart at st dot com>
Martin Uddén <nanook at lysator dot liu dot se>
