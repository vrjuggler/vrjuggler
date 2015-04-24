# VR Juggler 2.0 Feature List #

  * Modularization
    * Operating system abstraction ([VPR](http://www.vrjuggler.org/vapor/))
    * Sound system ([Sonix](http://www.vrjuggler.org/sonix/))
    * Java/C++ interaction through CORBA ([Tweek](http://www.vrjuggler.org/tweek/))
    * Configuration system ([JCCL](http://www.vrjuggler.org/jccl/))
    * Input system ([Gadgeteer](http://www.vrjuggler.org/gadgeteer/))
    * Virtual platform ([VR Juggler](http://www.vrjuggler.org/vrjuggler/))
  * Cluster support
    * Cluster plugins
      * Remote input manager
      * Synchronization layer
      * Application-specific shared data
  * DLLs on Win32
  * Mac OS X support
    * Requires [X11 for OS X](http://www.apple.com/macosx/features/x11/) and OS X 10.3 (Panther) or newer&mdash;OS X 10.4 (Tiger) highly recommended
  * [Windows XP Professional x64 Edition](http://www.microsoft.com/windowsxp/64bit/default.mspx) support
    * Requires [patched](https://bugzilla.mozilla.org/attachment.cgi?id=183172) version of [NSPR 4.6](ftp://ftp.mozilla.org/pub/mozilla.org/nspr/releases/) (NSPR 4.7 should have Windows x64 support integrated)
    * Requires Visual Studio 2005
  * Revamped configuration system
    * Simplified terminology
      * "Config chunks" are now called "configuration elements," reflecting that they are ''units of configuration''
      * "Chunks descs" are now called "configuration definitions" to indicate that they define the structure of configuration elements
    * XML for configuration files and definition files
    * File format versioning
    * Configuration element versioning
    * XSLT for transitioning between versions of file formats and configuration elements
    * Automatic configuration file updating in the editor GUI (see below)
  * Keyboard/mouse input for graphics windows
  * Input logger for input playback and testing
  * No more `CTRL-c` to shut down applications
  * Multiple viewports in graphics windows
  * Support for new compilers
    * GCC 3.0, 3.1, 3.2, 3.3, 3.4, 4.0
    * Visual C++ 7.0 (VS.NET 2002), 7.1 (VS.NET 2003), 8.0 (Visual Studio 2005)
  * Use of NSPR on certain platforms (e.g., Windows) to improve portability
  * Refactored Input Manager
  * Redesigned configuration editor ([VRJConfig](VrjConfig.md))
    * All-new GUI
    * Standardized remote communication protocol
    * Custom (high-level) editor framework
      * Custom editor for device/proxy relationships
      * Custom editor for display windows and viewports
      * Custom editor for simulator configurations
      * Custom editor for projection-based immersive VR systems
    * Configuration creation wizards
      * Wizard for projection-based immersive VR configuration
      * Wizard for device configuration
  * Revamped performance monitoring
    * Less intrusive API
    * New Java visualization plug-in
    * User-defined performance categories
  * Support for new graphics APIs
    * Open Scene Graph
    * OpenSG
  * User-defined debugging categories
  * Fully cross-platform device drivers
    * Based on abstraction layer for termios and Win32 serial I/O
  * New device drivers
    * [VRPN](http://www.cs.unc.edu/Research/vrpn/)
    * Joysticks supported by Linux "joydev" (USB and game port)
    * Joysticks supported by DirectInput
    * Microsoft Speech API for voice input
    * 3Dconnexion SpaceBall and SpaceMouse
  * Clearer output messages (differentiating errors from normal status)
  * Python language bindings: [PyJuggler](http://www.vrjuggler.org/pyjuggler/)
  * In-progress .NET language bindings (C#, !VB.NET, J#, etc.): [VRJ.NET](http://www.vrjuggler.org/vrj.net/)
  * New and expanded documentation created using [Doxygen](http://www.doxygen.org/), [JavaDoc](http://java.sun.com/products/jdk/javadoc/), and [DocBook](http://www.docbook.org/)
    * [VPR Documentation](http://www.vrjuggler.org/vapor/docs.php)
    * [Tweek Documentation](http://www.vrjuggler.org/tweek/docs.php) (Java and C++)
    * [JCCL Documentation](http://www.vrjuggler.org/jccl/docs.php) (Java and C++)
    * [Sonix Documentation](http://www.vrjuggler.org/sonix/docs.php)
    * [Gadgeteer Documentation](http://www.vrjuggler.org/gadgeteer/docs.php)
    * [VR Juggler and VRJConfig Documentation](http://www.vrjuggler.org/vrjuggler/docs.php) (Java and C++)