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

// ObjC includes
#import <Cocoa/Cocoa.h>
#import <objc/objc.h>
#import <objc/objc-runtime.h>

// Max includes
#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"
#include "commonsyms.h"
//#include "commonsyms_max4"
//#include "ext_common.h"

@interface MaxObject : NSObject 
{
	@public
		// maxobject must be public because of use in the c-interfaced attr accessors
		t_object	*maxObjectBridge;
		long		inletNum;								// proxy input identification
		t_hashtab	*inletNameIndex;
		t_hashtab	*outletNameIndex;
		long		numInlets;
		long		numOutlets;

	@private
		void		**inlets;
		t_symbol	**inletNames;
		char		**inletAssistance;

		void		**outlets;
		t_symbol	**outletNames;
		char		**outletAssistance;
}

+ (t_class *)	createMaxClassWithName:		(char*)maxclassname fromObjcClassWithName:(char*)objcclassname;
- (id)			initWithObject:				(t_object *)x name:(t_symbol *)s numArgs:(long)argc andValues:(t_atom *)argv;
- (t_max_err)	createInletWithIndex:		(long)indexNum named:(char*)cname withAssistanceMessage:(char*)assist;
- (t_max_err)	createOutletWithIndex:		(long)indexNum named:(char*)cname withAssistanceMessage:(char*)assist;
- (void)		setInletPointer:			(void*)ptr forIndex:(long)inletNum;
- (void)		setOutletPointer:			(void*)ptr forIndex:(long)inletNum;
- (t_object *)	getInletPointerForIndex:	(long)indexNum;
- (t_object *)	getOutletPointerForIndex:	(long)indexNum;
- (t_symbol *)	getNameForInletAtIndex:		(long)indexNum;
- (t_symbol *)	getNameForOutletAtIndex:	(long)indexNum;

- (t_max_err)   postInit;
- (t_max_err)   preFree;

- (t_max_err)	sendList:       (t_symbol *)s	withNumArgs:(long)argc andValues:(t_atom *)argv toOutlet:(long)indexNum;
- (t_max_err)	sendMessage:	(t_symbol *)s	withNumArgs:(long)argc andValues:(t_atom *)argv toOutlet:(long)indexNum;
- (t_max_err)	sendMessage:	(t_symbol *)s 	withNumArgs:(long)argc andValues:(t_atom *)argv toOutletNamed:(t_symbol *)outletName;
- (t_max_err)	sendMessage:	(t_symbol *)s	toOutlet:(long)indexNum;
- (t_max_err)	sendMessage:	(t_symbol *)s	toOutletNamed:(t_symbol *)outletName;
- (t_max_err)	sendInt:		(long)value		toOutlet:(long)indexNum;
- (t_max_err)	sendInt:		(long)value		toOutletNamed:(t_symbol*)outletName;
- (t_max_err)	sendFloat:		(double)value	toOutlet:(long)indexNum;
- (t_max_err)	sendFloat:		(double)value	toOutletNamed:(t_symbol*)outletName;

- (t_max_err)	assistanceMessage:			(long)msg forIndex:(long)arg withResultingString:(char*)dst;
- (void)		setLong:					(long)value forKey:(NSString *)key;
- (long)		longForKey:					(NSString *)key;
- (void)		setFloat:					(float)value forKey:(NSString *)key;
- (float)		floatForKey:				(NSString *)key;
- (void)		setSymbol:					(t_symbol*)value forKey:(NSString *)key;
- (t_symbol *)	symbolForKey:				(NSString *)key;

- (void) postMessage:(char*)string, ...;
- (void) postWarning:(char*)string, ...;
- (void) postError:(char*)string, ...;

@end
