dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2005 by Iowa State University
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
dnl -----------------------------------------------------------------
dnl File:          java.m4,v
dnl Date modified: 2005/05/02 21:55:31
dnl Version:       1.54
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl Provide command-line options and checks for Java utilities in the
dnl configuration process.  This does the substitution of the variables
dnl defined here (described below).
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_PATH_JAVA - Find the path to the Java installation.
dnl     DPP_CHECK_JNI - Verify the installation of the libraries needed for
dnl                     compiling JNI code.
dnl
dnl     DPP_CHECK_JAVA_METHOD        - Determine if the named class contains
dnl                                    the non-static method.
dnl     DPP_CHECK_JAVA_STATIC_METHOD - Determine if the named class contains
dnl                                    the static method.
dnl
dnl Command-line options added:
dnl     --with-jdkhome - Define the root directory for the Java installation.
dnl
dnl Variables defined:
dnl     JDK_HOME - The JDK installation directory.
dnl     JAVA     - The path up to and including the java(1) command.
dnl     JAVAC    - The path up to and including the javac(1) Java compiler.
dnl     JAVADOC  - The path up to and including the javadoc(1) utility.
dnl     JAVAH    - The path up to and including the javah(1) utility.
dnl     JAVAP    - The path up to and including the javap(1) utility.
dnl     JAR      - The path up to and including the jar(1) utility.
dnl     RMIC     - The RMI stub and skeleton compiler.
dnl     JNI_INC  - The include paths necessary for JNI.
dnl     JNI_LIB  - The library which needs to be statically linked for JNI.
dnl     JCPS     - Java classpath separator character (: on UNIX, ; on Win32).
dnl ===========================================================================

dnl java.m4,v 1.54 2005/05/02 21:55:31 patrickh Exp

