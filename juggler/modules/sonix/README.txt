# What is this: VR Juggler
#
# Organization Credits: Iowa State University
#                       Virtual Reality Applications Center (VRAC)
#
# Contributors (Alphabetically): 
#               Albert Baker, Allen Bierbaum, Carolina Cruz-Neira, 
#               Jesse Coats, Patrick Hartling, Andrew Himberger, 
#               Chris Johanson, Christopher Just,
#               Kevin Meinert, Ben Thompson
# 

------------------
Table of contents:
------------------

    1. Getting started
        A. Supported platforms
        B. Minimum requirements (compiler, libraries, etc.)
    2. Questions about Audio Juggler
        A. FAQ
        B. Documentation
              i. Getting Started Guide
             ii. Programmer's Guide
            iii. Programmer's Reference
             iv. VjControl
              v. VR Juggler Team Guide
        C. Mailing lists
              i. vrjuggler-info@lists.sourceforge.net
             ii. vrjuggler-devel@lists.sourceforge.net
    3. Compiling VR Juggler from source
        A. Overview
        B. Details
        C. Advanced topics

==============================================================================
1. Getting started
==============================================================================

    A. Supported platforms

        AudioJuggler is officially supported on the following platforms:

            IRIX 6.5
            RedHat Linux 7.1 and newer
            Windows NT 4.0 and 2000
            Windows 98/Me

    B. Minimum requirements (by operating system)

        * All
            GNU make 3.78 or newer
            GNU Autoconf 2.13
            GNU Automake 1.4

        * IRIX
            MIPSpro Compilers 7.3.1.1m or newer
            OpenAL or AudioWorks or other supported AudioJuggler sound subsystem

        * RedHat Linux 7.0
            GCC 2.96.69
            OpenAL

        * Win32
            Microsoft Visual Studio 6.0 (SP4 or SP5)
            Juggler Tools (http://www.vrjuggler.org/pub/juggler-tools-1.1.zip)
            OpenAL

==============================================================================
2. Questions about VR Juggler
==============================================================================

    As with most open source projects, there is a common path to follow
    for getting answers to questions.  The first step is to look at the
    Frequently Asked Questions (FAQ) page on the web site.  If you do
    not find an answer there, refer to the detailed documentation.  If
    that too fails, send your question to the appropriate mailing list
    to get an answer from someone else in the VR Juggler community.

    A. Frequently asked questions (FAQ)

    B. Documentation

    C. Mailing lists

==============================================================================
3. Compiling VR Juggler from source
==============================================================================

    A. Details

          i. Choose a directory to put the downloaded AudioJuggler source code
             (referred to as <JUGGLER_DIR> from now on).

         ii. Go to that directory and checkout AudioJuggler using CVS
             NOTE: make sure CVS is set up, and pointing to the AudioJuggler
             repository.  

            The simple version is:

                cd <JUGGLER_DIR>
                cvs checkout AudioJuggler

        iii. Enter the newly checked out AudioJuggler source directory

                cd <JUGGLER_DIR>/AudioJuggler

         iv. Compile AudioJuggler (using the Autoconf system and GNU make)

              a. Generate aclocal.m4, header template (vjDefines.h.in), and
                 the configure script

                    ./autogen.sh
    
              b. Probe the system for capabilities, generate Makefiles, etc.
                 (this configures the source tree for the current system)

                    ./configure

              c. Make the distribution using GNU make.  This compiles, links,
                 and creates the final distribution tree (in the form of
                 symlinks)

                    gmake

          v. Set the environment variable $AJ_BASE_DIR.  This is *required*
             by AudioJuggler's application makefiles and runtime environment.

             NOTES:
                 1. The directory "instlinks" is semi-important. 
                    It is a working distribution of the AudioJuggler library.
                    After "gmake" is done, you'll be able to use this
                    directory to compile and link your application code.
                    This will not work on Win32, so a full installation
                    must be performed.
                 2. It is helpful to have something similar to the following
                    line your $HOME/.tcshrc or $HOME/.cshrc file (assuming
                    the use of tcsh for your shell):

                        setenv AJ_BASE_DIR [JUGGLER_DIR]/AudioJuggler/instlinks

                    Use whatever is appropriate for your shell (BASH, ksh,
                    DOS, etc.)

    C. Advanced topics

       Sending compiler output to a different directory than the source
       tree can be helpful for debugging and development.  This is done
       by making a directory to put the object files and compiled
       libraries before building.

       The following steps set up a build directory within your VR Juggler
       source directory.  Here, we use the name <BUILD_DIR> to refer to
       the directory that will contain the object files and libraries.  It
       can be called whatever you want.

           mkdir <JUGGLER_DIR>/AudioJuggler/<BUILD_DIR>
           cd <JUGGLER_DIR>/AudioJuggler/<BUILD_DIR>
           <JUGGLER_DIR>/AudioJuggler/configure
