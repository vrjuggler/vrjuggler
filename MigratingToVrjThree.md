# Porting from VR Juggler 2.2 to 3.0 #

This document describes the process of updating VR Juggler 2.2 applications to VR Juggler 3.0. VR Juggler 2.0 users should read the [2.0 to 2.2 Migration Guide](MigratingToVrjTwoTwo.md) first. Considerations for VR Juggler 2.0 applications and configurations will not be made in this document.

## Configuration Changes ##

**IMPORTANT**: All VR Juggler 2.2 configuration files must be updated using an XSLT processor prior to being loaded into VRJConfig for automatic upgrading. Changes to the cluster configuration have been made that cannot be expressed using the automatic updating capabilities, and thus off-line updates are required as the first step.

The transform is in the file `juggler/trunk/modules/vrjuggler/data/xslt/2.2-3.0.xsl` (or `$VJ_BASE_DIR/share/vrjuggler-3.0/data/xslt/2.2-3.0.xsl`), and it should be applied to all VR Juggler 2.2 configuration files. Instructions for using this transform can be found in the file after the copyright notice. The sample configuration files that come with VR Juggler have been updated.

For other configuration changes, VRJConfig can be used to migrate config elements to newer versions automatically, and it is recommended that all VR Juggler 2.2 configuration files be loaded into VRJConfig and saved using the "Save All" button. For sites that need to maintain configuration files for both versions, the VR Juggler 2.2 files should be copied into a new location for use by VR Juggler 3.0.

## Gadgeteer Interface Changes ##

**IMPORTANT**: The Gadgeteer version number can be used at compile time to make decisions about which version of the API will be needed. The version number is available using the preprocessor symbol `__GADGET_version`, defined in `gadget/gadgetParam.h`. The encoding is based on the major, minor, and patch version numbers using the scheme `MMMmmmPPP` (three digits for the major version number, three for the minor, and three for the patch) with left zero padding. For example, Gadgeteer version 1.3.15 is encoded as 1003015. Code examples below will make use of `__GADGET_version` to demonstrate how to write code that works with Gadgeteer 1.2 and 2.0.

In many ways, Gadgeteer is the heart of the Juggler Suite. It handles input, and input drives VR applications. More than that, the cluster infrastructure, which is based on synchronized input data distribution, is part of Gadgeteer. The cluster infrastructure has been rewritten in Gadgeteer 2.0 to be based on a client/server model.

### Device Drivers ###

Subclasses of `gadget::Input` need no longer overload `operator delete` and override `gadget::Input::destroy()`. This was deemed unnecessary and removed in Gadgeteer 1.3.11 (encoded as the `__GADGET_version` value 1003011).

### Mouse Event Coordinate Frame ###

Prior to Gadgeteer 2.0, mouse events associated with a keyboard/mouse device utilized a coordinate frame with an origin at the upper left-hand corner of the window (or the display for root-level mouse positions). This was in contrast to the window positioning convention of using the lower left-hand corner as the origin. To resolve this discrepancy, the mouse event code was changed in Gadgeteer 1.3.16 (encoded as the `__GADGET_version` value 1003016) to use the lower left-hand corner as the origin. Thus, code using the X,Y coordinates from `gadget::MouseEvent` objects must be updated to reflect the inverted Y-axis values.

### Mouse Locking ###

It is now possible to to specify wether any modifier key causes the mouse to be locked to a respective display window with the `allow_mouse_locking` property in the `display` config element. Mouse locking can also be controlled at the API level using the following methods:

```
#include <gadget/Devices/KeyboardMouseDevice/InputArea.h>

void gadget::InputArea::lockMouse();
void gadget::InputArea::unLockMouse();
```

When these methods are used or a modifier key is pressed and the modifier key is configured in the apps loaded configuration,  the mouse is locked to its current location in the respective display window. This is different than in past VR Juggler releases where the mouse would snap to the center of the respective display window.

### Shared Pointer Usage ###

