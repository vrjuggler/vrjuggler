#!/bin/sh

# -----------------------------------------------------------------------------
# $Id$
# -----------------------------------------------------------------------------
# Print out the target information for the build system.  These come directly
# from the comments at the top of the top-level Makefile.in.  Doing this in a
# shell script is a few orders of magnitude faster than echoing each line
# individually in the Makefile.
# -----------------------------------------------------------------------------

cat <<EOF

Primary build/install targets are:
    world           - Build and install everything.
    world-all-abi   - Build and install everything usnig all supported ABIs.
    buildworld      - Build everything (same as 'all').
    installworld    - Install everything (same as 'install-all').  This
                      requirs that 'buildworld' be successfully completed
                      beforehand.

Build targets are:
    all             - Build everything.
    all-abi         - Build everything using all supported ABIs.
    dbg             - Build the debugging, static copy of the libraries.
    ddso            - Build the debugging, dynamic shared object version of
                      the libraries.
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

Dependency targets are:
    depend          - Recursively build the dependencies for the source files.
    newdepend       - Recursively (and forcibly) rebuild the dependencies for
                      the source files.

Install targets are:
    install-all     - Install all versions of the libraries, all header files,
                      the Data directory, the configuration GUI and the test
                      code.
    install-all-abi - Install every ABI possible on the target platform.
    install         - Install the libraries, all header files, the Data
                      directory, the configuration GUI and the test code.
    install-dbg     - Install the debugging, static version of the libraries.
    install-ddso    - Install the debugging, dynamic shared object version of
                      the libraries.
    install-opt     - Install the optimized, static version of the libraries.
    install-dso     - Install the dynamic shared object version of the
                      libraries.
    install-headers - Install only the header files.
    install-test    - Install only the test code.
    install-data    - Install only the Data directory.
    install-gui     - Install only the configuration GUI.

Miscellaneous targets are:
    links           - Define links (for developer use only) that provide
                      functionality allowing the use of the \$VJ_BASE_DIR
                      environment variable.
    clean-links     - Remove the symlinks created by 'links'.

Clean-up targets are:
    clean           - Clean up everything (uses common 'clean' target).
    clobber         - Clean up everything and remove the directories containing
                      the compiled libraries and its object files.

The default target is 'dbg'.  'install' will install the debugging, static
version of the libraries.

EOF
