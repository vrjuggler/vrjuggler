
  Juggler Project Overview


      The Juggler Team

$Date$

------------------------------------------------------------------------

    Introduction

Congratulations, you have downloaded the source code to the Juggler
Project, open source software for virtual reality from the Virtual
Reality Applications Center at Iowa State University. Within this
document, you will find information about the new VR Juggler 2.0
directory structure including descriptions of all the component modules
that make up the Juggler Project. Instructions for building the
individual modules can be found within the specific module directories
and on the VR Juggler website.


    Compiling

In this section, we describe how to compile the Juggler project. As of
this writing, the explanation focuses on VR Juggler itself.


      Important Note Regarding Compiling

Please note that you have downloaded /developmental/ code. It may not be
stable, and it may not even compile. As of this writing, compiling VR
Juggler itself is very complicated and is not recommended for anyone who
does not have some background in Autoconf, C++, and make.


      Compiling VR Juggler

As of this writing, VR Juggler is difficult to compile due to circular
dependencies in the various modules making up the Juggler Project. More
specifically, VR Juggler depends on Gadgeteer, and Gadgeteer depends on
VR Juggler. (Refer to the section called "Modules" for more information
about the specific modules.) As a result, the steps to build VR Juggler
are a little strange. Rest assured that this will be resolved soon. With
that in mind, the steps to build VR Juggler follow.


        Generate *configure* Scripts

Before any code can be built, the project *configure* scripts must be
generated. In the top-level juggler directory, run *autogen.sh*. This
will generate all the *configure* scripts in the Juggler Project source
tree.


        Configure and Compile VPR

First, VPR must be configured and compiled. All other projects depend on it.

   1.  cd modules/vapor

   2.  ./configure --prefix=$HOME/vrjuggler-2.0

   3.  gmake debug install

   4.  setenv VPR_BASE_DIR $HOME/vrjuggler-2.0


        Configure and Compile JCCL

Next, JCCL must be configured and compiled. JCCL requires a third-party
software package called Xerces C++. It can be downloaded from the Apache
XML site <http://xml.apache.org/xerces-c/index.html>. Once Xerces C++ is
built and installed, the JCCL configure script can find it.

   1.  cd modules/jackal

   2.  ./configure --prefix=$HOME/vrjuggler-2.0 --with-xercesroot=<Xerces C++ dir>

   3.  gmake debug install

   4.  setenv JCCL_BASE_DIR $HOME/vrjuggler-2.0

Depending on the version of Xerces C++ available, an extra argument may
be required to find the actual library. For example, if Xerces C++ 1.6.0
is installed, add /--with-xerceslib=libxerces-c1_6_0/ to the command
line for running *configure*. Check the Xerces C++ lib directory for the
exact name (without .so or .a).


        Configure VR Juggler

To deal with the circular dependency, VR Juggler must be configured but
not built. In addition, a "pseudo-installation" must be created that the
Gadgeteer configure script can find.

   1.  cd modules/vrjuggler

   2.  ./configure --prefix=$HOME/vrjuggler-2.0

   3.  gmake links

   4.  setenv VJ_BASE_DIR `pwd`/instlinks


        Configure and Compile Gadgeteer

Next, Gadgeteer can be configured and compiled. With the VR Juggler
pseudo-installation, this is pretty easy.

   1.  cd modules/gadgeteer

   2.  ./configure --prefix=$HOME/vrjuggler-2.0

   3.  gmake debug install

   4.  setenv GADGET_BASE_DIR $HOME/vrjuggler-2.0


        Re-Configure and Compile VR Juggler

Finally, with all of the previous steps taken, VR Juggler can be compiled.

   1.  cd modules/vrjuggler

   2.  ./reconfig

   3.  gmake debug install

   4.  setenv VJ_BASE_DIR $HOME/vrjuggler-2.0


    Modules

The Juggler Project is made up of several modules that have evolved from
the original VR Juggler source tree. Within this section, we describe
each of the modules, what they are used for, and, when possible, how
they relate to VR Juggler 1.0.


      VR Juggler Portable Runtime (VPR)


        Directory

juggler/modules/vapor


        Description

VPR is the foundation for all the Juggler modules. It provides an
object-oriented, cross-platform abstraction layer to common operating
system features such as threads, semaphores, and sockets. All the other
modules depend on VPR so that they may be ported easily to other
platforms. In VR Juggler 1.0, VPR was the collection of directories
Threads, SharedMem, and Sync. Since then, that code collected into a
single library and greatly extended into what is now VPR.