dnl ---------------------------------------------------------------------------
dnl Find the path to the Java installation.  Substition is performed on the
dnl variables $JDK_HOME, $JAVAC and $JAR.  If a Java installation is not found,
dnl set $JDK_HOME to 'none' to indicate error.  The search for a Java
dnl installation is performed in the following order:
dnl
dnl     1. Search path given on the command line (--with-jdkhome)
dnl     2. Search in the path(s) given as arguments to this macro
dnl     3. Search in $JDK_HOME (if it is set)
dnl     4. Search in the user's path
dnl     5. Search in a preset list of common locations
dnl
dnl Usage:
dnl     DPP_PATH_JAVA([path [, action-if-found [, action-if-not-found ]]]) 
dnl
dnl Arguments:
dnl     path - A colon-separated list of directories giving possible
dnl            locations of the Java installation.  These must be directories
dnl            where javac can be found.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_PATH_JAVA],
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_WIN32_SETUP])

   GIVEN_JDK_LIST=''

   dnl -----------------------------------------------------------------------
   dnl If an argument was given, assign it to $GIVEN_JDK_LIST and append a
   dnl ':' so that we can prepend it to the search path given to AC_PATH_PROG
   dnl below.
   dnl -----------------------------------------------------------------------
   if test "x$1" != "x" ; then
      GIVEN_JDK_LIST="$1:"
   fi

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
               [  --with-jdkhome=<PATH>   JDK installation directory      [default=$JDK_HOME
                                                           then /usr/java]],
               jdkhome="$withval")

   if test "x$jdkhome" != "x" ; then
      cmdline_path="$jdkhome/bin:"
      JDK_HOME="$jdkhome"
   fi

   if test "x$OS_TYPE" = "xWin32" ; then
      JDK_HOME=`unixpath -p "$JDK_HOME"`
      JCPS=';'
   else
      JCPS=':'
   fi

   JAVA=''
   JAVAC=''
   JAVADOC=''
   JAVAH=''
   JAVAP=''
   JAR=''

   dnl -----------------------------------------------------------------------
   dnl Check for the JDK (specifically try to find javac(1) in $JDK_HOME or
   dnl in ${_JDK_HOME_ALT_LIST}).  The order we want here is that a path to
   dnl Java specified on the command line supercedes everything else.  This
   dnl is followed by arguments passed to this macro; then the $JDK_HOME
   dnl environment variable; then the user's path; and finally the simple
   dnl list of possible alternates.
   dnl -----------------------------------------------------------------------
   dpp_temp_javac_search_path=${cmdline_path}${GIVEN_JDK_LIST}${JDK_HOME}/bin:${PATH}:${_JDK_HOME_ALT_LIST}
   AC_PATH_PROG([JAVAC], [javac], [no], [${dpp_temp_javac_search_path}])

   dnl -----------------------------------------------------------------------
   dnl If $JAVAC has a value other than "no", extract the value of $JDK_HOME
   dnl from it.  Also use the path to $JAVAC as the basis for the path to
   dnl jar, javah, and rmic.
   dnl -----------------------------------------------------------------------
   if test "x$JAVAC" != "xno" ; then
      dpp_temp_JDK_HOME=`echo $JAVAC | sed -e 's|/bin/javac$||'`

      if test "x$JDK_HOME" != "x$dpp_temp_JDK_HOME" ; then
         if test ! -x "$JDK_HOME/bin/javac" ; then
            JDK_HOME="$dpp_temp_JDK_HOME"
         fi
      fi

      if test "x$OS_TYPE" = "xWin32" ; then
         JDK_HOME=`dospath -p "$JDK_HOME"`
         JAVAC=`dospath -p "$JAVAC"`
      fi

      JAVA=`echo $JAVAC | sed -e 's|javac$||'`java
      JAVADOC=`echo $JAVAC | sed -e 's|javac$||'`javadoc
      JAR=`echo $JAVAC | sed -e 's|javac$||'`jar
      RMIC=`echo $JAVAC | sed -e 's|javac$||'`rmic

      ifelse([$2], , :, [$2])
   else
      JDK_HOME='none'
      ifelse([$3], , :, [$3])
   fi

   dnl -----------------------------------------------------------------------
   dnl Do the substition step for the variables.
   dnl -----------------------------------------------------------------------
   AC_SUBST(JCPS)
   AC_SUBST(JDK_HOME)
   AC_SUBST(JAVA)
   AC_SUBST(JAVAC)
   AC_SUBST(JAVADOC)
   AC_SUBST(JAR)
   AC_SUBST(RMIC)
])