Many uses of raw pointers in Gadgeteer have been replaced with instantiations of `boost::shared_ptr<T>`. The following list shows the classes, according to inheritance hierarchy, that are now accessed as `boost::shared_ptr<T>` instantiations:

  * `gadget::Input` as `gadget::InputPtr` for Gadgeteer 1.3.4 (`__GADGET_version` value 1003004) and newer
    * `gadget::Analog` as `gadget::AnalogPtr`
    * `gadget::Command` as `gadget::CommandPtr`
    * `gadget::Digital` as `gadget::DigitalPtr`
    * `gadget::Gesture` as `gadget::GesturePtr`
    * `gadget::Glove` as `gadget::GlovePtr`
    * `gadget::KeyboardMouse` as `gadget::KeyboardMousePtr`
    * `gadget::Position` as `gadget::PositionPtr`
    * `gadget::String` as `gadget::StringPtr`
  * `gadget::Proxy` as `gadget::ProxyPtr` for Gadgeteer 1.3.5 (`__GADGET_version` value 1003005) and newer
    * `gadget::AnalogProxy` as `gadget::AnalogProxyPtr`
    * `gadget::CommandProxy` as `gadget::CommandProxyPtr`
    * `gadget::DigitalProxy` as `gadget::DigitalProxyPtr`
    * `gadget::GestureProxy` as `gadget::GestureProxyPtr`
    * `gadget::GloveProxy` as `gadget::GloveProxyPtr`
    * `gadget::KeyboardMouseProxy` as `gadget::KeyboardMouseProxyPtr`
    * `gadget::PositionProxy` as `gadget::PositionProxyPtr`
    * `gadget::StringProxy` as `gadget::StringProxyPtr`

Most application-level code probably does not retain pointers to device or proxy instances. Normally, device proxy objects are accessed through automatically constructed temporary variables through usage of `gadget::DeviceInterface<T>::operator->()`. Input device instances can be retrieved either through the Input Manager singleton or through a device proxy object.

Nevertheless, it is valid for application code to have data members, stack variables, global variables, etc. that are pointers to Gadgeteer device instances and/or device proxy instances. The code shown below will work with all Gadgeteer versions between 1.0 and 2.0. The `#if` usage can be simplified by not worrying about Gadgeteer 1.3.x versions and simply testing against the 2.0.0 value (2000000).

```
#include <gadget/InputManager.h>
#include <gadget/Type/Position.h>
#include <gadget/Type/PositionProxy.h>
#include <gadget/gadgetParam.h>


#if __GADGET_version < 2000000
namespace gadget
{
#if __GADGET_version < 1003004
   typedef Input*         InputPtr;
   typedef Position*      PositionPtr;
#endif
#if __GADGET_version < 1003005
   typedef Proxy*         ProxyPtr;
   typedef PositionProxy* PositionProxyPtr;
#endif
}
#endif

void MyApp::preFrame()
{
   // Input device query.
   gadget::InputPtr dev =
      gadget::InputManager::instance()->getDevice("Some Device");

   // Downcast to a positional device instance.
   gadget::PositionPtr pos_dev =
#if __GADGET_version < 1003004
      dynamic_cast<gadget::PositionPtr>(dev);
#else
      boost::dynamic_pointer_cast<gadget::Position>(dev);
#endif

   // Proxy query.
   gadget::ProxyPtr proxy =
      gadget::InputManager::instance()->getProxy("Pos Proxy");

   // Downcast to a position proxy.
   gadget::PositionProxyPtr pos_proxy =
#if __GADGET_version < 1003005
      dynamic_cast<gadget::PositionProxyPtr>(proxy);
#else
      boost::dynamic_pointer_cast<gadget::PositionProxy>(proxy);
#endif

   // Rest of preFrame() ...
}
```

### Method Name Generalization ###

Gadgeteer 2.1 improves generality of device data handling and the proxy implementation. As part of this work, method names that are type specific (such as `gadget::AnalogData::getAnalog()` and `gadget::AnalogProxy::getAnalogPtr()`) have been replaced with general names (`gadget::AnalogData::getValue()` and `gadget::AnalogProxy::getTypedInputDevice()`). The most likely example of code that would use these methods is that which accesses the device sample buffers.

In Gadgeteer 2.0, the type-specific method names are still present, but forward compatibility methods have been added to make the future transition to Gadgeteer 2.1+ simpler. These changes are optional, but it is recommended that they be made sooner rather than later. Examples of using these interfaces are shown below:

```
   // head is a gadget::PositionInterface object.
   const buffer_type& head_sample_buffer =
#if __GADGET_version >= 2000000
       head->getTypedInputDevice()->getPositionDataBuffer();
#else
       head->getPositionPtr()->getPositionDataBuffer();
#endif

   const gmtl::Matrix44d head_mat =
#if __GADGET_version >= 2000000
      gmtl::convertTo<double>(head_sample_buffer[0][dev_num].getValue());
#else
      gmtl::convertTo<double>(head_sample_buffer[0][dev_num].getPosition());
#endif

   gadget::KeyboardMousePtr km_dev =
#if __GADGET_version >= 2000000
      mKeyboardMouse->getTypedInputDevice();
#else
      mKeyboardMouse->getKeyboardMousePtr();
#endif
```

## VR Juggler Interface Changes ##

