dnl ===========================================================================
dnl General macros for use within Doozer++ and by some users constructing
dnl moderately complex tests.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_PREREQ        - Complain and exit if the Doozer++ version is less
dnl                         than the given version number.
dnl     DPP_INIT          - Initialize Doozer++.
dnl     DPP_VERSION_CHECK - Compare two version numbers.
dnl ===========================================================================

dnl general.m4,v 1.5 2001/01/10 00:08:27 patrick Exp

dnl Set the version of Doozer++.
define(DPP_DPPVERSION, 1.0)

dnl ---------------------------------------------------------------------------
dnl Change the dots in NUMBER into commas.
dnl
dnl Usage:
dnl     DPP_PREREQ_SPLIT(NUMBER)
dnl
dnl Arguments:
dnl     NUMBER - A version number of the form major.minor.patch
dnl ---------------------------------------------------------------------------
define(DPP_PREREQ_SPLIT, [translit($1, ., [, ])])

dnl ---------------------------------------------------------------------------
dnl Default the ternary version number to 0 (e.g., 1, 7 -> 1, 7, 0).
dnl
dnl Usage:
dnl     DPP_PREREQ_CANON(MAJOR, MINOR [,TERNARY])
dnl ---------------------------------------------------------------------------
define(DPP_PREREQ_CANON, [$1, $2, ifelse([$3], , 0, [$3])])

dnl ---------------------------------------------------------------------------
dnl Complain and exit if version number 1 is less than version number 2.
dnl PRINTABLE2 is the printable version of version number 2.
dnl
dnl Usage:
dnl     DPP_PREREQ_COMPARE(MAJOR1, MINOR1, TERNARY1, MAJOR2, MINOR2, TERNARY2,
dnl                        PRINTABLE2)
dnl ---------------------------------------------------------------------------
define(DPP_PREREQ_COMPARE,
[ifelse(builtin([eval],
[$3 + $2 * 1000 + $1 * 1000000 < $6 + $5 * 1000 + $4 * 1000000]), 1,
[errprint(dnl
FATAL ERROR: Doozer++ version $7 or higher is required for this script
)m4exit(3)])])

dnl ---------------------------------------------------------------------------
dnl Complain and exit if the Doozer++ version is less than VERSION.
dnl
dnl Usage:
dnl     DPP_PREREQ(VERSION)
dnl
dnl Arguments:
dnl     VERSION - The Doozer++ version number of the form major.minor[.patch]
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_PREREQ,
[DPP_PREREQ_COMPARE(DPP_PREREQ_CANON(DPP_PREREQ_SPLIT(DPP_DPPVERSION)),
DPP_PREREQ_CANON(DPP_PREREQ_SPLIT([$1])), [$1])])

dnl ---------------------------------------------------------------------------
dnl Compare two version numbers separated into their components.
dnl
dnl Usage:
dnl     DPP_VERSION_COMPARE(known-major, known-minor, known-patch,
dnl                         required-major, required-minor, required-patch
dnl                         [, action-if-found [, action-if-not-found]])
dnl
dnl Arguments:
dnl     known-major         - The known major number value to be compared.
dnl     known-minor         - The known minor number value to be compared.
dnl     known-patch         - The known patch number value to be compared.
dnl     required-major      - The minimum required major number.
dnl     required-minor      - The minimum required minor number.
dnl     required-patch      - The minimum required patch number.
dnl     action-if-found     - The action to take if the known version number
dnl                           is at least the required version number.  This
dnl                           is optional.
dnl     action-if-not-found - The action to take if the known version number
dnl                           is not at least the required version number.
dnl                           This is optional.
dnl ---------------------------------------------------------------------------
define(DPP_VERSION_COMPARE,
[
    if test $(($3+$2*1000+$1*1000000<=$6+$5*1000+$4*1000000))
    then
        $7
        true
    else
        $8
        true
    fi
])

dnl ---------------------------------------------------------------------------
dnl Compare two version numbers.  The first is the version number in question,
dnl and the second is the minimum required value.
dnl
dnl Usage:
dnl     DPP_VERSION_CHECK(known-version, required-version
dnl                       [, action-if-found [, action-if-not-found]])
dnl
dnl Arguments:
dnl     known-version       - The known version number to be tested.  It must
dnl                           be of the form major.minor[.patch]
dnl     requird-version     - The minimum requird version number.  It must
dnl                           be of the form major.minor[.patch]
dnl     action-if-found     - The action to take if the known version number
dnl                           is at least the required version number.  This
dnl                           is optional.
dnl     action-if-not-found - The action to take if the known version number
dnl                           is not at least the required version number.
dnl                           This is optional.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_VERSION_CHECK,
[
    split_known_ver=`echo $1 | sed -e 's/\./ /g'`
    split_req_ver=`echo $2 | sed -e 's/\./ /g'`

    known_major=`echo $split_known_ver | cut -d' ' -f1`
    known_minor=`echo $split_known_ver | cut -d' ' -f2`
    known_patch=`echo $split_known_ver | cut -d' ' -f3`

    if test "x$known_patch" = "x" ; then
        known_patch=0
    fi

    req_major=`echo $split_req_ver | cut -d' ' -f1`
    req_minor=`echo $split_req_ver | cut -d' ' -f2`
    req_patch=`echo $split_req_ver | cut -d' ' -f3`

    if test "x$req_patch" = "x" ; then
        req_patch=0
    fi

    DPP_VERSION_COMPARE($known_major, $known_minor, $known_patch,
                        $req_major, $req_minor, $req_patch,
                        $3, $4)
])

dnl ---------------------------------------------------------------------------
dnl Initialize Doozer++.
dnl
dnl Usage:
dnl     DPP_INIT
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_INIT,
[
    topdir=`pwd`
])

dnl ---------------------------------------------------------------------------
dnl ---------------------------------------------------------------------------
dnl DPP_ACLOCAL_INCLUDE(macrodir)
AC_DEFUN(DPP_ACLOCAL_INCLUDE,
[
    test -n "$ACLOCAL_FLAGS" && ACLOCAL="$ACLOCAL $ACLOCAL_FLAGS"

    for k in $1 ; do ACLOCAL="$ACLOCAL -I $k" ; done
])

dnl ---------------------------------------------------------------------------
dnl Do final substitutions for variables defined in DPP_INIT.
dnl
dnl Usage:
dnl     DPP_SUBST
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_SUBST,
[
    AC_REQUIRE([DPP_SYSTEM_SETUP])
    AC_REQUIRE([DPP_WIN32_SETUP])

    if test "x$dpp_os_type" = "xWin32"; then
        topdir=`unix2dos -p "$topdir"`
    fi

    AC_SUBST(topdir)
])
