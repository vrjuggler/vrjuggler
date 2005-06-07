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
   4. Running sonix tests, and sample apps...
   5. Trouble shooting
      LD paths
      
   6. Extending Sonix to your custom sound API.
      Writting custom sound API plugins (sound driver backends) for sonix


   7. Some Sonix design Philosophy
   
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
        - Audiere interface
      - Also wanted: 
        - If someone wants to volunteer, we're also interested in writing 
          an interface to Lake's sound API (contact us:
          vrjuggler-devel@lists.sourceforge.net) 
        - Max

==============================================================================
1. Getting started
==============================================================================

   A. Supported platforms

      sonix is officially supported on the following platforms:

        IRIX 6.5
        RedHat Linux 8.0 and newer
        Windows NT 5.0 and 2000

   B. Minimum requirements (by operating system)

      * All
        GNU make 3.78 or newer
        GNU Autoconf 2.14
        GNU Automake 1.4

      * IRIX
        MIPSpro Compilers 7.3.1.1m or newer
        OpenAL or AudioWorks or other supported sonix sound subsystem

      * RedHat Linux 8.0
        GCC 3.0.4
        OpenAL
        Audiere

      * Win32
        Microsoft Visual Studio 7.0
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
      i.  sonix is very easy to configure and build. 
          it requires the GMTL math library (ggt.sf.net)
          it optionally can use OpenAL or audioworks as its audio backend.
          (www.openal.org LGPL or multigen.com closed source)

      1. cd juggler/modules/sonix

      2. Choose one of the following depending on which subsystem (or both) you
         want to enable in sonix...

       ./configure --prefix=$HOME/vrjuggler-2.0
          --with-gmtl-include=/yourhomedir/juggler/modules/GMTL --enable-openal
          --with-oalroot=/usr/local

       ./configure --prefix=$HOME/vrjuggler-2.0
          --with-gmtl-include=/yourhomedir/juggler/modules/GMTL
          --enable-audioworks

      3. gmake debug install

      4. setenv SNX_BASE_DIR $HOME/vrjuggler-2.0

      
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

  4.  Running sonix test and sample applications
  
  when using audioworks, there is some variables you can set:

  o). setting the AW sound daemon hostname.
      this is the address of the computer that audioworks daemon is running on
      set the env variable SNX_AW_HOSTNAME, or create $HOME/.sonix-aw with
      a single string in it that is your hostname.
  
  5.  Trouble shooting
      LD paths
      
  Make sure you have your LD_LIBRARY_PATHs set...
  
  for Linux:
  setenv LD_LIBRARY_PATH "$VPR_BASE_DIR"/lib:"$GADGET_BASE_DIR"/lib:"$JCCL_BASE_DIR"/lib:"$VJ_BASE_DIR"/lib:"$SNX_BASE_DIR"/lib:"$XERCESROOT"/lib:"$OALROOT"/lib

  for Irix:
  setenv LD_LIBRARYN32_PATH "$VPR_BASE_DIR"/lib32:"$GADGET_BASE_DIR"/lib32:"$JCCL_BASE_DIR"/lib32:"$VJ_BASE_DIR"/lib32:"$SNX_BASE_DIR"/lib32:"$XERCESROOT"/lib32:"$OALROOT"/lib32



      Sound problems:

  - currently unix OpenAL has some bugs, try not to confuse these with 
    a sonix bug. :)  One issue is that pitch and gain cannot be adjusted at 
    the same time since one only works at 0,0,0, and one only works if not 0,0,0

  6.  Extending Sonix to your custom sound API.
  
  Writting sound API plugins (sound driver backends) for sonix
  
  the most important features are bind/unbind and startup/shutdown.
  
  bind/unbind:
  basically you implement the driver to support bind and unbind to allow
  sonix the capability to load or unload any sound at any time.  This is
  useful for a few things:
  
  1.) so you can change sound APIs at runtime.  sonix calls unbind for each
    configured sound, and then kills the current API, starts the new API,
    and then binds all the sounds there.  

  2.) so the application can configure sounds before OR after the sound API
    starts.  Since bind is implemented, sonix therefore has the capability
    to load the sound after the API has started.  One caveat is that this
    only works if your sound API supports loading sounds after it has
    started.  One way to get around it as a plugin writter, is to shutdown
    the API, then load the new sound, then startup the API again.  maybe
    slow, but it should work.  One caveat with this is that your sound API
    needs to support shutting down, then restarting again.
    
  startup/shutdown:
  These two are required to make sonix function properly.
  They allows sonix to start, stop, or restart the audio API at any time.  
  This may happen when the user 
  1.) changes the audio API at runtime
  2.) starts or exits their application
  
  trigger/release and other control functions:
  Trigger is the most important and should be implemented by every API plugin.
  The other control features may not be supported under your sound API, 
  in this case it is safe to stub them (withg an empty function).
  
  Other features not supported by the sonix API...
  Feel free to suggest API extensions to the team.  We're open to new ideas.
  Try to keep in the spirit of sonix being a simple to use sound triggering
  library.  Extra filter params might be a gfood suggestion, while an API
  to specify a synthesis network of unit generators might not be a good extension
  and is probably outside the scope of sonix.  In any case, run it by us
  we'd be interested to hear any ideas, and definatly we want to know 
  what types of tools/features you need for sound.


  7. Some Sonix design Philosophy
  
  Order of function calls:
  sonix is defined to be order independant.  To support 
  this order independance, each sonix audio plugin MUST support order 
  independance!  if they don't support this, then undefined behaviour can 
  result.  A general rule to note when writting an audio plugin for sonix
  is that the plugin should support the ability to configure some sounds, then
  start the API, then configure some more sounds, then switch APIs
  and expect that all sounds configured previously will still work under the 
  newly started API.
  
  Think of sonix as a database of conifigured sounds.  These may be configured
  at absolutely any time.  When ever an API changes/starts/stops, then 
  it must look at the current database and load everything there.

  Of course it is up to the driver to do lazy loading or smart paging if it
  wants, of course this isn't nessesary as long as the sound is available
  when calling operations such as trigger.
  
  The sonix audio plugin should also listen for newly configured sounds
  after the API has been started.  When a new sound comes in after the API is
  started, the audio plugin can simply load it like it did the other sounds
  it loaded upon startup (remember our example has sounds configured before
  the API starts?).
  
  To support this order independance, the audio plugins have 4 methods:
 
     o  startAPI
     o  shutdownAPI
     o  bind
     o  unbind
     
  Where bind loads the sound and makes it ready for use by functions such as 
  trigger, setVolume, etc... If you implement all of these correctly, then 
  your driver should be able to support this order independance.
  

  Configuration:
  sonix has no built in config loaders.  if you make one you like, feel free
  to submit it.  We may want to discuss possible methods before you get too
  far though (send your ideas to the mailing list).  one thing we've done 
  before is to write a small xml loader (non-jccl) using the xmlpp project 
  (a very lightweight xml parser/tree).  We could see something like this 
  being included with sonix.  Other ideas are a module that uses jccl config 
  features - another thing that could be included with sonix.

  The design philosophy is that config should not be part of the core, since
  not everyone wants the same methods for configuration.  since it would be
  convenient, I agree it would be cool to include 1 or 2 usable config 
  loaders in an auxilliery directory under sonix, and have them compile 
  to separate library files.

