## VR Juggler 2.2 Features ##

Some VR Juggler 2.2 features may be merged to the VR Juggler 2.0 release branch provided that users do not have to change existing code or configurations.

### High-Level 2.2 Feature List ###

  * Improved display system configuration
    * Allow for hardware-specific swap locking
  * Vastly improved error handling and error reporting in VPR through the use of exceptions
  * Simpler thread creation through the use of [Boost.Function](http://www.boost.org/libs/function/) (instead of `vpr::ThreadMemberFunctor` and `vpr::ThreadNonMemberFunctor`)
  * Proper destruction of singletons at application exit
    * Allows for device drivers to be shutdown cleanly to avoid leaving hardware devices in a bad state
  * Mac OS X support using Cocoa

## VR Juggler 3.0 Features ##

### High-Level 3.0 Feature List ###

### Pending 3.0 Features ###

  * Support for new compilers
    * Sun Forte
  * Write new sample applications
    * Document as tutorial applications
  * Extended capabilities for user configuration of !OpenGL
    * Add a callback method to `vrj::GlApp` that allows users to set GL visual parameters programatically (similar to `vrj::PfApp::configPWin()`)
  * Improved display system configuration
    * Remove the notion of X11 pipes in favor of something more platform-agnostic
      * See SourceForge [task #40471](http://sourceforge.net/pm/task.php?func=detailtask&project_task_id=40471&group_id=8041&group_project_id=2906)
  * Cross-platform device drivers
    * USB serial compatibility or interface
  * Finish up the glove input interface
    * Convert the glove drivers to the glove interface
    * Refactor gesture interface
    * Refactor glove input to work will all glove types
  * Cluster reconfigurability
  * Customizable simulator
    * Draw functors
    * Simulator plug-in framework
      * Custom simulator interfaces with configurable selection
  * Revamped performance monitoring
    * Multiple ways to visualize data
  * VRJConfig additions
    * Configuration validation
      * On-the-fly validation
    * Wizards for file creation
      * Wizard for each device type (analog, digital, position)
      * Wizard for creating a new cluster configuration
    * Real-time validation of the active configuration
    * 3D visualization of the active configuration

  * Automated testing of system
    * Nightly builds with status report for each platform
    * Test suites for all projects
      * VPR, JCCL, GMTL exist but need extended as we go
      * VRJ, Gadgeteer need added
      * Goal 75% code coverage

## Check Google Code ##

For the most up-to-date information, check the status of the project at Sourceforge.net.

  * [Google Code Project Page](http://code.google.com/p/vrjuggler/)
  * [Bugs](http://code.google.com/p/vrjuggler/issues/list)
  * Current Tasks: [SF Tasks](http://sourceforge.net/pm/?group_id=8041)