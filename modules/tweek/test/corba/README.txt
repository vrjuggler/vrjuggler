==============================================================================
1. Compiling this program
==============================================================================

   A. Type make?

==============================================================================
2. Running the test
==============================================================================

   A. Setting Environment Variables

      Before you can run corba_test, you must set the following environment
      variables:

          TWEEK_BASE_DIR: The root directory of your compiled Tweek
                          installation
          OMNIORB_CONFIG: The name (with full path if necessary) of your
                          omniORB config file
         LD_LIBRARY_PATH: Paths to directories where any required shared
                          libraries (omniORB, for example) can be found
                            

   B. omniORB Configuration

      You must have an omniORB configuration file (typically called
      omniORB.cfg) that contains, at a minimum, an initial reference for
      your NameService.  The omniORB program omniNames manages the naming
      serivce.  When run, it will print out an IOR that may be used for the
      initial reference, or you may use a CORBA URI to find the naming
      service more dynamically.  The bundled example omniORB.cfg shows how
      to specify this initial reference using a corbaname URI, the simplest
      way of accomplishing this.  For more information on CORBA URIs, visit
      the following WWW sites:

         http://www.uk.research.att.com/omniORB/doc/3.0/omniORB/omniORB004.html
         http://cgi.omg.org/cgi-bin/doc?ptc/00-08-07

      Optionally, it can contain settings for access restrictions using
      the standard hosts.allow and hosts.deny files.  These are especially
      useful with omniNames on platforms where the omniORB tcpwrapGK
      gatekeeper library is in use.

   C. Running omniNames

      If you are not going to run a naming service locally but instead will
      contact an existing one, you can skip this step.

      The executable omniNames (omniNames.exe) provides a naming service
      for use with CORBA applications.  Once the previous two steps have
      been completed, run omniNames.  If it is your first time running it,
      you must give the -start option on the command line so that it can
      create its initial log file.  You may need to provide the -logdir
      argument with a path where you have write access if the default is
      not usable.

   D. Running corba_test

      Now that a naming service is available, you can run corba_test.  If
      all of the above have been done correctly, it can be run with no
      arguments.  You may, however, pass CORBA and omniORB initialization
      arguments to the program as described on the following page:

         http://www.uk.research.att.com/omniORB/doc/3.0/omniORB/omniORB006.html

$Id$
