AC_ARG_ENABLE(jdk-orb,
              [  --enable-jdk-orb        Use Java IDL (requires JDK 1.4) [default=no]],
              _enable_jdk_orb="$enable_val", _enable_jdk_orb='no')

# Override the C compiler that configure would otherwise find itself.
AC_ARG_WITH(cc, [  --with-cc=<PATH>        Alternate C compiler ],
            _alt_cc="$withval", _alt_cc='none')

# Override the C++ compiler that configure would otherwise find itself.
AC_ARG_WITH(cxx, [  --with-cxx=<PATH>       Alternate C++ compiler ],
            _alt_cxx="$withval", _alt_cxx='none')

AC_ARG_WITH(jdom, [  --with-jdom=<PATH>      Path to JDOM JAR file(s) ],
            JDOM_JAR="$withval", JDOM_JAR='none')

AC_ARG_WITH(omniorb, [  --with-omniorb=<PATH>   Path to omniORB installation ],
            OMNI_ROOT="$withval", OMNI_ROOT='none')

AC_ARG_WITH(omnibin, [  --with-omnibin=<PATH>   Path to omniORB executables ],
            OMNI_BIN_DIR="$withval", OMNI_BIN_DIR='none')

AC_ARG_WITH(omnilib, [  --with-omnilib=<PATH>   Path to omniORB libraries ],
            OMNI_LIB_DIR="$withval", OMNI_LIB_DIR='none')

AC_ARG_WITH(omniver,
            [  --with-omniver=<VER>    Major version of omniORB        [default=3]],
            OMNI_VER="$withval", OMNI_VER='3')

AC_ARG_WITH(openorb,
            [  --with-openorb=<PATH>   Path to OpenORB JAR file(s) ],
            OPENORB_JAR="$withval", OPENORB_JAR='none')

AC_ARG_WITH(idl-java,
            [  --with-idl-java=<PATH>  Path to Java IDL compiler ],
            _idl_java="$withval", _idl_java='no')
