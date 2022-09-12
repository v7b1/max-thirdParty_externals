/*
	Copyright (c) 2009, 74 Objects LLC.
	
	Redistribution and use in source and binary forms, with or without 
	modification, are permitted provided that the following conditions are met:
	
	 1. Redistributions of source code must retain the above copyright notice, 
	    this list of conditions and the following disclaimer.
	 2. Redistributions in binary form must reproduce the above copyright notice,
	    this list of conditions and the following disclaimer in the documentation
	    and/or other materials provided with the distribution.
	 3. Neither the name of ObjectiveMax nor the names of its contributors may be
	    used to endorse or promote products derived from this software without
	    specific prior written permission.
	
	THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
	WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
	MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
	EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
	OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
	WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
	OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
	ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#import "MaxAudioSignal.h"


@implementation MaxAudioSignal

//	TTAudioSignal
//	Wrapper for passing N-channels of audio vectors 
//	Copyright © 2007 by Timothy A. Place
//	License: GNU LGPL

// TODO: The old tt audio signal could point to external memory, or allocate its own for the vectors
// This enum was used to keep trac of which was the case:
// enum selectors{
//	k_mode_local = 1,
//	k_mode_external = 0,
//};


- (id) initWithMaxNumChannels:(long)howMany
{
	[super init];
	self->maxNumChannels = howMany;
	vectors = (float **)malloc(sizeof(float *) * howMany);
	//TODO: should I need to zero the vectors here?  probably...
	return self;
}


- (void) dealloc
{
	free(vectors);
	[super dealloc];
}


- (long) setSamplesForChannel:(long)channel withVector:(float *)newVector
{
	// could check against maxnumchannels here
	vectors[channel] = newVector;
	return 0;
}


+ (short) GetMinNumChannelsForASignal:(MaxAudioSignal*)signal1 andAnotherSignal:(MaxAudioSignal*)signal2
{
	if(signal1->numChannels > signal2->numChannels)
		return signal2->numChannels;
	else
		return signal1->numChannels;
}


@end
