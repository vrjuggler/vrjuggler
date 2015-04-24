# Q: What is VR Juggler? #
There is a lot of information on this website, but I do not understand exactly what VR Juggler is or what it provides for virtual reality software.

# A: VR Juggler is middleware designed for the creation of cross-platform, cross-VR-system immersive software applications. #

## VR Juggler Is ... ##

VR Juggler provides an abstraction layer between the hardware of a VR system (head-mounted display, projection-based system, etc.) and the virtual world created in software.  This is very similar to how an operating system hides the details of how to access a hard drive, a network interface card, and a processor in a computer (desktop or otherwise).  Software applications make use of the operating system's abstract interfaces for communicating with the hardware, and the operating system is in charge of managing how applications access the hardware.  In other words, the operating system acts as  a platform upon which software applications run.  While VR Juggler is not an operating system, it does hide the details of VR input and output hardware.  For that reason, VR Juggler is referred to as a ''virtual platform''.

VR Juggler provides a great many features to its users:

  * Run-time reconfiguration so that applications can be reconfigured while they are running
  * Support for multi-pipe hardware
  * Support for a wide variety of input devices used in VR systems
  * Passive and active stereoscopic rendering
  * Cross-platform abstractions to make VR applications much more portable across operating systems
  * Support for clusters (VR Juggler 2.0 and newer)
  * Support for use of different graphics programming interfaces ([OpenGL](http://www.opengl.org/), [OpenGL Performer](http://www.sgi.com/products/software/performer/), [OpenSG](http://www.opensg.org/), and [Open Scene Graph](http://www.openscenegraph.org/)
    * Application programmers use the graphics API of their choice directly and let VR Juggler set up the projections and other window- and pipe-related parameters.

## VR Juggler is Not ... ##

The next logical question, then, is "What is VR Juggler not?"  Many people coming from a background of using scene graphs or software such as GLUT to write real-time graphics software expect that VR Juggler is the same as other software they have used.  This is not the right expectation to have of VR Juggler and what it does.  Specifically, VR Juggler is not any of the following:

  * VR Juggler is not a scene graph nor does it provide a custom scene graph for application programmers (see above for the list of supported scene graphs)
  * VR Juggler is not an OpenGL abstraction layer _per se_, though it does hide the details of opening windows, managing multiple graphics pipes, and setting up view frustum parameters
  * VR Juggler is not a model loader and leaves model loading up to the scene graph or to the application programmer

Moreover, it does not provide any of the following features:

  * VR Juggler does not have built-in scene navigation
  * VR Juggler does not do collision detection

Application programmers must implement these capabilities themselves.  There are other questions in this FAQ related to the capabilities of VR Juggler that new application programmers expect to find based on experience with other software packages.  Some examples are the following:

  * [Data importing from other applications](FormatConversions.md)
  * [Model loading](ModelLoading.md)
  * [Supported video cards](SupportedVideoCards.md)

Back to [FAQ](LiveFaq.md)