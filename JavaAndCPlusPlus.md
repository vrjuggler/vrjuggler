# Q: Why use both Java and C++? #

VR Juggler documents refer to the use of Java and C++ together.  Why not just make everything Java or everything C++ instead of having a mix?  Do I have to use both languages to write an application?

# A: The two languages serve different purposes. #

In VR Juggler, Java and C++ are used for different purposes.  We use C++ for high-performance, natively compiled code capable of delivering the real-time performance needed for VR.  Users write their applications in C++ and compile them against the VR Juggler libraries.

We use Java to make cross-platform GUIs that can communicate with the running C++ code.  These GUIs may do any number of things.  In particular, VR Juggler 1.0 has a tool called VjControl that can be used to do remote runtime reconfiguration of a running VR Juggler application.  In VR Juggler 1.0, users do not write any Java code.

VR Juggler 2 will have the next iteration on this tool, [VRJConfig](VrjConfig.md). With VRJConfig, users will have the option of writing custom configuration editors in Java.  These editors will plug into VRJConfig dynamically.

Back to [FAQ](LiveFaq.md)