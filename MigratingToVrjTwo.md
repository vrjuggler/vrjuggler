# Porting from VR Juggler 1.0 to 2.0 #

## Introduction ##

Much has changed from VR Juggler 1.0 to 2.0. Why? Our user base has been increasing steadily, and we wanted to put in changes that would make VR Juggler last for years to come. Many of the warts and bugs in the old version have been fixed, and the interface to everything has been changed to support some of our future plans. Don't worry, this change has been made for the best, and a change this big will not happen again.  Our system is more modular and reusable than before. Thanks for your patience, and let us know if you have any porting questions not answered
here.

### Summary of Changes ###

#### Separation of subsystems into modules with namespaces ####

To help make VR Juggler more modular, each of the major subsystems of VR Juggler has been split into a separate module. Each of these modules has an associated C++ namespace for all of the symbols in the module. For example, the new [Gadgeteer](http://www.vrjuggler.org/gadgeteer/) module (which handles all of the device interfaces you might use for a VR application) has the namespace `gadget`. Thus all symbols in Gadgeteer must be accessed using the `gadget::` scoping prefix.

Because of the move to namespaces, the "vj" prefix from all VR Juggler 1.0 class names has been removed and replaced with the namespace of the module that contains the given class. For example, `vjKernel` has moved to the VR Juggler module which defines the `vrj` namespace. So `vjKernel` is now `vrj::Kernel`.

#### Cross-Platform System Abstraction ####
VR Juggler now has a common module called [VPR](http://www.vrjuggler.org/vapor/) (VR Juggler Portable Runtime) that captures all platform-specific system abstractions.  This includes programming constructs such as threads and semaphores and system access methods such as serial ports and sockets. This common abstraction makes it possible for VR Juggler applications to run on all supported platforms with little or no platform-specific code.

#### Removal of `Juggler_utils` Library ####
As a result of the separation of VR Juggler into multiple subsystems, the standalone `Juggler_utils` library became unnecessary. In VR Juggler 1.0 and early versions of VR Juggler 1.1, this library contained a lot of standalone "utility" code that was needed by the other VR Juggler libraries. The separationinto subsystems resulted in most of the utility code migrating to the VPR (Vapor) module mentioned before.

#### New Math Library ####
VR Juggler introduces the use of a new common math library called [GMTL](http://ggt.sourceforge.net/).

## Interface Changes ##
If you've looked at the VR Juggler 2.0, you'll notice that much has changed since 1.0. Of particular note is the directory structure. In the source code, you'll see many standalone modules where before they were mixed. The installation of Juggler has more directories in the include directory (=vrj=, `snx`, `tweek`, `vpr`, `gadget`, and `jccl`). Below is a step-by-step list of the things you will need to port and how to do it.

### Modules and Namespaces ###
In VR Juggler 1.0, every object that was part of the Juggler API started with the prefix "vj". Since VR Juggler 2.0 has been broken up into distinct functional modules, there needed to be a way to differentiate between their respective objects. All of the classes and objects that used to start with "vj" now instead have a namespace designating which module they belong to.

But what exactly is a namespace? In short, it is a mechanism in C++ that defines a new scope and helps to avoid naming collisions. For instance, you could define two classes both called `Foo`, put them in different namespaces, and use them both in the same program. Say you have them in namespaces `spaceOne` and `spaceTwo`. Then you could reference the different classes with `spaceOne::Foo` and `spaceTwo::Foo`.

For the purposes of VR Juggler 2.0, namespaces are used as a way to make it clear which module an object belongs to. As stated before, these modules are roughly defined along the lines of their functionality:

  * [VR Juggler Portable Runtime](http://www.vrjuggler.org/vapor/) (namespace `vpr`): system-level functionality (threading and synchronization) and other utilities
  * [Juggler Configuration Library](http://www.vrjuggler.org/jccl/) (namespace `jccl`): system and application configuration
  * [Gadgeteer](http://www.vrjuggler.org/gadgeteer/) (namespace `gadget`): device input and management
  * [VR Juggler](http://www.vrjuggler.org/vrjuggler/) (namespace `vrj`): VR development platform that ties everything together
  * [Generic Math Template Library](http://ggt.sourceforge.net/) (namespace `gmtl`): general-purpose math library
  * [Tweek](http://www.vrjuggler.org/tweek/) (namespace `tweek`): distributed Model-View-Controller system that handles GUI communication with a remote C++ application
  * [Sonix](http://www.vrjuggler.org/sonix/) (namespace `snx`): audio library

To create and run VR Juggler applications, you only need to use the first five modules listed. The others are optional for VR Juggler 2.0 but can be extremely useful depending on your needs. These modules can also be used independent of VR Juggler for purposes other than VR applications.

Now you know about namespaces, but how does that help you port from 1.0 to 2.0? Well, you'll find that most of VR Juggler's "vj" classes you used in 1.0 haven't changed much at all other than getting put into a module's namespace. Here are some common changes made when porting 1.0
applications to VR Juggler 2.0:

```
//Declaring input device objects in 1.0
vjPosInterface posInt;
vjDigitalInterface button1;

//Declaring input device objects in 2.0
gadget::PositionInterface posInt;
gadget::DigitalInterface button1;

//Initializing devices in 1.0
posInt.init("VJHead");
button1.init("VJButton");

//Initializing devices in 2.0 (no change)
posInt.init("VJHead");

button1.init("VJButton");

//Checking position device values in 1.0
vjMatrix* mat = posInt.getData();

//Checking position device values in 2.0
gmtl::Matrix44f mat = posInt.getData();

//Checking digital device values in 1.0
if (button1.getData() == vjDigital::ON) { ... }

//Checking digital device values in 2.0
if (button1.getData() == gadget::Digital::ON) { ... }

//Declaring math objects in 1.0
vjMatrix mat;
vjVec3 vec;
vjQuat q;

//Declaring math objects in 2.0
//(See section below for more info on GMTL)
gmtl::Matrix44f mat;
gmtl::Vec3f vec;
gmtl::Quatf q;

//Passing a Juggler 1.0 matrix to an OpenGL function call
vjMatrix mat;
 ... //Do stuff to mat
glMultMatrixf(mat.getFloatPtr());

//Passing a Juggler 2.0 matrix to an OpenGL function call
gmtl::Matrix44f mat;
 ... //Do stuff to mat
glMultMatrixf(mat.getData());
```

This is by no means a complete list of changes necessary for every VR application. Read on for module-specific information regarding their header files and other porting changes.

### Base Application Object ###
In writing applications, the names of the application object parent classes have changed. For example, VR Juggler 1.0 applications that derived from `vjPfApp` now derive from `vrj::PfApp`.  This is shown in the following:

```
#include <vrj/Kernel/Kernel.h>
#include <vrj/Draw/OGL/PfApp.h>

// my OpenGL Performer VR Juggler application object
class myApplication : public vrj::PfApp
{
};
```

Similarly, !OpenGL applications that derived from `vjGlApp` now derive from `vrj::GlApp`:

```
#include <vrj/Kernel/Kernel.h>
#include <vrj/Draw/OGL/GlApp.h>

// my OpenGL VR Juggler application object
class myApplication : public vrj::GlApp
{
};
```

When linking, the library names have changed, and more options are required because there are more libraries in VR Juggler 2.x than there were in VR JUggler 1.0.  The libraries are the following:

  * `vrj`: The base VR Juggler library.
  * `vrj_ogl`: The VR Juggler !OpenGL Draw Manager
  * `vrj_pf`: The VR Juggler Performer Draw Manager
  * `gadget`: Gadgeteer
  * `jccl`: JCCL
  * `vpr`: VPR

For a simpler makefile, the following can be done:

```
app: $(OBJECTS)
	$(LINK) app $(OBJECTS) \ 
			  `$(VJ_BASE_DIR)/bin/vrjuggler-config --libs` \ 
			  `$(VJ_BASE_DIR)/bin/vrjuggler-config --extra-libs`
```

The most important aspect of the above is the use of the **vrjuggler-config** script. The `--libs` argument prints out the basic VR Juggler libraries including paths. The option `--extra-libs` prints out all the dependencies of VR Juggler.

For Win32 users, all the sample and test applications have Visual C++ project files. The above will work using GNU make in a Cygwin environment.

For much more detailed information, interested developers can refer to the headers `vrj/Draw/Pf/PfApp.h`, `vrj/Draw/OGL/GlApp.h`, `vrj/Draw/OSG/OsgApp.h`, `gadget/Type/PositionInterface.h`, `gadget/Type/DigitalInterface.h`, and `vrj/Kernel/Kernel.h`.

### Application Drawing Pipeline ###
To allow the user more control over the synchronization of applications, we have introduced the `bufferPreDraw()` function in these derived application classes: `vrj::GlApp`, `vrj::OpenSGApp`, and `vrj::OsgApp`. This function is executed once per graphics buffer each frame. For instance, if you are running in stereo, you have buffers for both the right and left eyes. You also have multiple buffers when rendering to multiple windows or surface displays.

In your VR Juggler 1.0 !OpenGL applications, you most likely had the draw function clear the graphics window and render the scene. The code was probably similar to this:

```
// my VR Juggler 1.0 OpenGL application object
class myApplication : public vjGlApp
{
	...
	void draw()
	{
		//Clear the window
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Render the scene
		...
	}
	...
};
```

In the new version of VR Juggler, you will want to move the color buffer clearing out of the draw function and into the `bufferPreDraw()` method:

```
// my VR Juggler 2.0 OpenGL application object
class myApplication : public vrj::GlApp
{
	...
	void bufferPreDraw()
	{
		// Clear the window.
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void draw()
	{
		// Clear the depth buffer.
		glClear(GL_DEPTH_BUFFER_BIT);

		// Render the scene...
		...
	}
	...
};
```

Why can't you just clear both the depth and color buffers in `bufferPreDraw()`?

Because when rendering stereo views, !OpenGL uses two color buffers (one for each eye) but a single depth buffer is shared between the left and right eye views. The drawing pipeline happens in this order: `bufferPreDraw()` for left eye, `bufferPreDraw()` for right eye, `draw()` for left eye, `draw()` for right eye. Because the depth buffer is shared, the objects drawn for the left eye are carried over into the right eye rendering and much of your scene can get blocked out for the right eye's view.

Why can't you just clear both the depth and color buffers in `draw()`?

Because when rendering multiple viewports within a single window, the `glClear()` command affects the entire window and not just the current viewport that you're trying to render. So if you clear the color buffer during `draw()`, all of the previously drawn viewports are cleared and you end up with just a single viewport being visible.

### Math Library ###
VR Juggler now uses the [GMTL math library](http://ggt.sourceforge.net/). The old vjMath library is no longer available. There are some very fundamental changes between GMTL and Math from VR Juggler 1.0. You should definitely read the GMTL Programmer's Guide and FAQ, both of which are available from http://ggt.sourceforge.net/

The main difference between GMTL and VR Juggler 1.0 Math is the way you use methods. GMTL is similar to object-oriented C programming where the object is always the first parameter in the function. This difference from VR Juggler 1.0 Math will affect all application code using math.

Another big difference is that GMTL uses radian-based rotations, where VRJ 1.0 Math used degrees. Fortunately GMTL provides conversion functions to allow you to work with rotational data in either notation.

The `vjMatrix`, `vjVec3`, and `vjQuat` classes that you're used to have been changed to `gmtl::Matrix44f`, `gmtl::Vec3f`, `gmtl::Point3f`, and `gmtl::Quatf` respectively. The following shows an example of using the math classes.

```
#include <gmtl/Matrix.h>
#include <gmtl/Quat.h>
#include <gmtl/Vec.h>
#include <gmtl/Generate.h>

// make a rotation quaternion from an axis angle
gmtl::Quatf q;
gmtl::setRot(q, gmtl::AxisAnglef(gmtl::Math::deg2Rad(90.0f), 0.0f, 1.0f, 0.0f));

// make a rotation quaternion from Euler angles
gmtl::setRot(q, gmtl::EulerAngleXYZf(0.0f, gmtl::Math::deg2Rad(90.0f), 0.0f));

// set the rotation part of the matrix to the rotation defined by the quaternion
gmtl::Matrix44f mat;
gmtl::setRot(mat, q);

// set the translation of the matrix
gmtl::Vec3f vec(1.0f, 3.0f, -5.0f);
gmtl::setTrans(mat, vec);

// set the matrix to the rotation defined by the quaternion (eliminates any other transform in the mat)
gmtl::Matrix44f rot_mat;
gmtl::set(rot_mat, q);

// transform the vector (the mathematical definition of Mat * Vec is that it only does rotation)
gmtl::Vec3f result_v = mat * vec;

// transform a point (the mathematical definition of Mat * Pnt is that does a full transform)
gmtl::Point3f result_p = mat * point;

// init the matrix using the quaternion
gmtl::Matrix44f rot_mat(gmtl::make<gmtl::Matrix44f>(q));

// get the length of the vector
float length = gmtl::length(vec);

// get the cross product
gmtl::Vec3f result_cross = gmtl::cross(vec1, vec2);
```

Common C preprocessor symbols such as `VJ_DEG2RAD` and `VJ_RAD2DEG`, found in `vjConfig.h`, are now located as inline functions in `gmtl/Math.h`. These are listed in the `gmtl::Math` namespace. For example, `VJ_DEG2RAD` is now `gmtl::Math::deg2Rad()`. The following illustrates the new use:

```
#include <gmtl/Math.h>

// convert degrees to radians...
float radians = gmtl::Math::deg2Rad(degrees);
```

The headers are in the directory `gmtl`. Typically you will type your `#include` statements in the following format:

```
#include <gmtl/somefile.h>
```

In order to aid in quick migration from 1.0 to the most recent version of VR Juggler, we have provided several optional math classes that have the same syntax from VR Juggler 1.0. These include the `vrj::Vec3`, `vrj::Matrix`, and `vrj::Quat` classes (which act exactly as `vjVec3`, `vjMatrix`, and `vjQuat` do). The class header files are located in your VR Juggler source tree in `modules/vrjuggler/deprecated/Math`. For a VR Juggler 2.0 installation, they can be found in `$VJ_BASE_DIR/include/deprecated/Math`.  The file names are `Vec3.h`, `Matrix.h`, and `Quat.h`.

  * OTE**: The `deprecated` header tree will not be available for VR Juggler releases beyond the 2.0.x series.  Starting with VR Juggler 2.1/2.2, those headers are removed from the installation tree, so code must be migrated to the newer APIs.**

Using these helper classes makes porting your code much easier.  Instead of converting all of your math code to GMTL, all you have to do is include the header files and make namespace changes to all of your math code. For instance, if you have the following lines of VR Juggler 1.0 code:

```
vjVec3 vec = v1.cross(v2);
vjMatrix mat;
mat.makeTrans(vec);
vjQuat q;
q.makeRot(q);
```

The code ported with the math helper classes would look like this:

```
vrj::Vec3 vec = v1.cross(v2);
vrj::Matrix mat;
mat.makeTrans(vec);
vrj::Quat q;
q.makeRot(q);
```

Note the only thing that changed is the `vrj::` namespace has replaced the old "vj" prefix. It's also important to note that although GMTL uses radians for rotations, these math helper classes use degrees just as in VR Juggler 1.0. Every math function in these classes that was available in 1.0 is also available here.

For more information, refer to the http://ggt.sf.net/ website. It has excellent online reference guide and a programming guide that is more in depth than this short porting tutorial. If you don't have a web connection, refer to the header files in the `gmtl` directory. The header files have all the documentation used to generate the reference guide. Typically the headers you will use are `gmtl/Vec.h`, `gmtl/Point.h`, `gmtl/Matrix.h`, and `gmtl/Quat.h`.

### Threads and Synchronization ###
One major change to VR Juggler 2.0 is the operating system abstraction in Juggler. It has been separated into a standalone, decoupled portable runtime called the VR Juggler Portable Runtime (VPR, pronounced "vapor"). VPR is a C++ abstraction the following:

  * Threads
  * Synchronization primitives
  * Sockets
  * Serial I/O
  * High-precision timers
  * GUIDs
  * I/O statistics collection
  * Thread-safe ostreams with levels, categories, colors, and thread IDs
  * Performance monitoring

It even has simulated sockets (using a discrete-event simulator for sockets). What does this mean for you? What will you need to port?

  * Every place you used a thread in your VR Juggler 1.0 application, you'll need to replace `vjThread` with `vpr::Thread`.
  * Replace `vjMutex` with `vpr::Mutex`.
  * Condition variables are now called `vpr::CondVar` instead of `vjCond`.
  * Replace `vjSemaphore` with `vpr::Semaphore`.
  * `vjDEBUG` is now `vprDEBUG`. Related output symbols (e.g., `vjDEBUG_FLUSH`, `vjDEBUG_BEGIN`, etc.) have been renamed similarly. When calling `vprDEBUG`, the category is now required.  Previously, an integer constant could be used in place of a category symbol. For example, the following shows the use of the category `vprDBG_ALL`:

```
vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "hello world!" << vprDEBUG_FLUSH;
```

  * Related to this change is the location of the debugging output headers. In VR Juggler 1.0, the only debugging header was `Kernel/vjDebug.h`. Now, the basic header that is always needed is `vpr/Util/Debug.h`. Project-specific extensions are named similarly.  For example, the JCCL debugging header is `jccl/Util/Debug.h`. In VR Juggler proper, the header is `vrj/Util/Debug.h`.
  * The VPR C++ portable runtime library is located in `libvpr`, and the headers can be found in `include/vpr`.

For more detailed information, refer to the VPR _Programmer's Reference_.

### Sound ###
Your interface to sound is `snx::SoundHandle`, found in `snx/SoundHandle.h`.

To select your sound API, configure the VR Juggler Sound Manager with configuration elements or directly call the Sonix API as in the following example:

```
snx::sonix::instance()->changeAPI("OpenAL");
```

Here, the string "OpenAL" can be "AudioWorks", "Audiere", or "OpenAL" depending on what Sonix plug-ins are available.

If you want to disable sound, then do the following:

```
snx::sonix::instance()->changeAPI("Stub");
```

The new sound library is located in `libsonix`, and the headers can be found in `include/snx`. See also: `include/snx/SoundHandle.h`, `include/snx/sonix.h`.

### Device Input ###
Header file includes for access to devices in Gadgeteer:

```
#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DigitalInterface.h>
#include <gadget/Type/AnalogInterface.h>
#include <gadget/Type/CommandInterface.h>
#include <gadget/Type/StringInterface.h>
```

Gadgeteer has its own namespace that is separate from VR Juggler.  The namespace is `gadget`, and classes such as `vjAnalogInterface` are now `gadget::AnalogInterface`.

As before, use `init()` on the interface class to tell it which device to use (this is specified in your config files). For example, to test for a button down event, you will use this code:

```
gadget::DigitalInterface mAccelerateButton;
mAccelerateButton.init();
if (mAccelerateButton->getData() == gadget::Digital::TOGGLE_ON)
{
	... do stuff ...
}
```

Gadgeteer is located in `libgadget` and `include/gadget`.

### Position Filters ###
In VR Juggler 1.0 all applications were treated in feet, which caused problems for all our friends outside the United States who use SI units.  Therefore for VR Juggler 2.0 we have switched to meters and added a Position Filter Proxy that allows an application to use any units that it wishes. The addition of position filters causes three potential changes for applications.

The first change is in the configuration of the devices themselves, for these modifications please refer to the VR Juggler _Configuration Guide_ which can help you configure your devices properly.

The second change is that a positional device now returns a copy of its position matrix instead of a reference, as shown here.

```
gadget::PositionInterface posInt;

//Checking position device values in 1.0
vjMatrix* mat = posInt.getData();

//Checking position device values in 2.0
gmtl::Matrix44f mat = posInt.getData();
```

The third change is that an application may want to manually override the default scaling. So this is supported through the `getData()` call as shown here:

```
gadget::PositionInterface posInt;

// Checking position device value's and scaling them by 3.28 which returns 
// the value in feet.
gmtl::Matrix44f mat = posInt.getData(3.28f);
// OR
gmtl::Matrix44f mat = posInt.getData(gadget::PositionUnitConversion::ConvertToFeet);
```

There are 4 built in unit conversions that Juggler supplies they are:

  1. `gadget::PositionUnitConversion::ConvertToFeet`
  1. `gadget::PositionUnitConversion::ConvertToInches`
  1. `gadget::PositionUnitConversion::ConvertToMeters`
  1. `gadget::PositionUnitConversion::ConvertToCentimeters`

### Keyboard/Mouse Devices ###
Gadgeteer 1.0 Beta 1 includes refactored handling of keybard and mouse data.  Plain windows (now referred to as "input windows") and graphics windows feed keyboard and mouse events from the windowing system into keyboard/mouse devices.  User applications can get access to these events through the type `gadget::KeyboardMouseInterface` (formerly `gadget::EventWindowInterface` and `gadget::KeyboardInterface` before that).  VR Juggler 1.0 used the name `vjKeyboardInterface`.

Names of properties and configuration elements have changed as a result of this work. Some XSLT code is available to update VR Juggler 2.0 Alpha 4 configuration files automatically to use the new names. The code is found in the file `$VJ_BASE_DIR/share/vrjuggler/data/xlst/2.0a4-2.0b1.xsl`. To update a configuration file using the **xsltproc** command (installed as part of the [GNOME desktop](http://www.gnome.org/)), run the following command:

```
xsltproc -o output.jconf 2.0a4-2.0b1.xsl input.jconf
```

Other XSLT 1.0 processors such as [Xalan-J](http://xml.apache.org/xalan-j/) or [Saxon](http://saxon.sourceforge.net/) can be used instead of **xsltproc** if it is not available. Consult the documentation for specific tool you use for usage information.

### Configuration System ###
The configuration system in VR Juggler has been separated into a standalone library. This library gives exactly the same features as before. This library has its own namespace: `jccl`. The library is called JCCL, short for Juggler Configuration and Control Library.

The first place you configure VR Juggler is typically in the application `main()` function. JCCL config file are loaded by the kernel. This is exactly the same as before:

```
int main(int argc, char* argv[])
{
	vrj::Kernel* kernel = vrj::Kernel::instance(); // Declare a new Kernel
	for ( int i = 1; i < argc; ++i )
	{
		std::cout << "Loading Config File: " << argv[i] << "\n" << std::flush;
		kernel->loadConfigFile(argv[i]);
	}

	// other init stuff...
}
```

With classes that handle VR Juggler 1.0 config chunks, the usage of `vjConfigChunk*` is as follows:

```
bool myclass::configAdd(vjConfigChunk* chunk)
```

For VR Juggler, `vjConfigChunk*` is called `jccl::ConfigElementPtr`, as shown below.

```
bool myclass::configAdd(jccl::ConfigElementPtr element)
```

To summarize, everything is now in the =jccl:: namespace, config chunks are now called config elements, and a special "smart-pointer" type is used instead of C/C++ pointers.

Once you have a jccl::ConfigElementPtr object, the signature of the `getProperty()` method has changed. In VR Juggler 1.0 code, a property value of type `int` would have been requested as follows:

```
int prop_value = static_cast<int>(chunk->getProperty("someInt"));
```

or as:

```
int prop_value = (int) chunk->getProperty("someInt");
```

In the new API, the desired type is passed as a template parameter, as shown below:

```
int prop_value = element->getProperty<int>("someInt");
```

All the types you would have used with the cast operator including `std::string` and `jccl::ConfigElementPtr` can be used for the template parameter. Other types such as `unsigned int`, `short`, etc. that were not directly supported by the old `vjVarValue` class can be used as well.

Configuration elements no longer have a property called "Name". To get an element's name, use the `getName()` method thusly:

```
std::string name = element->getName();
```

The method `jccl::ConfigChunk::getType()` (or `vjConfigChunk::getType()`) has been replaced with `jccl::ConfigElement::getID()`, which returns a `std::string` object. The value of the returned string corresponds to the token for the configuration element in the definition file. Most existing code will change from the following:

```
std::string type = (std::string) element->getType();
```

to this:

```
std::string type(element->getID());
```

Code that uses `jccl::ConfigManager` (formerly `vjConfigManager`) must be careful in accessing the list of pending configuration elements. Prior to the creation of JCCL, the pending list was locked and unlocked internally for all accesses. Now, the responsibility of protecting access to the pending list has shifted to user-level code. The methods `lockPending()` and `unlockPending()` are provided to lock and unlock the pending list.  Furthermore, the methods that access the pending list have assertions to verify that the list has been locked properly. If your code gets assertion failures similar to the following:

```
Assertion failed: (1 == mPendingLock.test()), file
<some path>/modules/jackal/rtrc/jccl/RTRC/ConfigManager.h, line 214, pid 165370
```

then you need to verify that you lock and unlock the pending list correctly.

The JCCL library is located in `libjccl`, and the headers are in `include/jccl`.

### Configuration Files ###
The configuration files used by VR Juggler have gone through quite a lot of changes since VR Juggler 1.0. The old formats used by description files and configuration files have been replaced completely by a new XML format. This means that the old formats are no longer supported by the Java or C++ code, so conversions must be made externally to [VRJConfig](VrjConfig.md) and VR Juggler before trying to use existing configuration files. In this section, we explain some brief background information regarding the conversion to XML, and we then explain how to update existing configuration files to the latest format.

#### History ####
The configuration files used by VR Juggler have undergone three overhauls since version 1.0. In August 2001, the first pass at converting to XML was made. In the configuration definition files, the complex format was replaced by very straightforward XML. The job done by the definition files was already very hierarchical in nature, so the transition to a standard, inherently hierarchical file format was a natural progression. In the actual configuration files, the changes made were not so elegant. Essentially, the key tokens were replaced by XML elements, but the actual values were left as specially formatted strings that required extra parsing _after_ the XML parser had done its job.

In May 2002, the second version of the XML configuration file formats were devised. Little changed in the definition file format, but the configuration files were redesigned so that no extra parsing step was required. With no user-level parsing required, we are now able to use XSLT stylesheets to process and update files easily. Other critical changes included breaking a circular dependency between the definition file format and the configuration file format. Versioning was also added
to both file formats as part of this change.

In July 2003, the third version fo the XML configuration file and definition file formats were introduced.  These are the final versions for VR Juggler 2.0, and they include many important new features.  The most valuable new feature is config element versioning and automatic element upgrading through VRJConfig.  With the third version, there is now one config definition per `.jdef` file.  Each `.jdef` file can contain multiple versions of its definition, and each version includes XSLT code for migrating config files of the previous version.  VRJConfig uses this XSLT code to perform automatic upgrades of config elements.

#### Configuration Definition File Versions ####
The versions of the configuration definition files are as follows:

  * 1.0: The VR Juggler 1.0 custom file format. This format has no version number information.
  * 1.9: The first iteration on the XML format, in use between August 2001, and May 23, 2002. This format has no version number information.
  * 2.0: The revamped XML format, in use between May 24, 2002, and July 17, 2002. Any definition files created using VjControl during that time period qualify as version 2.0, though they may lack the version number information. If a version number is missing, add the following as the second line of the file:

```
<?org-vrjuggler-jccl-settings desc.db.version="2.0"?>
```

  * 2.1: An iteration on 2.0 that removed some unnecessary complexity in dealing with the number of property values for a given configuration element. This version was in use between July 18, 2002, and January 15, 2003. All files created during this time period should have the following as the second line of the file:

```
<?org-vrjuggler-jccl-settings desc.db.version="2.1"?>
```

  * 2.2: An iteration on 2.1 that improved the way that embedded configuration element types and element pointer types are specified. Instead of overloading the "enumeration" concept, a new element (=allowedType=) was added. This version was in use between January 15, 2003, and February 27, 2003. Note that no developer release snapshots were made during this time. All files created during this short period should have the following as the second line of the file:

```
<?org-vrjuggler-jccl-settings desc.db.version="2.2"?>
```

  * 2.3: An iteration on 2.2 that added version support to individual configuration element definitions. This version was in use between February 27, 2003, and July 10, 2003. This was the format used by VR Juggler 2.0 Alpha 2 and 2.0 Alpha 3. Note that the version information is not used in VR Juggler 2.0 Alpha 1, so updating from the 2.2 format is not strictly required. All files should have the following as the second line of the file:

```
<?org-vrjuggler-jccl-settings desc.db.version="2.3"?>
```

  * 3.0: A complete rewrite of the config definition file format that split the definitions up into separate files and introduced support for definition versioning and automatic upgrades using XSLT.  This version was in use between July 10, 2003, and September 17, 2004. All definition files now must use the file extension `.jdef`. This was the format used by VR Juggler 2.0 Alpha 4. All files must have the following as the second line of the file:

```
<?org-vrjuggler-jccl-settings definition.version="3.0"?>
```

  * 3.l: A minor iteration on the 3.0 file format that introduced the concept of "editable enumerations."  This version has been in use since September 17, 2004.  All files must have the following as the second line of the file:

```
<?org-vrjuggler-jccl-settings definition.version="3.1"?>
```

#### Configuration File Versions ####
The versions of the configuration files are as follows:

  * 1.0: The VR Juggler 1.0 custom file format. This format has no version number information.
  * 1.9: The first iteration on the XML format, in use between August 2001 and May 23, 2002. This format has no version number information.
  * 2.0: The revamped XML format, in use between May 24, 2002, and September 11, 2002. Any configuration files created using VjControl or VRJConfig during that time period qualify as version 2.0, though they may lack the version number information. If a version number is missing, add the following as the second line of the file:

```
<?org-vrjuggler-jccl-settings config.db.version="2.0"?>
```

  * 2.1: An iteration on 2.0 that changed the way external files are included. This version was in use between September 12, 2002, and July 10, 2003.  All files should have the following as the second line of the file:

```
<?org-vrjuggler-jccl-settings config.db.version="2.1"?>
```

  * 3.0: A complete rewrite of the config file format that made use of config element versioning and simplified the handling of included external files. This version has been in use since July 10, 2003, and all config files using this format have the file extension `.jconf`.  This is the file format used for VR Juggler 2.0 Alpha 4 and newer.  All such files must have the following as the second line of the file:

```
<?org-vrjuggler-jccl-settings configuration.version="3.0"?>
```

#### Updating ####
We now explain how to update from one version of a file format toanother. We begin with the first step: updating from the (versionless) VR Juggler 1.0 formats to version 2.0 of the definition file format and version 2.0 of the configuration file format. We then move on to explain how to do iterative updates once you have converted to the base XML format.

##### Migrating Configurations Between VR Juggler 2.0 Pre-Releases #####
Before we describe the generalized process for updating config files and config definitions, there are some higher level updates that must be applied when updating from one VR Juggler 2.0 pre-release to the next.  This will be the process used by all users of a given VR Juggler 2.0 pre-release since 2.0 Alpha 1.

To update from VR Juggler 2.0 Alpha 1 to VR Juggler 2.0 Alpha 2 or Alpha 3, use the file `$VJ_BASE_DIR/share/vrjuggler/data/xslt/2.0a1-2.0a2.xsl` with an XSLT 1.0 processor.  For example, use **xsltproc** as follows:

```
xsltproc -out output.config 2.0a1-2.0a2.xsl input.config
```

This will create a new file, `output.config`, for use with VR Juggler 2.0 Alpha 2 or Alpha 3 based on the 2.0 Alpha 1 input file `input.config`.  Refer to the comments at the top of the file `2.0a1-2.0a2.xsl` (after the copyright) for more details.

To update from VR Juggler 2.0 Alpha 2 or Alpha 3 to VR Juggler 2.0 Alpha 4, use the file `$VJ_BASE_DIR/share/vrjuggler/data/xslt/2.0a2-2.0a4.xsl` with an XSLT 1.0 processor.  For example, use **xsltproc** as follows:

```
xsltproc -out output.jconf 2.0a2-2.0a4.xsl input.config
```

This will create a new file, `output.jconf`, for use with VR Juggler 2.0 Alpha 4 based on the VR Juggler 2.0 Alpha 2 or Alpha 3 input file `input.config`.  Refer to the comments at the top of the file `2.0a2-2.0a4.xsl` (after the copyright) for more details.

To update from VR Juggler 2.0 Alpha 4 to any VR Juggler 2.0 beta release or the final 2.0.0 release, use the file `$VJ_BASE_DIR/share/vrjuggler/data/xslt/2.0a4-2.0b1.xsl` with an XSLT 1.0 processor.  For example, use **xsltproc** as follows:

```
xsltproc -out output.jconf 2.0a4-2.0b1.xsl input.jconf
```

After this, the file `output.jconf` must be loaded into VRJConfig and saved to complete the update process. These steps result in the creation of a new file, `output.jconf`, for use with the VR Juggler 2.0 beta releases, or the final 2.0.0 release, based on the VR Juggler 2.0 Alpha 4 input file `input.jconf`.  Refer to the comments at the top of the file `2.0a4-2.0b1.xsl` (after the copyright) for more details.

Users taking advantage of more advanced configuration features of VR Juggler by defining custom config element types should review the remaining updating descriptions.

##### Version 1.0 to 2.0 (Definition Files and Configuration Files) #####
At this time, there is no automated way to update VR Juggler 1.0 configuration files to the 2.0 format. Extensive changes have been made in the basic structure and the content of the files. It is recommended that users run VRJConfig and create new VR Juggler 2.0 configurations from scratch. This offers a chance to get familiar with the new configuration editor. We hope to offer an automated update mechanism in time for the full VR Juggler 2.0.0 release.

##### Version 1.9 to 2.0 (Configuration Files) #####
A Python 2 script has been written to update from the so-called 1.9 configuration file format version to 2.0. You can find it in `juggler/modules/jackal/tools/xmlupdate` or in `$VJ_BASE_DIR/share/jccl/tools/xmlupdate` depending on whether you have a source or a binary distribution of VR Juggler. The script is called **xmlupdate.py**, and it is run as follows:

```
./xmlupdate.py file1.config file2.config
./xmlupdate.py *.config
```

The updated files will have the same base name as the original file but with the extension `.new`. The original files will not be modified.

  * OTE**: This script cannot process configuration definition files. As of this writing, we have no automated way to update from version 1.9 of the definition file format to version 2.0.**

##### Version 2.0 to 2.1 (Configuration Definition Files) #####
Updating to version 2.1 of the definition file format can be done through the use of an XSLT stylesheet. This stylesheet is found in `juggler/modules/jackal/tools/xmlupdate` or in `$VJ_BASE_DIR/share/jccl/tools/xmlupdate`, depending on whether you have a source or a binary distribution of VR Juggler. The stylesheet is named `desc_2.0-2.1.xsl`, and it can be used with any XSLT 1.0 processor. For example, if you have **xsltproc** available, you can use it to update a
definition file as follows:

```
xsltproc -o output.desc desc_2.0-2.1.xsl input.desc
```

This will generate a new file, `output.desc`, based on `input.desc` that uses version 2.1 of the definition file format.

We also provide a simple makefile in the xmlupdate directory for using this stylesheet with **xsltproc** or with the Xalan command-line utility. The purpose of the makefile is to hide the details of executing the actual commands. To do the above conversion, simply enter the following:

```
make INPUT_FILE=input.desc OUTPUT_FILE=output.desc desc-update-2.0-2.1
```

At this time, the makefile only supports the use of **xsltproc** and the Xalan command-line utility. It defaults to using **xsltproc**. If that tool is not available, line 38 of the makefile so that the value of `$(XSLT_TARGET)` is `update-xalan`. If neither Xalan nor **xsltproc** is available, you will have to use whatever XSLT processor is available on your system.

##### Version 2.1 to 2.2 (Configuration Definition Files) #####
Updating to version 2.2 of the definition file format can be done through the use of an XSLT stylesheet. This stylesheet is found in `juggler/modules/jackal/tools/xmlupdate` or in `$VJ_BASE_DIR/share/jccl/tools/xmlupdate`, depending on whether you have a source or a binary distribution of VR Juggler. The stylesheet is named `desc_2.1-2.2.xsl`, and it can be used with any XSLT 1.0 processor. For example, if you have **xsltproc** available, you can use it to update a
definition file as follows:

```
xsltproc -o output.desc desc_2.1-2.2.xsl input.desc
```

This will generate a new file, `output.desc`, based on `input.desc` that uses version 2.2 of the definition file format.

We also provide a simple makefile in the xmlupdate directory for using this stylesheet with **xsltproc** or with the Xalan command-line utility. The purpose of the makefile is to hide the details of executing the actual commands. To do the above conversion, simply enter the following:

```
make INPUT_FILE=input.desc OUTPUT_FILE=output.desc desc-update-2.1-2.2
```

At this time, the makefile only supports the use of **xsltproc** and the Xalan command-line utility. It defaults to using **xsltproc**. If that tool is not available, line 38 of the makefile so that the value of `$(XSLT_TARGET)` is `update-xalan`. If neither Xalan nor **xsltproc** is available, you will have to use whatever XSLT processor is available on your system.

##### Version 2.2 to 2.3 (Configuration Definition Files) #####
Updating to version 2.3 of the definition file format can be done through the use of an XSLT stylesheet. This stylesheet is found in `juggler/modules/jackal/tools/xmlupdate` or in `$VJ_BASE_DIR/share/jccl/tools/xmlupdate`, depending on whether you have a source or a binary distribution of VR Juggler. The stylesheet is named `desc_2.2-2.3.xsl`, and it can be used with any XSLT 1.0 processor. For example, if you have **xsltproc** available, you can use it to update a
definition file as follows:

```
xsltproc -o output.desc desc_2.2-2.3.xsl input.desc
```

This will generate a new file, `output.desc`, based on `input.desc` that uses version 2.3 of the definition file format.

We also provide a simple makefile in the xmlupdate directory for using this stylesheet with **xsltproc** or with the Xalan command-line utility. The purpose of the makefile is to hide the details of executing the actual commands. To do the above conversion, simply enter the following:

```
make INPUT_FILE=input.desc OUTPUT_FILE=output.desc desc-update-2.2-2.3
```

At this time, the makefile only supports the use of **xsltproc** and the Xalan command-line utility. It defaults to using **xsltproc**. If that tool is not available, line 38 of the makefile so that the value of `$(XSLT_TARGET)` is `update-xalan`. If neither Xalan nor **xsltproc** is available, you will have to use whatever XSLT processor is available on your system.

##### Version 2.3 to 3.0 (Configuration Definition Files) #####
Updating to version 3.0 of the definition file format can be done through the use of an XSLT stylesheet. This stylesheet is found in `juggler/modules/jackal/tools/xmlupdate` or in `$VJ_BASE_DIR/share/jccl/tools/xmlupdate`, depending on whether you have a source or a binary distribution of VR Juggler. The stylesheet is named `desc_2.3-3.0.xsl`, and it can be used with any XSLT 1.0 processor. For example, if you have **xsltproc** available, you can use it to update a
definition file as follows:

```
xsltproc -o junk desc_2.3-3.0.xsl input.desc
```

This will generate a collection of `.jdef` files (one for each config definition in `input.desc`) in the subdirectory `definitions`.  These new files will use version 3.0 of the definition file format.

We also provide a simple makefile in the xmlupdate directory for using this stylesheet with **xsltproc** or with the Xalan command-line utility. The purpose of the makefile is to hide the details of executing the actual commands. To do the above conversion, simply enter the following:

```
make INPUT_FILE=input.desc OUTPUT_FILE=output.desc desc-update-2.3-3.0
```

At this time, the makefile only supports the use of **xsltproc** and the Xalan command-line utility. It defaults to using **xsltproc**. If that tool is not available, line 38 of the makefile so that the value of `$(XSLT_TARGET)` is `update-xalan`. If neither Xalan nor **xsltproc** is available, you will have to use whatever XSLT processor is available on your system.

##### Version 3.0 to 3.1 (Configuration Definition Files) #####
Updating to version 3.1 of the definition file format can be done through the use of an XSLT stylesheet. This stylesheet is found in `juggler/modules/jackal/tools/xmlupdate` or in `$VJ_BASE_DIR/share/jccl/tools/xmlupdate`, depending on whether you have a source or a binary distribution of VR Juggler. The stylesheet is named `jdef_3.0-3.1.xsl`, and it can be used with any XSLT 1.0 processor. For example, if you have **xsltproc** available, you can use it to update a
definition file as follows:

```
xsltproc -o output.jdef jdef_3.0-3.1.xsl input.jdef
```

This will generate a new file, `output.jdef`, based on `input.jdef` that uses version 3.0 of the definition file format.

We also provide a simple makefile in the xmlupdate directory for using this stylesheet with **xsltproc** or with the Xalan command-line utility. The purpose of the makefile is to hide the details of executing the actual commands. To do the above conversion, simply enter the following:

```
make INPUT_FILE=input.desc OUTPUT_FILE=output.desc jdef-update-3.0-3.1
```

At this time, the makefile only supports the use of **xsltproc** and the Xalan command-line utility. It defaults to using **xsltproc**. If that tool is not available, line 38 of the makefile so that the value of `$(XSLT_TARGET)` is `update-xalan`. If neither Xalan nor **xsltproc** is available, you will have to use whatever XSLT processor is available on your system.

##### Version 2.0 to 2.1 (Configuration Files) #####
Updating to version 2.1 of the configuration file format can be done through the use of an XSLT stylesheet. This stylesheet is found in `juggler/modules/jackal/tools/xmlupdate` or in
=$VJ\_BASE\_DIR/share/jccl/tools/xmlupdate=, depending on whether you have a source or a binary distribution of VR Juggler. The stylesheet is named `cfg_2.0-2.1.xsl`, and it can be used with any XSLT 1.0 processor. For example, if you have **xsltproc** available, you can use it to update a definition file as follows:

```
xsltproc -o output.config cfg_2.0-2.1.xsl input.config
```

This will generate a new file, `output.config`, based on `input.config` that uses version 2.1 of the definition file format.

We also provide a simple makefile in the xmlupdate directory for using this stylesheet with **xsltproc** or with the Xalan command-line utility. The purpose of the makefile is to hide the details of executing the actual commands. To do the above conversion, simply enter the following:

```
make INPUT_FILE=input.config OUTPUT_FILE=output.config cfg-update-2.0-2.1
```

At this time, the makefile only supports the use of **xsltproc** and the Xalan command-line utility. It defaults to using **xsltproc**. If that tool is not available, line 38 of the makefile so that the value of `$(XSLT_TARGET)` is `update-xalan`. If neither Xalan nor **xsltproc** is available, you will have to use whatever XSLT processor is available on your system.

##### Version 2.1 to 3.0 (Configuration Files) #####
Updating to version 3.0 of the configuration file format can be done through the use of an XSLT stylesheet. This stylesheet is found in `juggler/modules/jackal/tools/xmlupdate` or in
=$VJ\_BASE\_DIR/share/jccl/tools/xmlupdate=, depending on whether you have a source or a binary distribution of VR Juggler. The stylesheet is named `cfg_2.1-3.0.xsl`, and it can be used with any XSLT 1.0 processor. For example, if you have **xsltproc** available, you can use it to update a definition file as follows:

```
xsltproc -o output.jconf cfg_2.1-3.0.xsl input.config
```

This will generate a new file, `output.jconf`, based on `input.config` that uses version 3.0 of the definition file format.

We also provide a simple makefile in the xmlupdate directory for using this stylesheet with **xsltproc** or with the Xalan command-line utility. The purpose of the makefile is to hide the details of executing the actual commands. To do the above conversion, simply enter the following:

```
make INPUT_FILE=input.config OUTPUT_FILE=output.config cfg-update-2.1-3.0
```

At this time, the makefile only supports the use of **xsltproc** and the Xalan command-line utility. It defaults to using **xsltproc**. If that tool is not available, line 38 of the makefile so that the value of `$(XSLT_TARGET)` is `update-xalan`. If neither Xalan nor **xsltproc** is available, you will have to use whatever XSLT processor is available on your system.