dnl ===========================================================================
dnl Perl-related macros.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_PERL_VER - Find a version of Perl that is at least the given
dnl                    version number.
dnl
dnl Command-line options added:
dnl     --with-perl
dnl
dnl Variables defined:
dnl     PERL - The full path to the Perl binary meeting the requirements.
dnl ===========================================================================

dnl perl.m4,v 1.2 2000/12/19 17:06:57 patrick Exp

dnl ---------------------------------------------------------------------------
dnl Find a version of Perl that is at least the given version number.
dnl Substitution is performed on the variable $PERL.
dnl
dnl Usage:
dnl     DPP_PERL_VER(version, alternate-path)
dnl
dnl Arguments:
dnl     version        - Minimum required Perl version.
dnl     alternate-path - Alternate path(s) to check for the Perl installation.
dnl                      For more than one path value, the path list should be
dnl                      space-separated.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_PERL_VER,
[
    dnl Specify a directory containing a working Perl 5.004 (or newer) binary.
    dnl There is no default since standard locations are always available.
    AC_ARG_WITH(perl,
                [  --with-perl=<PATH>      Directory containing Perl $1
                          or newer binary                 [No default]],
                dpp_user_perl_path="$withval")

    chkPerl ( ) {
        if test -x "${1}" ; then
            retval=`${1} -e 'use English; printf("%s", (($PERL_VERSION >= 5.004) ? 0 : 1));'`
        else
            retval=1
        fi

        echo $retval
    }

    dpp_perl_path='$2 /usr/local/bin /usr/bin C:/Perl/bin'

    dnl If $dpp_user_perl_path has a value, prepend that on $dpp_perl_path so
    dnl that it will be checked before the standard places.
    if test "x$dpp_user_perl_path" != "x" ; then
        dpp_perl_path="$dpp_user_perl_path $dpp_perl_path"
    fi

    AC_CACHE_CHECK(for Perl version >= $1,
        dpp_cv_perl_version,
        [ dpp_cv_perl_version='no' ;
          for dpp_dir in $dpp_perl_path ; do
              if eval "test \"`chkPerl $dpp_dir/perl`\" -eq 0" ; then
                  dpp_cv_perl_version="$dpp_dir/perl"
                  break
              elif eval "test \"`chkPerl $dpp_dir/perl5`\" -eq 0" ; then
                  dpp_cv_perl_version="$dpp_dir/perl5"
                  break
              fi
          done
        ])

    PERL="$dpp_cv_perl_version"

    AC_SUBST(PERL)
])
