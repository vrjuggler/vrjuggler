# What is this: VR Juggler
#
# Organization Credits: Iowa State University
#                       Virtual Reality Applications Center (VRAC)
#
# Contributors (Alphabetically): 
#               Albert Baker, Allen Bierbaum, Carolina Cruz-Neira, 
#               Jesse Coats, Patrick Hartling, Andrew Himberger, 
#               Chris Johanson, Christopher Just, Kevin Meinert,
#               Ben Thompson, Terry Welsh
# 

------------------
Table of contents:
------------------

    1. Getting started
        A. Supported platforms
        B. Minimum requirements (compiler, libraries, etc.)
    2. Questions about VR Juggler
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

==============================================================================
1. Getting started
==============================================================================

    A. Supported platforms

        VR Juggler is officially supported on the following platforms:

            IRIX 6.5
            RedHat Linux 7.1 and newer
            Windows NT 4.0, 2000, and XP
            Windows 98/Me

        VR Juggler is known to work on the following platforms (but is
        not officially supported):

            HP-UX 11.00
            FreeBSD 4.2 and 5.0-CURRENT (x86)
            Sun Solaris 7 (x86) with patches up through June 6, 2001
            Sun Solaris 8 (x86 and SPARC) with patches up through June 25, 2001
            Mac OS X

    B. Minimum requirements (by operating system)

        * All
            GNU make 3.78 or newer
            GNU Autoconf 2.13
            GNU Automake 1.4
            Java 2 Platform 1.2.2 or newer (only for VjControl)

        * IRIX
            MIPSpro Compilers 7.3.1.3m or newer
            OpenGL

        * RedHat Linux 7.1
            GCC 2.96 20000731 (Red Hat Linux 7.1 2.96-81)
            Mesa3D 3.2 or newer
             or 
            OpenGL 1.2 drivers

        * HP-UX 11.00
            aCC A.03.30
            OpenGL

        * Solaris 7 (patches including 108377-23)
            GCC 2.95.3 or newer
            GNU binutils 2.11.2 or newer
            OpenGL

        * Solaris 8 (patches including 108652-24 for SPARC or 108653-24 for
          x86)
            GCC 3.0 or newer
            GNU binutils 2.11.2 or newer
            OpenGL

        * Win32
            Microsoft Visual Studio 7
            Juggler Tools (http://www.vrjuggler.org/pub/juggler-tools-1.1.zip)

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

        http://www.vrjuggler.org/html/misc/faq.html

    B. Documentation

          i. Getting Started Guide (for everyone)

            http://www.vrjuggler.org/html/programmer.guide/index.html
            http://www.vrjuggler.org/html/books/getting.started.guide.pdf

         ii. Programmer's Guide (for VR Juggler application programmers)

            http://www.vrjuggler.org/html/programmer.guide/index.html
            http://www.vrjuggler.org/html/books/programmer.guide.pdf

        iii. Programmer's Class Reference (for application programmers and VR
             Juggler developers)

            http://www.vrjuggler.org/html/programmer.reference/index.html
            http://www.vrjuggler.org/html/books/programmer.reference.pdf

         iv. VjControl (configuration of VR Juggler)

            http://www.vrjuggler.org/html/vjcontrol/index.html
            http://www.vrjuggler.org/html/books/vjcontrol.guide.pdf

          v. VR Juggler Team Guide (for VR Juggler developers only)

            http://www.vrjuggler.org/html/juggler.team.guide/index.html
            http://www.vrjuggler.org/html/books/juggler.team.guide.pdf

    C. Mailing lists

         i. vrjuggler-info

             General VR Juggler questions should be sent to the
             vrjuggler-info@lists.sourceforge.net mailing list.  This is a
             general discussion and announcement list that usually does not
             have a lot of technical content.

        ii. vrjuggler-devel

            VR Juggler developer questions and questions relating to
            complex and/or technical details should be sent to the
            vrjuggler-devel@lists.sourceforge.net mailing list.

==============================================================================
3. Compiling VR Juggler from source
==============================================================================

    For a detailed description of this process, please refer to
    http://www.vrjuggler.org/html/juggler.team.guide/build/juggler/index.html
    before sending email to the mailing lists.

    The documentation for compiling VR Juggler from source is now in the
    INSTALL file.

$Id$
