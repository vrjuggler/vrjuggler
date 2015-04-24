# Q: What video cards are supported by VR Juggler? #

What video cards are currently supported by VRJuggler? What cards (nVidia quadro, for example), give the best performance?

# A: VR Juggler can use whatever the operating system supports. #

VR Juggler does not interact directly with the video card.  It sends commands to it through OpenGL or a scene graph such as [OpenSG](http://www.opensg.org/), the [Open Scene Graph](http://www.openscenegraph.org/), or [OpenGL Performer](http://www.sgi.com/software/performer/).  Hence, whatever cards are supported by your (accelerated) OpenGL graphics drivers can be used by VR Juggler.

Back to [FAQ](LiveFaq.md)