dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl
dnl Doozer++ is (C) Copyright 2000, 2001 by Iowa State University
dnl
dnl Original Author:
dnl   Patrick Hartling
dnl
dnl This library is free software; you can redistribute it and/or
dnl modify it under the terms of the GNU Library General Public
dnl License as published by the Free Software Foundation; either
dnl version 2 of the License, or (at your option) any later version.
dnl
dnl This library is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl Library General Public License for more details.
dnl
dnl You should have received a copy of the GNU Library General Public
dnl License along with this library; if not, write to the
dnl Free Software Foundation, Inc., 59 Temple Place - Suite 330,
dnl Boston, MA 02111-1307, USA.
dnl
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Provide command-line options and checks for Java utilities in the
dnl configuration process.  This does the substitution of the variables
dnl defined here (described below).
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_PATH_JAVA - Find the path to the Java installation.
dnl
dnl Command-line options added:
dnl     --with-jdkhome - Define the root directory for the Java installation.
dnl
dnl Variables defined:
dnl     JDK_HOME - The JDK installation directory.
dnl     JAVAC    - The path up to and including the javac(1) Java compiler.
dnl     JAR      - The path up to and including the jar(1) utility.
dnl     RMIC     - The RMI stub and skeleton compiler.
dnl ===========================================================================

dnl java.m4,v 1.4 2001/02/16 22:05:23 patrick Exp

dnl ---------------------------------------------------------------------------
dnl Find the path to the Java installation.  Substition is performed on the
dnl variables $JDK_HOME, $JAVAC and $JAR.
dnl
dnl Usage:
dnl     DPP_PATH_JAVA([os-type])
dnl
dnl Arguments:
dnl     os-type - The type of the target operating system (either UNIX or
dnl               Win32).  This is optional and will be determined if not
dnl               specified.
dnl ---------------------------------------------------------------------------
AC_DEFUN(DPP_PATH_JAVA,
[
    if test "x$1" = "x" ; then
        AC_REQUIRE([DPP_SYSTEM_SETUP])
 
        temp_os_type="$dpp_os_type"
    else
        temp_os_type="$1"
    fi

    AC_REQUIRE([DPP_WIN32_SETUP])

    dnl -----------------------------------------------------------------------
    dnl Define the root directory for the Java installation.
    dnl The default is /usr/java or what the user has defined in his or her
    dnl $JDK_HOME environment variable.  ${_JDK_HOME_ALT_LIST} must be a
    dnl colon-separated list of bin directories that could contain javac(1)
    dnl and the other JDK utilities.
    dnl -----------------------------------------------------------------------
    if test "x$JAVA_HOME" != "x" ; then
        _JDK_HOME_ALT_LIST="/usr/local/java/bin"
    else
        _JDK_HOME_ALT_LIST="${JAVA_HOME}/bin:/usr/local/java/bin"
    fi

    AC_ARG_WITH(jdkhome,
                [  --with-jdkhome=<PATH>   JDK installation directory      [default=\$JDK_HOME
                                                           then /usr/java]],
                JDK_HOME="$withval", JDK_HOME="${JDK_HOME-/usr/java}")

    JAVAC=''
    JAR=''

    dnl -----------------------------------------------------------------------
    dnl Check for the JDK (specifically try to find javac(1) in $JDK_HOME or
    dnl in ${_JDK_HOME_ALT_LIST}).
    dnl -----------------------------------------------------------------------
    AC_PATH_PROG(JAVAC, javac, no, $PATH:${JDK_HOME}/bin:${_JDK_HOME_ALT_LIST})

    dnl -----------------------------------------------------------------------
    dnl If $JAVAC has a value other than "no", set $JAR to the path to jar(1).
    dnl -----------------------------------------------------------------------
    if test "x$JAVAC" != "xno" ; then
        if test "x$temp_os_type" = "xWin32" ; then
            JAVAC=`unix2dos -p "$JAVAC"`
        fi

        JAR=`echo $JAVAC | sed 's/javac$//'`jar
        RMIC=`echo $JAVAC | sed 's/javac$//'`rmic
    fi

    dnl -----------------------------------------------------------------------
    dnl Do the substition step for the variables.
    dnl -----------------------------------------------------------------------
    AC_SUBST(JDK_HOME)
    AC_SUBST(JAVAC)
    AC_SUBST(JAR)
    AC_SUBST(RMIC)
])
