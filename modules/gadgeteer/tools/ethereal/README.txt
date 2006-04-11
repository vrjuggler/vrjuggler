What is this?
=============

This directory contains "plug-in" code for Ethereal (www.ethereal.com) that
can be used to analyze network traffic relating to Gadgeteer devices.
The plug-ins are not really plug-ins because the are compiled into
Ethereal rather than loaded dynamically.  In either case, the more
appropriate term is "dissector" because the code dissects and presents an
easy-to-read visual representation of the packets extracted from the
network.

Protocols
=========

   BIRDNET (Ascension MotionStar)
   ------------------------------
   Description:
      BIRDNET is the network protocol used by the Ascension MotionStar
      to communicate tracking data over a network connection.

   Files:
      packet-birdnet.c
      packet-birdnet.h

   Usage:
      Unfortunately, the BIRDNET protocol uses the same port number as
      the X11 protocol.  The only way we have found to use the BIRDNET
      dissector is to remove the X11 protocol dissector from Ethereal
      when it is compiled.  This means modifying either Makefile.am or
      Makefile.in in the Ethereal source tree to remove references to
      packet-x11.c.

   Compatibility:
      The BIRDNET dissector has been tested most recently with Ethereal 
      0.9.13 on Red Hat Linux 8.0 and IRIX 6.5.  It was originally written
      in May 2001 to work with Ethereal 0.8.18.  The original version for
      Ethereal 0.8 is not publicly available.

   Limitations:
      The dissector will not decode the data sampled from the MotionStar.
      In other words, the position/angles/quaternion/matrix information
      will not be shown in Ethereal.

$Id$
