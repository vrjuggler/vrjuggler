#!/bin/sh

#
#  File:	   $RCSfile$
#  Date modified:  $Date$
#  Version:	   $Revision$
#
#
#                                 VR Juggler
#                                     by
#                               Allen Bierbaum
#                              Christopher Just
#                              Patrick Hartling
#                             Carolina Cruz-Neira
#                                Albert Baker
#
#                   Copyright (C) - 1997, 1998, 1999, 2000
#               Iowa State University Research Foundation, Inc.
#                             All Rights Reserved
#

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
    install-sample  - Install only the sample applications.
    install-test    - Install only the test code.
    install-tools   - Install only the user tools code.
    install-data    - Install only the Data directory.
    install-gui     - Install only the configuration GUI.

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

The default target is 'dbg'.  'install' will install the debugging, static
version of the libraries.

EOF
