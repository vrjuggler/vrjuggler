#!/bin/sh

# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
#
# Original Authors:
#   Allen Bierbaum, Christopher Just,
#   Patrick Hartling, Kevin Meinert,
#   Carolina Cruz-Neira, Albert Baker
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
# -----------------------------------------------------------------
# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$
# -----------------------------------------------------------------
#
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# Print out the target information for the build system.  These come directly
# from the comments at the top of the top-level Makefile.in.  Doing this in a
# shell script is a few orders of magnitude faster than echoing each line
# individually in the Makefile.
# -----------------------------------------------------------------------------

cat <<EOF

Primary build/install targets are:
    world           - Build and install everything.
    world-all-abi   - Build and install everything using all supported ABIs.
    buildworld      - Build everything (same as 'all').
    installworld    - Install everything (same as 'install-all').  This
                      requires that 'buildworld' be successfully completed
                      beforehand.
    release         - Build and install a release version.
    release-all-abi - Build and install a release version using all supported
                      ABIs.

Build targets are:
    all             - Build everything.
    all-abi         - Build everything using all supported ABIs.
    debug           - Build the VR Juggler libraries (dynamic and static) with
                      debugging symbols turned on.
    dbg             - Build the debugging, static copy of the libraries.
    ddso            - Build the debugging, dynamic shared object version of
                      the libraries.
    optim           - Build the VR Juggler libraries (dynamic and static) with
                      optimization flags enabled.
    opt             - Build the optimized, static version of the libraries.
    dso             - Build the dynamic shared object version of the
                      libraries.
    obj             - Build the object files using the default value for
                      \${OBJDIR}.
    gui             - Build the Java-based configuration GUI.

Library type targets are:
    static-libs     - Build the static versions of the libraries.  This target
                      depends on a correct value for \${LIBDIR} (that does not
                      have a default value).
    shared-libs     - Build the dynamic shared versions of the libraries.
                      This target depends on a correct value for \${LIBDIR}
                      (that does not have a default value).

Install targets are:
    hier            - Make the full directory hierarchy for the installation.
    install-all     - Install all versions of the libraries, all header files,
                      the Data directory, the configuration GUI and the test
                      code.
    install-all-abi - Install every ABI possible on the target platform.
    install         - Install the full debugging version of VR Juggler (with
                      both static and dynamic libraries). 
    install-debug   - Same as 'install'.
    install-dbg     - Install the debugging, static version of the libraries.
    install-ddso    - Install the debugging, dynamic shared object version of
                      the libraries.
    install-optim   - Install the full optimized version of VR Juggler (with
                      both static and dynamic libraries).
    install-opt     - Install the optimized, static version of the libraries.
    install-dso     - Install the dynamic shared object version of the
                      libraries.
    install-headers - Install only the header files.
    install-samples - Install only the sample applications.
    install-test    - Install only the test code.
    install-tools   - Install only the user tools code.
    install-data    - Install only the Data directory.
    install-gui     - Install only the configuration GUI.

Documentation generation targets:
    docs            - Build the internal and the public documentation.
    doc-internal    - Build only the internal documentation.
    doc-public      - Build only the public documentation.

Miscellaneous targets are:
    links           - Define links (for developer use only) that provide
                      functionality allowing the use of the \$VJ_BASE_DIR
                      environment variable.
    clean-links     - Remove the symlinks created by 'links'.

Clean-up targets are:
    clean           - Clean up everything (uses common 'clean' target).
    cleandepend     - Clean up the dependency files (uses common 'cleandepend'
                      target).
    clobber         - Clean up everything and remove the directories containing
                      the compiled libraries and its object files.

The default target is 'debug'.  'install' will install the debugging, static
version of the libraries.

EOF
