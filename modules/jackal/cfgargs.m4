# Enable performance testing.  Values that enable it are as follows:
#
#      SGI: Enable SGI IPC performance testing.  This defines JCCL_PERFORMANCE
#           to be JCCL_PERF_SGI
#    POSIX: Enable POSIX threads performance testing.  This defines
#           JCCL_PERFORMANCE to be JCCL_PERF_POSIX
#
# Values that disable it are "no" or "NONE".  The default is "SGI".
AC_ARG_ENABLE(performance,
              [  --enable-performance=<SGI|POSIX|NONE>
                          enable performance testing      [default=SGI] ],
              [ if test "x$enableval" = "xyes" ; then
                    _enable_perf='SGI'
                elif test "x$enableval" = "xno" ; then
                    _enable_perf='NONE'
                else
                    _enable_perf="$enableval"
                fi
              ], _enable_perf='SGI')

# -------------------------------------------- #
# --with-pkg[=arg] and --without-pkg arguments #
# -------------------------------------------- #

# Define the root directory for the Xerces-c installation.
AC_ARG_WITH(xercesroot,
            [  --with-xercesroot=<PATH>
                          Xerces-c installation directory [default=/usr]],
            XERCESROOT="$withval", XERCESROOT="/usr")

AC_ARG_WITH(xerceslib,
            [  --with-xerceslib=<NAME>
                          Xerces-c base library name      [default=xerces-c]],
            XERCES_LIB="$withval", XERCES_LIB='xerces-c')
