ObjectiveMax

Framework for creating Max objects using Objective-C
Timothy Place
Electrotap, LLC

______________________________________________________


INTRODUCTION
ObjectiveMax is an open-source framework for creating Max objects using Objective-C.
It is used in commercial products such as Electrotap's Tap.Tools.


FEATURES
  * Write objects in Objective-C, and have them automatically wrapped as Max externals.
  * Max 5 ready - there are no incompatibilities with the latest version of Cycling '74's Max
  * Easy way to gain full access to Apple's Cocoa API's
  * The easiest way to write Max externals on the Mac


REQUIREMENTS
	Max 5.0.6 or newer
	MacOS 10.5.x


LICENSE
ObjectiveMax is licensed under the GNU GPL.  See the LICENSE.txt file in this distribution.  
Other licensing arrangements are available by contacting support@electrotap.com.


DIRECTORIES
	MaxObject			--	The source code for the MaxObject framework
	MaxObject.framework	--	An already compiled MaxObject framework for the Mac
	MaxObject-Examples 	--	Example Max externals written in Objective-C


HOW TO BUILD OBJECTS
	1. Copy the c74support folder from the Max 5 SDK into the same folder as this ReadMe file.
	2. Build the framework from MaxObject.xcodeproj 
	   and copy the build MaxObject.framework to /Library/Frameworks
	3. Open the Xcode project for an object in MaxObject-Examples
	4. Click the hammer icon or choose 'Build' from the menus
	5. The external will need to be copied/added into the Max searchpath

