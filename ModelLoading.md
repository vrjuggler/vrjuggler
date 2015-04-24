# Q: How do I load model format XXX using VR Juggler? #

I have a model in format XXX (3DS, VRML, OpenFlight, OBJ, etc.), and I want to load it with VR Juggler.  How do I do that?

# A: VR Juggler does not have built-in capabilities for loading models. #

VR Juggler is an abstraction for VR hardware in much the same way that an operating system is an abstraction for your computer's hardware.  In that sense, an operating system has no built-in features for loading a document in format Y (Word, Excel, JPEG, MP3, etc.)--applications written for use in the operating system are required to load the document.  Similarly, a VR Juggler application must be written to load a model.

VR Juggler provides developers with access to various scenegraph implementations (OpenGL Performer, OpenSceneGraph, and OpenSG) that can load models.  To that end, VR Juggler comes with example applications that allow model loading using one of those scenegraphs.  In VR Juggler 1.0, those applications are as follows:

  * pfNav: Loads a model, named on the command line, with OpenGL Performer and allows flythroughs with collision detection.
  * pfConfigNav: An extension to pfNav that uses a VR Juggler configuration file to load the model.  In so doing, the model may be changed at runtime using VjControl.
  * simplePf: A very simple OpenGL Performer application that demonstrates how to load a model into the Performer scenegraph.  This has no navigation or collision detection.
  * OsgNav: An application similar to pfNav except that it uses the OpenSceneGraph (OSG) and has no collision detection.

VR Juggler 1.1 and newer have all of the above and the following:

  * OpenSGNav: An application similar to pfNav and OsgNav except that it uses OpenSG and has no collision detection.

If you do not want to use a scenegraph, you must use some tool (`polytrans`, for example) to convert the model into OpenGL commands or find a loader that has support for OpenGL rendering.  Those commands must then be called by a VR Juggler OpenGL application in the application object's `draw()` method.

Back to [FAQ](LiveFaq.md)