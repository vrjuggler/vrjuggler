# Q: How do I set up VRJ VNC? #

I have seen some PPT slides on VRJ and VNC to paste a PC desktop window into the 3D space. Are there any How-to's or tutorials which show how it can be done in detail? Or any sample code?

# A: Start with the example application in the `vrjvnc` directory. #

The VRJ VNC source code can be found under `$VJ_BASE_DIR/share/vrjuggler/tools/vrjvnc`.  In that directory is a subdirectory named `example` containing a relatively simple VR Juggler application that uses VRJ VNC.  That is the best place to get started with VRJ VNC. There is a file, `README.txt`, in the example application directory that should be reviewed before beginning.

Setting up VRJ VNC does not require much effort. First, configure and run a VNC server. Then, using the example application, modify the configuration file `vrjvnc.jconf` to have the host name, port number, and password for the VNC server. Before running the VR Juggler application, set the environment variable `$JCCL_DEFINITION_PATH` to include the example application directory. This is necessary so that Juggler can find the config definition file `vrj_vnc_app.jdef` at run time. Finally, run the example application with some VR Juggler configuration (e.g., `standalone.jconf`) **and** `vrjvnc.jconf`.

**NOTE**: VRJ VNC can only be run on UNIX-based platforms. The VNC server can, of course, be run anywhere that VNC is supported, but VRJ VNC as a VNC client currently works only on UNIX-based operating systems.

Back to [FAQ](LiveFaq.md)