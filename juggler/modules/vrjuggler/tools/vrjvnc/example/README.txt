Overview
--------

   NOTE: This application does not currently compile on Windows due to
         use of X11 data types in the VNC client code.

This example creates a VNC desktop viewer and draws it using OpenGL.

The example allows the user to grab, move, and resize the VNC desktop
interactively.

This is not meant to be a full application but is only meant to show how
VNC could be used inside a user application.

Please note that vrj_vnc_app.jdef configuration definition file in the
local directory.  It is used to create a config element that is needed to
configure the application with the correct VNC desktop parameters.  This
is optional in end-user applications, but is provided with this sample to
make it easier to customize the applciation.  In order for VRJConfig and
VR Juggler to find vrj_vnc_app.jdef, the environment variable
$JCCL_DEFINITION_PATH must be set to include this directory.  In the
examples below, <example-dir> represents the absolute path to this
directory.  On UNIX-based platforms, set $JCCL_DEFINITION_PATH to the
following value:

   $VJ_BASE_DIR/share/vrjuggler/data/definitions:<example-dir>

On Windows, it must be set to:

   %VJ_BASE_DIR%\share\vrjuggler\data\definitions;<example-dir>

License
-------

This example application is distributed under the GNU General Public
License (GPL).  This is necessary because it uses the VRJ VNC software
which is also distributed under the GPL.  Refer to COPYING.txt in the
parent directory (..) for the full GPL text.
