PyAppLoader
===========

   Description
   -----------
      This C++ application demonstrates how to load VR Juggler application
      objects written in Python into the VR Juggler microkernel.  It can
      take any number of Python application objects on the command line and
      cycle between them.  The Python application objects must come from a
      Python module with a global function named "vrjInit()" that returns an
      instance of a VR Juggler application object.

   Running It
   ----------
      To run PyAppLoader, you must have all the directories containing the
      Python modules you wish to load listed in your $PYTHONPATH environment
      variable.  Once that is set up, the application is executed as
      follows:

         ./pyAppLoader -c <VR Juggler config file> -m <full Python class name>

      There may be one or more VR Juggler configuration files listed (one
      file per -c argument) and one or more Python modules (one fully
      qualified class name per -m argument).

      For example, to run PyAppLoader with the VR Juggler configuration
      files sim.base.jconf and sim.wand.mixin.jconf and the Python
      class app.SimpleGlApp (found in app.py), use the following command:

   ./pyAppLoader -c sim.base.jconf -c sim.wand.mixin.jconf -m app.SimpleGlApp

      Note that it is not necessary to specify the full path to the
      configuration files if they are in the default directory or can be
      found via your $VJ_CFG_PATH environment variable.  The default search
      path for VR Juggler configuration files is the directory
      $VJ_BASE_DIR/share/vrjuggler/data/configFiles.

$Id$
