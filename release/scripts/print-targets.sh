#!/bin/sh

cat <<EOF

Build targets are:
    all             - Build everything.
    all-abi         - Build everything using all supported ABIs.
    dbg             - Build a debugging, static copy of the library.
    ddso            - Build a debugging, dynamic shared object version of the
                      library.
    opt             - Build an optimized, static copy of the library.
    dso             - Build a dynamic shared object version of the library.
    obj             - Build the object files using the default value for
                      \${OBJDIR}.
    gui             - Build the Java-based configuration GUI.

Library type targets are:
    lib-static      - Build the static version of the library.  This target
                      depends on a correct value for \${OBJDIR} (whose default
                      value is set in Makefile.base).
    lib-dynamic     - Build the dynamic shared version of the library.  This
                      target depends on a correct value for \${OBJDIR} (whose
                      default value is set in Makefile.base).

Dependency targets are:
    depend          - Recursively build the dependencies for the source files.
    newdepend       - Recursively (and forcibly) rebuild the dependencies for
                      the source files.

Install targets are:
    install-all     - Install all versions of the library, all header files,
                      the Data directory, the configuration GUI and the test
                      code.
    install-all-abi - Install every ABI possible on the target platform.
    install         - Install the library, all header files, the Data
                      directory, the configuration GUI and the test code.
    install-dbg     - Install the debugging, static version of the library.
    install-ddso    - Install the debugging, dynamic shared object version of
                      the library.
    install-opt     - Install the optimized, static version of the library.
    install-dso     - Install the dynamic shared object version of the library.
    install-headers - Install only the header files.
    install-test    - Install only the test code.
    install-data    - Install only the Data directory.
    install-gui     - Install only the configuration GUI.

Miscellaneous targets are:
    links           - Define links (for developer use only) that provide
                      functionality allowing the use of the \$VJ_BASE_DIR
                      environment variable.  This target depends on a correct
                      value for \${OBJDIR} (whose default value is set in
                      Makefile.base).
    clean-links     - Remove the symlinks created by 'links'.

Clean-up targets are:
    clean           - Clean up everything (uses common 'clean' target).
    clobber         - Clean up everything and remove the directory containing
                      the compiled library.

The default target is 'dbg'.  'install' will install the debugging, static
version of the library.

EOF