VPR itself contains a great deal of platform-specific code. It
categorizes this code into subsystems, and various subsystems may be
chosen depending upon the target platform and target threading system.
For example, on IRIX, there are three possible subsystems: SPROC, POSIX
and Netscape Portable Runtime (NSPR). Win32, on the other hand, only
uses the NSPR subsystem. The subsystems are:

    * SPROC (IRIX only)

    * POSIX (UNIX-based platforms only)

    * NSPR (all platforms, required on Win32 and Mac OS X)

Because VPR is the foundation for all the other modules, it is important
to choose the right subsystem.

Choosing the correct subsystem depends on many factors. For example,
which of the subsystems are supported on the desired target platform?
Are there compatibility issues with software outside the Juggler
Project? In most cases, however, the choice is straightforward. Indeed,
IRIX is the only platform that presents any real issues. In particular,
the use of OpenGL Performer from SGI raises important compatibility
concerns. SPROC threads are the only threads that are stable with OpenGL
Performer (on IRIX). POSIX threads will work, but applications may
sometimes crash on startup. As of this writing, OpenGL Performer does
not work at all with NSPR. Other software may pose similar problems, and
it is important to know the needs of other tools when configuring VPR.

On platforms other than IRIX, the choice is relatively simple. If NSPR
is available, its use is recommended. If NSPR is not available, POSIX
threads is the next logical choice--at least on UNIX-based platforms. On
Win32 and Mac OS X, NSPR is required.

As mentioned above, all the other modules depend on VPR, so it must be
compiled before anything else. For more information on compiling VPR,
refer to modules/vapor/INSTALL.txt.


      Juggler Configuration and Control Library (JCCL)


        Directory

juggler/modules/jackal


        Description

JCCL provides the configuration layer for VR Juggler and other modules.
The config chunks and configuration editor VjControl come from JCCL. In
essence, JCCL is the VR Juggler 1.0 Config directory with many new
features including XML-based config files.

To build the JCCL C++ library, Xerces-C is required for parsing XML. As
of this writing, version 1.5.1 has been used for testing. Newer versions
may work. Xerces-C can be downloaded from http://xml.apache.org/xerces-c/.

To build VjControl, the Java API for XML Parsing (JAXP) is required.
JAXP is distributed with JDK 1.4, and it is available for earlier
versions as a separately distributed library. More information and
downloads can be found at http://java.sun.com/xml/jaxp/index.html.


      Gadgeteer


        Directory

juggler/modules/gadgeteer


        Description

Gadgeteer is made up of the code that was formerly in the VR Juggler 1.0
Input directory. It contains all the drivers used for VR Juggler input
devices, and it contains the high-level Input Manager. The goal with
Gadgeteer is to mold it into the first dynamically loadable component
within the VR Juggler microkernel. At this time, it must still be
compiled in, but we hope to generalize the concepts of VR Juggler
Managers to the point that the kernel knows nothing about them until
they are loaded at run time.


      Sonix


        Directory

juggler/modules/sonix


        Description


      Plexus


        Directory

juggler/modules/plexus


        Description


      Distributed Shared Objects (DSO)


        Directory

juggler/modules/dso


        Description


      Tweek


        Directory

juggler/modules/tweek


        Description

Tweek is a cross-platform, cross-language library consisting of a C++
API and a Java API. Its purpose is to provide a convenient mechanism for
a Java-based GUI to interact with a complex C++ library or application.
To provide this capability, it uses CORBA.

The Java code in Tweek includes the Tweek Java API and the Tweek
JavaBean Loader application. The Java API is provided to make the use of
JavaBeans and CORBA simpler for programmers who want to write JavaBeans
that can communicate with a C++ application. The JavaBean Loader
application is a generalized application used to display and manage
those Beans. It simply loads the Beans it finds and presents them to the
user in some sort of structured manner (for example, a collection of
icons with web-style navigation).


      VR Juggler


        Directory

juggler/modules/vrjuggler


        Description

VR Juggler is still VR Juggler. There are many new features in VR
Juggler since the 1.0 series, but in essence, it is still the same
familiar tool. Of the above projects, VR Juggler depends on VPR, JCCL,
and Gadgeteer. Its individual components have simply been broken out
into individually developed modules. More information about the latest
version of VR Juggler is given in the following section.


    VR Juggler 2.0

