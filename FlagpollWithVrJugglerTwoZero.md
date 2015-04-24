# Using Flagpoll with VR Juggler 2.0 #

VR Juggler 2.0.2 introduced the use of [Flagpoll](http://code.google.com/p/flagpoll/) as an alternative to the `*-config` scripts (`vrjuggler-config` et. al.). VR Juggler 2.2 and beyond require the use of Flagpoll because the `*-config` scripts have been removed. It is possible to use Flagpoll with VR Juggler 2.0.2 and newer to ease the transition to VR Juggler 2.2.

## Install Doozer 2.1 ##

To begin, [Doozer](http://sourceforge.net/projects/doozer/) 2.1.4 or newer must be installed instead of Doozer 2.0.0. For RPM-based Linux distributions, Doozer 2.1.x packages are available. See the [VR Juggler download page](http://www.vrjuggler.org/download.php) for details on the Infiscape YUM repository.

## Modify `vrj.appdefs.mk` ##

Next, the file `$VJ_BASE_DIR/share/vrjuggler/vrj.appdefs.mk` must be modified. The change is very simple, but it may interfere with other users and their use of a shared VR Juggler 2.0 installed. Make sure that everyone is aware that `$VJ_BASE_DIR/share/vrjuggler/vrj.appdefs.mk` is being changed.

First, we need to remove the (conditional) assignment of `$(DZR_BASE_DIR)` because, as we will see, it is set elsewhere. This line will be near the top of the file. It looks like this:

```
DZR_BASE_DIR?=         $(VJ_BASE_DIR)/share/Doozer
```

In Doozer 2.1, the file `$VJ_BASE_DIR/share/vrjuggler/vrj.appdefs.mk` is included by Doozer's `ext/vrjuggler/dzr.vrjuggler.mk` file. Thus, the file in `$VJ_BASE_DIR/share/vrjuggler/vrj.appdefs.mk` that includes `ext/vrjuggler/dzr.vrjuggler.mk` must be removed. Otherwise, a circular inclusion will occur, and GNU Make will run into serious problems. The line to remove looks like this:

```
include $(DZR_BASE_DIR)/ext/vrjuggler/dzr.vrjuggler.mk
```

## Modifying a Doozer-Based Makefile ##

Finally, two simple changes must be made to VR Juggler 2.0 application makefiles based on Doozer. They are as follows:


  1. Remove the line that includes `$(VJ_BASE_DIR)/share/vrjuggler/vrj.appdefs.mk`
  1. Add the following lines:

```
DZR_BASE_DIR=  $(shell flagpoll doozer --get-prefix)
include $(DZR_BASE_DIR)/ext/vrjuggler/dzr.vrjuggler.mk
```

An example of the change to the Doozer-bsaed makefile is shown below as a unified diff:

```
--- Makefile.in (revision 19219)
+++ Makefile.in (revision 19220)
@@ -37,7 +37,8 @@
 srcdir=		@srcdir@
 SRCS=		MPApp.cpp main.cpp
 
-include $(VJ_BASE_DIR)/share/vrjuggler/vrj.appdefs.mk
+DZR_BASE_DIR=	$(shell flagpoll doozer --get-prefix)
+include $(DZR_BASE_DIR)/ext/vrjuggler/dzr.vrjuggler.mk
 include $(DZR_BASE_DIR)/ext/vrjuggler/dzr.vrjuggler.glapp.mk
 
 # -----------------------------------------------------------------------------
```

These are the very same changes that must be made for VR Juggler 2.2 use, and by making these changes, a VR Juggler 2.0 application makefile based on Doozer can work with both VR Juggler 2.0 and 2.2.