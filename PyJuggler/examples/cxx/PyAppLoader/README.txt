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

         ./pyAppLoader -c <VR Juggler config file> -m <Python module>

      There may be one or more VR Juggler configuration files listed (one
      file per -c argument) and one or more Python modules (one module
      per -m argument).

      For example, to run PyAppLoader with the VR Juggler configuration
      files sim.base.config and sim.wand.mixin.config and the Python
      module app.SimpleGlApp, use the following command:

   ./pyAppLoader -c sim.base.config -c sim.wand.mixin.config -m app.SimpleGlApp

      Note that it is necessary to specify the full path to the
      configuration files.  Typically, they will be found in the directory
      $VJ_BASE_DIR/share/vrjuggler/data/configFiles.

$Id$
