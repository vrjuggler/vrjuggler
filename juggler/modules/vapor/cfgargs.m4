# Define which threading model to use.  Possible values are "SPROC", "POSIX",
# or "NSPR".
# The default is "SPROC".
AC_ARG_ENABLE(subsystem,
              [  --enable-subsystem=<SPROC|POSIX|NSPR>
                          Define OS abstraction subsystem],
              SUBSYSTEM="$enableval", SUBSYSTEM='none')

AC_ARG_ENABLE(simulator,
              [  --enable-simulator      Compile simulated sockets layer ],
              SIMULATOR="$enableval", SIMULATOR='no')

# Rudimentary spell checking based on the first letter in the $SUBSYSTEM name.
case $SUBSYSTEM in
    S*)
        SUBSYSTEM='SPROC'
        ;;
    P*)
        SUBSYSTEM='POSIX'
        ;;
    N*)
        SUBSYSTEM='NSPR'
        ;;
esac

# This enables the use of so-called "NSPR threads".  By default, NSPR is not
# compiled to use NSPR threads, so we do not want to use them by default
# either.
DPP_ENABLE_NSPR_THREADS(no)

# Force the use of GCC as the compiler.
# NOTE: This is not fully implemented yet for any platform, but it is
# partially usable on Windows.
DPP_WITH_GCC(no)

AC_ARG_WITH(boost,
            [  --with-boost=<PATH>     Boost installation              [default=<JUGGLER>]],
            _with_boost="$withval", _with_boost='no')
