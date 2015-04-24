**READ THIS FIRST:** WhatIsaVrjDeveloperRelease

**WARNING:** VR Juggler developer releases are unsupported and very likely to be unstable.  You have been warned.

## Why you might want to run a developer release? ##
If you want to see what the bleeding-edge of VR Juggler development looks like and help the VR Juggler developers refine and debug VR Juggler, then the developer release is for you.

The developer releases contain the latest features and give a taste of what the next version of VR Juggler will be like.

Enough with the preliminary info, you are here so you must want a developer release.  See below for the currently available releases

## New and future features ##

For information related to all the new features since VR Juggler 1.0, please refer to the Discussion Area on our web site. Please note that a few of those [features](VrjTwoZeroFeatures.md) are still in development for VR Juggler 2.0.

## Porting from 1.0 ##

Developer releases represent many months of work by an expanding group of developers. We have worked to address problems with VR Juggler 1.0, both in the design and in the implementation. We have also taken advantage of recent advances in C++ compiler technology to make the code faster, simpler, and more robust.

There is a [migration guide](MigratingToVrjTwo.md) for people who want to port applications to VR Juggler 1.1/2.0. We will continue to revise and update this document. Users who find errors or omissions are encouraged to inform us so that we can make the document as comprehensive as possible.


---


## VR Juggler 1.1 DR3 ##

### Changes ###
  * Reorganized the samples directory to separate simple applications from advanced examples.
  * Usability enhancements to [VRJConfig](VrjConfig.md).
  * Bug fixes in Sonix plugin loading code.
  * Improved stability of the Trackd driver.
  * Improved reliability of the Pinch Glove driver.
  * Install the ConfigUpdater tool.
  * Bug fixes to the thread sleeping code.
  * Added an abstraction for loading code at runtime.

As a reminder, the VR Juggler 1.0 math library has been replaced by the Generic Math Template Library (GMTL), found at http://ggt.sourceforge.net/. This library offers improved speed and flexibilty in the math code.

### Requirements ###

For the requirements see: SystemSetup

This page will always contain the most up-to-date requirements for each platform