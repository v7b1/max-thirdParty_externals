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

#import "MaxObject.h"


/*!
	@class		TTAudioSignal
	@abstract	An object containing a set of audio vectors.
	@discussion This object serves as the primary mechanism for passing audio between TTBlue objects.  
				It contains a set of audio vectors, specifically pointers to TTAudioSample values.
				
				Because this code is used in audio-contexts, with high-performance demands, all members
				are made public for direct access in audio processing loops.
*/
@interface MaxAudioSignal : NSObject {
	@public
		/*! @var sr 			Sample Rate for this signal.  Every channel in a signal must have the same sample-rate */
		long			sr;

		/*! @var vs 			Vector Size for this signal.  Every channel in a signal must have the same vector-size. */
		long			vs;

		/*! @var maxNumChannels	The number of audio channels for which memory has been allocated. */	
		short			maxNumChannels;

		/*! @var numChannels	The number of audio channels that have valid sample values stored in them. */
		short			numChannels;

		/*! @var vectors		An array of pointers to the first sample in each vector */
		float			**vectors;
}

/*!
	@method 	initWithMaxNumChannels:
	@abstract	Initialize the object, defining a maximum number of audio channels that can be used.
	@discussion	This method allocates memory for the audio vectors and sets maxNumChannels accordingly.
	@param		howMany			The maximum number of audio channels that this signal will be able to represent.
	@result		The instance pointer (id) for this object.
*/
- (id)			initWithMaxNumChannels:(long)howMany;

/*!
	@method 	setSamplesForChannel:withVector:
	@abstract	Assigns a vector of sample values to a channel in this signal.
	@discussion	The vector member of this class simply holds a pointer, not a copy of the data.  This makes the 
				operation of this method (and others) fast, but also means that care should be taken to ensure
				that the data being pointed to by this signal is valid, and does not become invalid during the
				lifetime of the signal.
				
				It is the responsibility of the user of this method to ensure that the sample-rate and vector-size
				are also set correctly.
	@param		channel			The channel number (zero-based) to assign the vector to.
	@param		vector			A pointer to the first sample in a vector of samples.
	@result		An error code.
*/
- (long)		setSamplesForChannel:(long)channel withVector:(float *)newVector;

/*!
	@method 	GetMinNumChannelsForASignal:andAnotherSignal:
	@abstract	Use this class method to determine the least number of channels the two signals have in common.
	@discussion	In cases where a processAudio method expects to have a matching number of audio inputs and outputs,
				this method can be used to compare the two signals and return the number of channels for which
				it is safe to assume that the number of inputs and outputs are the same.
	@param		signal1			The first of the two signals to be compared.
	@param		signal2			The second of the two signals to be compared.
	@result		The number of channels that are valid for both signal1 and signal2.
*/
+ (short)		GetMinNumChannelsForASignal:(MaxAudioSignal*)signal1 andAnotherSignal:(MaxAudioSignal*)signal2;

@end
