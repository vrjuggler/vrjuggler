# What is this: sonix
#
# Organization Credits: Iowa State University
#                       Virtual Reality Applications Center (VRAC)
#
# Contributors: 
#                       Kevin Meinert, Carolina Cruz-Neira
#

------------------
Table of contents:
------------------

   0. Introduction
      A. What is sonix?
      B. Audio APIs supported (compatability)
   1. Getting started
      A. Supported platforms
      B. Minimum requirements (compiler, libraries, etc.)
   2. Questions about sonix
      A. FAQ
           i. Please Explain the Patterns Used in sonix.
      B. Documentation
           i. None yet! Coming soon
      C. Mailing lists
           i. vrjuggler-info@lists.sourceforge.net
          ii. vrjuggler-devel@lists.sourceforge.net
   3. Compiling sonix from source
      A. Overview
      B. Details
      C. Advanced topics


==============================================================================
0. Introduction
==============================================================================

   A. What is sonix? 
      
      sonix is a project designed to: 
      - Allow access to many audio APIs so that an application can simply 
        program to one interface. This effectively moves the Sound API 
        descision to the application user or sysadmin.
      - Allow reconfiguration without affecting application
         - reconfiguration of Sound API 
           (from AudioWorks to OpenAL to OpenML? for example) 
         - reconfiguration of sound resources 
           (from Bang==bang.wav to Bang==bigbang.wav for example). 
      
   B. Audio APIs supported (compatability)

      - Currently sonix implements an: 
        - OpenAL interface 
        - AudioWorks interface 
      - Future plans to implement: 
        - OpenML? interface
      - Also wanted: 
        - If someone wants to volunteer, we're also interested in writing 
          an interface to Lake's sound API (contact me: kevn@vrac.iastate.edu) 
        - Max

==============================================================================
1. Getting started
==============================================================================

   A. Supported platforms

      sonix is officially supported on the following platforms:

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
        OpenAL or AudioWorks or other supported sonix sound subsystem

      * RedHat Linux 7.0
        GCC 2.96.69
        OpenAL

      * Win32
        Microsoft Visual Studio 6.0 (SP4 or SP5)
        Juggler Tools (http://www.vrjuggler.org/pub/juggler-tools-1.1.zip)
        OpenAL

==============================================================================
2. Questions about sonix
==============================================================================

   As with most open source projects, there is a common path to follow
   for getting answers to questions.  The first step is to look at the
   Frequently Asked Questions (FAQ) page on the web site.  If you do
   not find an answer there, refer to the detailed documentation.  If
   that too fails, send your question to the appropriate mailing list
   to get an answer from someone else in the sonix community.

   A. Frequently asked questions (FAQ)
       i. Please Explain the Patterns Used in sonix.
         - Adaptor   (snx::SoundImplementation)
           provides a common interface to the underlying sound API.

         - Prototype (snx::SoundImplementation)
           each sound implementation can be cloned, preserving its engine setup state

         - Store/plugin-method (snx::SoundFactory)
           each sound impl is registered with a Store. The Store allows
           users to select items from its inventory.  
           The Store is also an "Abstract Factory"

         - Abstract Factory (snx::SoundFactory)
           The Store can create new instances of the requested sound 
           implementation.  The Abstract Factory consults its Store of 
           registered objects, and if found, makes a clone of that object 
           (Prototype pattern).  The Abstract Factory is used to configure 
           the Bridge.

         - Bridge (sonix / snx::SoundImplementation)
           sonix is the class abstraction which is decoupled from its 
           implementation SoundImplementation.  This way the two can vary 
           independently.  Bridge also facilitates run-time configuration
           of the sound API.

         - Proxy (std::string)
           refer to actual sounds with a std::string Proxy.  This allows you 
           to generically control a sound for any sound API.  The sonix
           class acts as Mediator between every Proxy method and the actual 
           SoundImplementation Adaptor.
       
   B. Documentation
       i. None yet.  Coming soon

   C. Mailing lists
       i. vrjuggler-info@lists.sourceforge.net
      ii. vrjuggler-devel@lists.sourceforge.net
          
==============================================================================
3. Compiling sonix from source
==============================================================================

   A. Details

        i. Choose a directory to put the downloaded sonix source code
           (referred to as <JUGGLER_DIR> from now on).

       ii. Go to that directory and checkout sonix using CVS
           NOTE: make sure CVS is set up, and pointing to the sonix
           repository.  

           The simple version is:

            cd <JUGGLER_DIR>
            cvs checkout sonix

      iii. Enter the newly checked out sonix source directory

            cd <JUGGLER_DIR>/sonix

       iv. Compile sonix (using the Autoconf system and GNU make)

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

        v. Set the environment variable $SNX_BASE_DIR.  This is *required*
          by sonix's application makefiles and runtime environment.

          NOTES:
             1. The directory "instlinks" is semi-important. 
                It is a working distribution of the sonix library.
                After "gmake" is done, you'll be able to use this
                directory to compile and link your application code.
                This will not work on Win32, so a full installation
                must be performed.
             2. It is helpful to have something similar to the following
                line your $HOME/.tcshrc or $HOME/.cshrc file (assuming
                the use of tcsh for your shell):

                   setenv SNX_BASE_DIR [JUGGLER_DIR]/sonix/instlinks

                Use whatever is appropriate for your shell (BASH, ksh,
                DOS, etc.)

   C. Advanced topics

      Sending compiler output to a different directory than the source
      tree can be helpful for debugging and development.  This is done
      by making a directory to put the object files and compiled
      libraries before building.

      The following steps set up a build directory within your sonix
      source directory.  Here, we use the name <BUILD_DIR> to refer to
      the directory that will contain the object files and libraries.  It
      can be called whatever you want.

         mkdir <JUGGLER_DIR>/sonix/<BUILD_DIR>
         cd <JUGGLER_DIR>/sonix/<BUILD_DIR>
         <JUGGLER_DIR>/sonix/configure
