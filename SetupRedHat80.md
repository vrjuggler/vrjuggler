# Installing Red Hat Linux 8.0 #

## Basic Configuration ##

I installed the basic workstation configuration without any customization.

## Extra Packages ##

### Required ###

The following are requirements for VR Juggler after January 28, 2004.  Releases prior to this date do not need to install Boost and CppDOM separately.

  * [Boost 1.31.0](http://www.sf.net/projects/boost/)
    * If you need to compile and install Boost, bjam (boost-jam) will be necessary
  * [CppDOM 0.3.0](http://www.sf.net/projects/xml-cppdom/) or newer
    * To build CppDOM, you need [SCons](http://scons.sf.net/)

### Recommended ###

  * [OpenAL](http://www.openal.org/) for the Sonix OpenAL plug-in
  * [Audiere](http://audiere.sf.net/) for the Sonix Audiere plug-in
  * libuuid developer tools (RPM is `e2fsprogs-devel`)
  * Hardware-accelerated graphics support
    * http://www.nvidia.com/
    * http://www.ati.com/
  * DDD, the Data Display Debugger (RPM is `ddd`)
  * Some Java development kit, preferably J2SE 1.4.2 or newer.  These come from http://java.sun.com/j2se/
  * OpenORB: http://openorb.sourceforge.net/ (only if using a version of the JDK older than 1.4.0)

[JDOM](http://www.jdom.org/) is needed if working with a version of VR Juggler 1.1/2.0 released prior to October 15, 2003.

### Optional (System) ###

The following RPMs can be installed from the Red Hat distribution media by the system administrator, but they are not required.

**GLUT**

GLUT would be needed to use some features of Open Scene Graph and OpenSG.

  * `glut`
  * `glut-devel`

The GLUT source can be downloaded from [the GLUT website](http://www.opengl.org/resources/libraries/glut.html).  If GLUT 3.7 is not a good option, try [freeglut](http://freeglut.sf.net/).

### Optional (External) ###

The following optional packages come from external sources (rather than the base Red Hat distribution).

**omniORB**

  * omniORB 4.0.x
    * The official omniORB download page is http://omniorb.sourceforge.net/download.html
    * Binary RPMs can be found at http://wraith.fourpalms.org/pub/omniORB/RedHat/ or at http://opensource.nederland.net/omniORB/
      * Minimum required installation is `omniORB`, `omniORB-devel`, and `omniORB-servers`