dnl ---------------------------------------------------------------------------
dnl Verify that the available JDK is at least the required version.
dnl
dnl Usage:
dnl     DPP_JDK_VERSION(jdk-home, required-version, [, action-if-found [, action-if-not-found ]])
dnl
dnl Arguments:
dnl     jdk-home            - JDK installation directory.
dnl     required-version    - The minimum required JDK version.
dnl     action-if-found     - The action to take if the JNI libraries are
dnl                           found.
dnl     action-if-not-found - The action to take if the JNI libraries are not
dnl                           found.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_JDK_VERSION],
[
   AC_REQUIRE([DPP_PATH_JAVA])

   if test -x "$1/bin/java" ; then
      dpp_jdk_ver_line=`$1/bin/java -version 2>&1 | grep 'java version'`

      changequote(<<, >>)
      dpp_jdk_ver=`echo $dpp_jdk_ver_line | sed -e 's/^.*"[^0-9]*\([0-9][.0-9]*\).*"$/\1/'`
      changequote([, ])

      DPP_VERSION_CHECK_MSG_NO_CACHE([JDK], [$dpp_jdk_ver], [$2], [$3], [$4])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Verify the installation of the libraries needed for compiling JNI code.
dnl
dnl Usage:
dnl     DPP_CHECK_JNI([jdk-home [, action-if-found [, action-if-not-found [, extra-libraries ]]]])
dnl
dnl Arguments:
dnl     jdk-home            - JDK installation directory.
dnl     action-if-found     - The action to take if the JNI libraries are
dnl                           found.
dnl     action-if-not-found - The action to take if the JNI libraries are not
dnl                           found.
dnl     extra-libraries     - Any extra libraries or library paths needed for
dnl                           the JNI code.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_CHECK_JNI],
[
   AC_ARG_WITH(jni-incdir, [  --with-jni-incdir=<val> JNI include path(s)],
               JNI_INC="$withval")

   AC_ARG_WITH(jni-libdir,
               [  --with-jni-libdir=<val> JNI libraries and path(s)],
               JNI_LDFLAGS="$withval")

   AC_ARG_WITH(jvm-lib, [  --with-jvm-lib=<val>    JVM shared library name],
               JVM_LIB="$withval")

   AC_ARG_WITH(java-threads,
               [  --with-java-threads=<green|native>
                          Java threading type             [default=native]],
               dpp_java_threads="$withval", dpp_java_threads='native')

   dnl To help avoid typos, check the first letter of the value given for
   dnl $dpp_java_threads and assign the full string based on the result.
   case $dpp_java_threads in
      g*)
         dpp_java_threads='green'
         ;;
      n*)
         dpp_java_threads='native'
         ;;
      *)
         AC_MSG_ERROR([*** Unknown Java thread type $dpp_java_thread ***])
         ;;
   esac

   dnl Sanity check for Win32.
   if test "x$OS_TYPE" = "xWin32" -a "x$dpp_java_threads" = "xgreen" ; then
      AC_MSG_WARN([*** Win32 JVM only supports native threads ***])
      dpp_java_threads='native'
   fi

   dnl If no value was given for the first argument, set $JDK_HOME to the
   dnl default value of /usr/java unless it already has a value.  Otherwise,
   dnl set $JDK_HOME to the value given in the first argument.
   ifelse([$1], , : ${JDK_HOME=/usr/java}, JDK_HOME=$1)

   dnl -----------------------------------------------------------------------
   dnl SJJ added code
   dnl -----------------------------------------------------------------------
   : ${JNI_INC_PATH=$JDK_HOME/include}

   JNI_EXTRA_LIBS=$4

   case $target_cpu in
      i*86)
         dpp_jni_arch=i386
         ;;
      *)
         dpp_jni_arch="$target_cpu"
         ;;
   esac

   case "$PLATFORM" in
      AIX)
         : ${JNI_INC=-I$JNI_INC_PATH -I$JNI_INC_PATH/aix}
         ;;
      Darwin)
         : ${JNI_INC=-I/System/Library/Frameworks/JavaVM.framework/Headers}
         : ${JVM_LIB=-framework JavaVM}
         ;;
      FreeBSD)
         : ${JNI_INC=-I$JNI_INC_PATH -I$JNI_INC_PATH/freebsd}
         : ${JNI_LDFLAGS=-L$JDK_HOME/jre/lib/$dpp_jni_arch/${dpp_java_threads}_threads -L$JDK_HOME/jre/lib/$dpp_jni_arch/classic -L$JDK_HOME/jre/lib/$dpp_jni_arch -L$JDK_HOME/jre/lib/$dpp_jni_arch/client}
         : ${JVM_LIB=jvm}
         JNI_EXTRA_LIBS="$JNI_EXTRA_LIBS -pthread"
         ;;
      HP)
         : ${JNI_INC=-I$JNI_INC_PATH -I$JNI_INC_PATH/hp-ux}
         : ${JNI_LDFLAGS=-L$JDK_HOME/lib/PA_RISC/${dpp_java_threads}_threads -L$JDK_HOME/lib/PA_RISC/classic -L$JDK_HOME/lib/PA_RISC -L$JDK_HOME/lib/PA_RISC/client}
         : ${JVM_LIB=jvm}
         ;;
      IRIX)
         : ${JNI_INC=-I$JNI_INC_PATH -I$JNI_INC_PATH/irix}
         : ${JNI_LDFLAGS=-L$JDK_HOME/lib32/sgi/${dpp_java_threads}_threads -L$JDK_HOME/lib32/sgi/classic -L$JDK_HOME/lib32/sgi -L$JDK_HOME/lib32/sgi/client}
         : ${JVM_LIB=jvm}
         ;;
      Linux)
         : ${JNI_INC=-I$JNI_INC_PATH -I$JNI_INC_PATH/linux}
         : ${JNI_LDFLAGS=-L$JDK_HOME/jre/lib/$dpp_jni_arch/${dpp_java_threads}_threads -L$JDK_HOME/jre/lib/$dpp_jni_arch/classic -L$JDK_HOME/jre/lib/$dpp_jni_arch -L$JDK_HOME/jre/lib/$dpp_jni_arch/client -L$JDK_HOME/jre/lib/$dpp_jni_arch/server}
         : ${JVM_LIB=jvm}
         ;;
      alpha-osf)
         ;;
      Solaris)
         : ${JNI_INC=-I$JNI_INC_PATH -I$JNI_INC_PATH/solaris}
         : ${JNI_LDFLAGS=-L$JDK_HOME/jre/lib/$dpp_jni_arch/${dpp_java_threads}_threads -L$JDK_HOME/jre/lib/$dpp_jni_arch}
         : ${JVM_LIB=jvm}
         ;;
      Win*)
         : ${JNI_INC=-I`dospath -p "$JNI_INC_PATH"` -I`dospath -p "$JNI_INC_PATH"/win32`}
         : ${JVM_LIB=`dospath -p "$JDK_HOME/lib/jvm.lib"`}
         ;;
      UNKNOWN)
         ;;
   esac

   dnl Ensure that $JAVAC has a value before it is used.
   : ${JAVAC=$JDK_HOME/bin/javac}

   JAVAH=`echo $JAVAC | sed -e 's|javac$||'`javah
   JAVAP=`echo $JAVAC | sed -e 's|javac$||'`javap

   dnl Now that the paths are all defined, verify that the function
   dnl JNI_CreateJavaVM() is avaialble.  This is used to ensure that the
   dnl above settings are correct for the host platform.
   DPP_LANG_SAVE
   DPP_LANG_C

   dpp_save_CPPFLAGS="$CPPFLAGS"
   dpp_save_LDFLAGS="$LDFLAGS"
   dpp_save_LIBS="$LIBS"

   CPPFLAGS="$CPPFLAGS $JNI_INC"
   LDFLAGS="$LDFLAGS $JNI_LDFLAGS"

   if test "x$PLATFORM" = "xDarwin" ; then
      LIBS="$LIBS $JVM_LIB"
      AC_CACHE_CHECK([for JNI_CreateJavaVM in JavaVM framework],
                     [dpp_cv_JNI_CreateJavaVM_javavm_fw],
                     [AC_TRY_LINK([#include <jni.h>],
                        [JNI_CreateJavaVM(0, 0, 0);],
                        [dpp_cv_JNI_CreateJavaVM_javavm_fw='yes'],
                        [dpp_cv_JNI_CreateJavaVM_javavm_fw='no'])])
      dpp_jni_libs="$dpp_cv_JNI_CreateJavaVM_javavm_fw"
   elif test "x$OS_TYPE" = "xUNIX" ; then
      AC_CHECK_LIB([$JVM_LIB], [JNI_CreateJavaVM],
         [AC_CHECK_HEADER([jni.h], [dpp_jni_libs='yes'], [dpp_jni_libs='no'])],
         [dpp_jni_libs='no'], [$JNI_EXTRA_LIBS])
   fi

   CPPFLAGS="$dpp_save_CPPFLAGS"
   LDFLAGS="$dpp_save_LDFLAGS"
   LIBS="$dpp_save_LIBS"

   DPP_LANG_RESTORE

   if test "x$dpp_jni_libs" = "xyes" ; then
      if test "x$PLATFORM" = "xDarwin" ; then
         JNI_LIB="$JVM_LIB"
      else
         JNI_LIB="-l$JVM_LIB $JNI_EXTRA_LIBS"
      fi

      ifelse([$2], , :, [$2])
   else
      ifelse([$3], , :, [$3])
   fi

   AC_SUBST(JAVAH)
   AC_SUBST(JAVAP)
   AC_SUBST(JNI_INC)
   AC_SUBST(JNI_LDFLAGS)
   AC_SUBST(JNI_LIB)
])

