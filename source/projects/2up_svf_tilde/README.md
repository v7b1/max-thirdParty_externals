# 2up_svf_64bit-version
a 64-bit version of Randy Jones' 2up_svf~ (state-variable filter) for Max/MSP

This is what Randy Jones (http://madronalabs.com) said on a MaxMSP forum entry in 2007 about it:
> Awhile back (2001?) I was looking for nice sounding filters for MSP and ran across the SVF algorithm described by Hal Chamberlin. I implemented a version of this algorithm with 2x oversampling and added a one-pole filter which seemed to beef up the bass at high Q somewhat. I think jhno took out some of my weird additions to make a less CPU-intensive version for the MSP distribution. Also 2up_svf~ samples its Q and frequency control signals, if connected, every 16 samples rather than every signal vector. 16 samples was the most I could get away with and still make a smooth "q- tip" or analog kick type noise.





This is an update of the original code to the latest MaxSDK and consequently to a true 64 bit version.

Volker Böhm
http://vboehm.net