**IMPORTANT**: The VR Juggler version number can be used at compile time to make decisions about which version of the API will be needed. The version number is available using the preprocessor symbol `__VJ_version`, defined in `vrj/vrjParam.h`. The encoding is based on the major, minor, and patch version numbers using the scheme `MMMmmmPPP` (three digits for the major version number, three for the minor, and three for the patch) with left zero padding. For example, VR Juggler version 2.3.15 is encoded as 2003015. Code examples below will make use of `__VJ_version` to demonstrate how to write code that works with VR Juggler 2.2 and 3.0.

### New `main()` Function Step ###

VR Juggler 3.0 clustered applications must identify whether the application instance is the server or one of the clients. This is done using a command line option at launch time: either `--vrjmaster` or `--vrjslave`. In order for the kernel to know about the command line option, one of the `vrj::Kernel::init()` overloads must be invoked _before_ any calls to `vrj::Kernel::loadConfigFiles()` or `vrj::Kernel::start()`. Moreover, `vrj::Kernel::start()` must not be called before configuration files are loaded--unless remote run-time reconfiguration is used to configure VR Juggler.

There are two ways to initialize the kernel from the application `main()` function. Both were introduced in VR Juggler 2.3.0 (encoded as the `__VJ_version` value 2003000). The first is suitable for the case when the application does not process command line options other than a list of VR Juggler configuration files. In this case, we use the `vrj::Kernel::init(int&,char*[])` initialization method. This is demonstrated in the code below.

```
#include <cstdlib>
#include <vrj/Kernel/Kernel.h>
#include <vrj/vrjParam.h>


int main(int argc, char* argv[])
{
#if ! defined(VRJ_USE_COCOA)
   if (argc <= 1)
   {
      std::cerr << "Missing command line options\n";
      return EXIT_FAILURE;
   }
#endif

   vrj::Kernel* kernel = vrj::Kernel::instance();
   MyApp app;

#if __VJ_version >= 2003000
   // Initialize the kernel.
   kernel->init(argc, argv);
#endif

   // Load any config files specified on the command line
   for (int i = 1; i < argc; ++i)
   {
      kernel->loadConfigFile(argv[i]);
   }

   kernel->start();
   kernel->setApplication(&app);
   kernel->waitForKernelStop();

   return EXIT_SUCCESS;
}
```

### Draw Manager Namespaces ###

The namespaces for specific windowing and draw implementations within VR Juggler have been updated to move API-specific information from the file name and into the C++ namespace. As an example, the OpenGL window is no longer used this way:

```
#include <vrj/Draw/OGL/GlWindow.h>

// Deprecated namespace
vrj::GlWindow
```

Instead, it is now the following:

```
#include <vrj/Draw/OpenGL/Window.h>

// New namespace
vrj::opengl::Window
```

This change affects these classes:

```
// OpenGL classes
vrj::GlApp
vrj::GlContextData
vrj::GlDrawManager
vrj::GlProcApp
vrj::GlUserData
vrj::GlWindow
vrj::GlWindowPtr

// Open Scene Graph (OSG) class
vrj::OsgApp

// OpenSG class
vrj::OpenSGApp

// Direct3D classes
vrj::D3dApp
vrj::D3dDrawManager

// Performer classes
vrj::PfApp
vrj::PfDrawManager
vrj::PfUtil
```

These classes are now named as follows:

```
// OpenGL classes
vrj::opengl::App
vrj::opengl::ContextData
vrj::opengl::DrawManager
vrj::opengl::ProcApp
vrj::opengl::UserData
vrj::opengl::Window
vrj::opengl::WindowPtr

// OSG class
vrj::osg::App

// OpenSG class
vrj::opensg::App

// Direct3D classes
vrj::direct3d::App
vrj::direct3d::DrawManager

// Performer classes
vrj::pf::App
vrj::pf::DrawManager
vrj::pf::Util
```

In the VR Juggler 3.0 release, there are helper headers that are in the scenegraph-specific and OpenGL-specific directories with typedefs from the old namespace to the new namespace to help in the migration process. As an example, here is the contents of the new `vrj/Draw/OGL/GlApp.h` header:

```
/**
 * \file
 *
 * This file is for backwards compatibility with VR Juggler 2.x code. New
 * code should not include this header and should use vrj/Draw/OpenGL/App.h.
 *
 * @deprecated This file has been deprecated in favor of vrj/Draw/OpenGL/App.h.
 */

#if defined(__GNUC__)
#warning "This header is deprecated. Use vrj/Draw/OpenGL/App.h instead."
#endif

#include <vrj/Draw/OpenGL/App.h>


namespace vrj
{

/**
 * Backwards compatibility type declaration for vrj::opengl::App.
 *
 * @since 2.3.11
 *
 * @deprecated vrj::GlApp has been deprecated in favor of vrj::opengl::App.
 */
typedef vrj::opengl::App GlApp;

} // End of vrj namespace
```

These header files will be removed in the VR Juggler 3.2 release.