dnl ---------------------------------------------------------------------------
dnl Determine if the named class contains the given static method.  The method
dnl can take arguments which must be declared using Java syntax.
dnl
dnl Usage:
dnl     DPP_CHECK_JAVA_STATIC_METHOD(class, method [, arg-decls, [, action-if-found [, action-if-not-found [, classpath]]]])
dnl
dnl Arguments:
dnl     class               - The class that should contain the static method.
dnl     method              - The method being looked up.  It must be passed
dnl                           using the full calling syntax except for the
dnl                           class name.  For example, pass 'staticMethod()'
dnl                           for the method staticMethod() that takes no
dnl                           arguments.
dnl     arg-decls           - Declarations for the method's arguments.  This
dnl                           can be any valid Java code.
dnl     action-if-found     - The action(s) to take if the class does contain
dnl                           the method.
dnl     action-if-not-found - The action(s) to take if the class does not
dnl                           contain the method.
dnl     classpath           - The classpath to use for looking up the class.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_CHECK_JAVA_STATIC_METHOD],
[
   DPP_LANG_SAVE
   DPP_LANG_JAVA

   dnl The following is basically an expansion of the AC_CACHE_CHECK macro.
   dnl This was necessary because the variable name depends on the class and
   dnl the method being checked.

   dnl Construct the variable name.  All non-alphanumeric characters are
   dnl converted to _'s.  
   dnl XXX: This long sed command sucks, but I can't seem to get changequote()
   dnl to do what I need...
   dpp_cache_var=`echo dpp_cv_'$1.$2' | sed -e 's/\./_/g' -e 's/(/_/g' -e 's/)/_/g' -e 's/ /_/g'`

   dnl Backwards compatibility with Autoconf 2.13.
   : ${ECHO_N:=$ac_n}
   : ${ECHO_C:=$ac_c}

   AC_MSG_CHECKING([for $2 in $1])

   dnl Check cached value (based on AC_CACHE_VAL).
   if eval "test \"`echo '$''{'$dpp_cache_var'+set}'`\" = set" ; then
      echo $ECHO_N "(cached) $ECHO_C" 1>&AC_FD_MSG
   dnl The value was not cached.
   else
      dpp_class_name='ConfTest'

      dnl Create the .java file that will be compiled.
      cat > $dpp_class_name.java <<STATIC_EOF
public class $dpp_class_name
{
   public static void main (String args[])
   {
      $3;
      $1.$2;
   }
};
STATIC_EOF

      if test "x$6" != "x" ; then
         dpp_classpath="-classpath $6"
      fi 

      dpp_java_compile='${JAVAC-javac} $dpp_classpath $dpp_class_name.java 1>&AC_FD_CC'

      dnl Try compiling the Java test file.  If it succeeded, set the
      dnl variable to 'yes'.
      if (eval $dpp_java_compile) 2>&AC_FD_CC; then
         eval "$dpp_cache_var=yes"
      dnl If the test file failed to compile, set the variable to 'no'.
      else
         eval "$dpp_cache_var=no"
         echo "configure: failed program was:" >&AC_FD_CC
         cat $dpp_class_name.java >&AC_FD_CC
      fi

      rm -f ${dpp_class_name}.*
   fi

   dpp_test_result=`eval echo '$''{'$dpp_cache_var'}'`
   AC_MSG_RESULT([$dpp_test_result])

   DPP_LANG_RESTORE

   if test "x$dpp_test_result" = "xyes" ; then
      ifelse([$4], , :, [$4])
   else
      ifelse([$5], , :, [$5])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Determine if the named class contains the given non-static method.  The
dnl method can take arguments which must be declared using Java syntax.
dnl
dnl Usage:
dnl     DPP_CHECK_JAVA_METHOD(class, method [, arg-decls, [, action-if-found [, action-if-not-found [, classpath]]]])
dnl
dnl Arguments:
dnl     class               - The class that should contain the non-static
dnl                           method.
dnl     method              - The method being looked up.  It must be passed
dnl                           using the full calling syntax except for an
dnl                           object name.  For example, pass 'myMethod()'
dnl                           for the method myMethod() that takes no
dnl                           arguments.
dnl     arg-decls           - Declarations for the method's arguments.  This
dnl                           can be any valid Java code.
dnl     action-if-found     - The action(s) to take if the class does contain
dnl                           the method.
dnl     action-if-not-found - The action(s) to take if the class does not
dnl                           contain the method.
dnl     classpath           - The classpath to use for looking up the class.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_CHECK_JAVA_METHOD],
[
   DPP_LANG_SAVE
   DPP_LANG_JAVA

   dnl The following is basically an expansion of the AC_CACHE_CHECK macro.
   dnl This was necessary because the variable name depends on the class and
   dnl the method being checked.

   dnl Construct the variable name.  All non-alphanumeric characters are
   dnl converted to _'s.
   dnl XXX: This long sed command sucks, but I can't seem to get changequote()
   dnl to do what I need...
   dpp_cache_var=`echo dpp_cv_'$1.$2' | sed -e 's/\./_/g' -e 's/(/_/g' -e 's/)/_/g' -e 's/ /_/g'`

   dnl Backwards compatibility with Autoconf 2.13.
   : ${ECHO_N:=$ac_n}
   : ${ECHO_C:=$ac_c}

   AC_MSG_CHECKING([for $2 in $1])

   dnl Check cached value (based on AC_CACHE_VAL).
   if eval "test \"`echo '$''{'$dpp_cache_var'+set}'`\" = set" ; then
      echo $ECHO_N "(cached) $ECHO_C" 1>&AC_FD_MSG
   dnl The value was not cached.
   else
      dpp_class_name='ConfTest'

      dnl Create the .java file that will be compiled.
      cat > $dpp_class_name.java <<NONSTATIC_EOF
public class $dpp_class_name
{
   public static void main (String args[])
   {
      $3;
      $1 obj = new $1();
      obj.$2;
   }
};
NONSTATIC_EOF

      if test "x$6" != "x" ; then
         dpp_classpath="-classpath $6"
      fi 

      dpp_java_compile='${JAVAC-javac} $dpp_classpath $dpp_class_name.java 1>&AC_FD_CC'

      dnl Try compiling the Java test file.  If it succeeded, set the
      dnl variable to 'yes'.
      if (eval $dpp_java_compile) 2>&AC_FD_CC; then
         eval "$dpp_cache_var=yes"
      dnl If the test file failed to compile, set the variable to 'no'.
      else
         eval "$dpp_cache_var=no"
         echo "configure: failed program was:" >&AC_FD_CC
         cat $dpp_class_name.java >&AC_FD_CC
      fi

      rm -f ${dpp_class_name}.*
   fi

   dpp_test_result=`eval echo '$''{'$dpp_cache_var'}'`
   AC_MSG_RESULT([$dpp_test_result])

   DPP_LANG_RESTORE

   if test "x$dpp_test_result" = "xyes" ; then
      ifelse([$4], , :, [$4])
   else
      ifelse([$5], , :, [$5])
   fi
])
