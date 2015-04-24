# Q: How can I change the model used to represent the wand? #

How to load a model to represent the wand instead of drawing it with OpenGL? What are the 3D formats supported by VR Juggler?

# A: Change the simulator viewport configuration. #

With the Performer Draw Manager, changing the model loaded for the wand (and the head) can be accomplished by editing the simulator viewport configuration elements to refer to a different model.  The model formats supported are those supported by Performer.

With the OpenGL Draw Manager in VR Juggler 2.0, it is not possible to change the wand or the head graphics in the simulator viewports without modifying the basic GL simulator C++ code and recompiling.  There was an effort made to allow users to plug in draw functors to the OpenGL Draw Manager, but the design was never fully functional.  This feature is planned to be completed and available for use in [VR Juggler 2.2](DevelopmentStatus#VR_Juggler_2_2_Features.md).

Back to [FAQ](LiveFaq.md)