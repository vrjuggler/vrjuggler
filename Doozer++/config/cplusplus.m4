dnl ************* <auto-copyright.pl BEGIN do not edit this line> *************
dnl Doozer++ is (C) Copyright 2000-2008 by Iowa State University
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
dnl File:          cplusplus.m4,v
dnl Date modified: 2008/01/01 15:29:20
dnl Version:       1.24
dnl -----------------------------------------------------------------
dnl ************** <auto-copyright.pl END do not edit this line> **************

dnl ===========================================================================
dnl C++ macros.
dnl ---------------------------------------------------------------------------
dnl Macros:
dnl     DPP_STD_CPP          - Add a command line option to enable or disable
dnl                            the use of standard C++ by the C++ compiler.
dnl     DPP_CXX_TEMPLATES    - Test the C++ compiler's template-handling
dnl                            capabilities.
dnl     DPP_CXX_NAMESPACE    - Test the C++ compiler's namespace-handling
dnl                            capabilities.
dnl     DPP_CXX_HAVE_STD     - Test if the C++ compiler supports the std
dnl                            namespace and the standard C++ library.
dnl     DPP_CXX_INLINE       - Test the C++ compiler's inlining capabilities.
dnl     DPP_CXX_RTTI         - Test the C++ compiler's RTTI capabilities.
dnl     DPP_CXX_STATIC_CAST  - Test the C++ compiler's support for
dnl                            static_cast<>.
dnl     DPP_CXX_DYNAMIC_CAST - Test the C++ compiler's support for
dnl                            dynamic_cast<>.
dnl     DPP_CXX_CONST_CAST   - Test the C++ compiler's support for
dnl                            const_cast<>.
dnl     DPP_CXX_EXCEPTIONS   - Test if the C++ compiler supports exceptions.
dnl
dnl Command-line options added:
dnl     --enable-std
dnl ===========================================================================

dnl cplusplus.m4,v 1.24 2008/01/01 15:29:20 patrickh Exp

dnl ---------------------------------------------------------------------------
dnl Add a command line option to enable or disable the use of standard C++ by
dnl the C++ compiler.  This includes use of the std namespace.
dnl
dnl Usage:
dnl     DPP_STD_CPP([default-std-setting])
dnl
dnl Arguments:
dnl     default-std-setting - The default value, "yes" or "no, for the use of
dnl                           standard C++ by the compiler.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_STD_CPP],
[
   AC_BEFORE([$0], [DPP_SYSTEM_SETUP])

   AC_ARG_ENABLE(std,
                 [  --enable-std            Force the use of standard C++   [default=$1]],
                 dpp_std="$enableval", dpp_std=$1)
])

dnl ---------------------------------------------------------------------------
dnl Test the C++ compiler's template-handling capabilities.
dnl
dnl Usage:
dnl     DPP_CXX_TEMPLATES([action-if-not-capable])
dnl
dnl Arguments:
dnl     action-if-not-capable - The action to take if the C++ compiler cannot
dnl                             deal with templates.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_CXX_TEMPLATES],
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_PROG_CXX])

   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS

   AC_CACHE_CHECK([for C++ template support],
       [dpp_cv_CXX_handles_templates],
       [AC_TRY_COMPILE([template<class T> class Test {
public:
Test() {;}
};
template<class T> void f (const Test<T>&){;}],
                      [
Test<double> d;
Test<int> i;
f(d);
f(i);
return 0; ],
                      [ dpp_cv_CXX_handles_templates='yes'
                        rm -rf ./ii_files ./SunWS_cache ],
                      [dpp_cv_CXX_handles_templates='no'])])

   DPP_LANG_RESTORE

   if test "x$dpp_cv_CXX_handles_templates" = "xno" ; then
      ifelse([$1], , :, [$1])
   else
      AC_DEFINE(HAVE_TEMPLATES, ,
                [define if the C++ compiler supports basic templates])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Test the C++ compiler's support for default parameters with templates.
dnl
dnl Usage:
dnl     DPP_CXX_DEFAULT_TEMPLATE_PARAMETERS([action-if-not-supported])
dnl
dnl Arguments:
dnl     action-if-not-supported - The action to take if the C++ compiler does
dnl                               not handle default parameters for templates.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_CXX_DEFAULT_TEMPLATE_PARAMETERS],
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_PROG_CXX])
   AC_REQUIRE([DPP_CXX_TEMPLATES])

   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS

   AC_CACHE_CHECK([whether the C++ compiler supports default template parameters],
      [dpp_cv_CXX_default_template_parameters],
      [AC_TRY_COMPILE([
template<class T = double, int N = 10> class Test {
public:
    int func (void) { return 0; }
};
],
      [Test<float> obj; return obj.func();],
      [ dpp_cv_CXX_default_template_parameters='yes'
        rm -rf ./ii_files ./SunWS_cache ],
      dpp_cv_CXX_default_template_parameters='no')])

   DPP_LANG_RESTORE

   if test "x$dpp_cv_CXX_default_template_parameters" = "xno" ; then
      ifelse([$1], , :, [$1])
   else
      AC_DEFINE([HAVE_DEFAULT_TEMPLATE_PARAMETERS], ,
         [define if the C++ compiler supports default template parameters])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Test the C++ compiler's namespace-handling capabilities.
dnl
dnl Usage:
dnl     DPP_CXX_NAMESPACE([action-if-not-capable])
dnl
dnl Arguments:
dnl     action-if-not-capable - The action to take if the C++ compiler cannot
dnl                             deal with namespaces.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_CXX_NAMESPACE],
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_PROG_CXX])

   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS

   AC_CACHE_CHECK([for C++ namespace support],
      [dpp_cv_CXX_handles_namespace],
      [AC_TRY_COMPILE([ namespace dpp_test { void func (void) {;} }; ],
                     [ dpp_test::func(); ],
                     [ dpp_cv_CXX_handles_namespace='yes'
                       rm -rf ./ii_files ./SunWS_cache ],
                     [dpp_cv_CXX_handles_namespace='no'])])

   DPP_LANG_RESTORE

   if test "x$dpp_cv_CXX_handles_namespace" = "xno" ; then
      ifelse([$1], , :, [$1])
   else
      AC_DEFINE(HAVE_NAMESPACE, ,
                [define if the C++ compiler supports namespaces])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Test if the C++ compiler supports the std namespace and the standard C++
dnl library.
dnl
dnl Usage:
dnl     DPP_CXX_HAVE_STD([action-if-not-capable])
dnl
dnl Arguments:
dnl     action-if-not-capable - The action to take if the C++ compiler does
dnl                             not support the standard library.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_CXX_HAVE_STD],
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_PROG_CXX])
   AC_REQUIRE([DPP_CXX_NAMESPACE])

   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS

   AC_CACHE_CHECK([for C++ standard library support], [dpp_cv_CXX_have_std],
      [AC_TRY_COMPILE(
[
#include <iostream>
#include <map>
], ,
          [ dpp_cv_CXX_have_std='yes'
             rm -rf ./ii_files ./SunWS_cache ],
          [dpp_cv_CXX_have_std='no'])])

   DPP_LANG_RESTORE

   if test "x$dpp_cv_CXX_have_std" = "xno" ; then
      ifelse([$1], , :, [$1])
   else
      AC_DEFINE(HAVE_STD, ,
                [define if the C++ compiler supports the standard library])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Test the C++ compiler's inlining capabilities.
dnl
dnl Usage:
dnl     DPP_CXX_INLINE([action-if-not-capable])
dnl
dnl Arguments:
dnl     action-if-not-capable - The action to take if the C++ compiler cannot
dnl                             deal with the use of the inline modifier.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_CXX_INLINE],
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_PROG_CXX])

   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS

   AC_CACHE_CHECK([for C++ inline support],
      [dpp_cv_CXX_handles_inline],
      [AC_TRY_COMPILE([ inline void func (void) {;}], [ func(); ],
                     [ dpp_cv_CXX_handles_inline='yes'
                       rm -rf ./ii_files ./SunWS_cache ],
                     [dpp_cv_CXX_handles_inline='no'])])

   DPP_LANG_RESTORE

   if test "x$dpp_cv_CXX_handles_inline" = "xno" ; then
      ifelse([$1], , :, [$1])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Test the C++ compiler's RTTI capabilities.
dnl
dnl Usage:
dnl     DPP_CXX_RTTI([action-if-not-capable])
dnl
dnl Arguments:
dnl     action-if-not-capable - The action to take if the C++ compiler does
dnl                             not implement RTTI.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_CXX_RTTI],
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_PROG_CXX])

   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS

   AC_CACHE_CHECK([for C++ RTTI support], [dpp_cv_CXX_handles_rtti],
      [AC_TRY_COMPILE([#include <typeinfo>
class Base
{
public:
   Base (void) {;}
   virtual int f (void) { return 0; }
};

class Derived : public Base
{
public:
   Derived (void) {;}
   virtual int f (void) { return 1; }
}; ],
          [
Derived d;
Base* ptr = &d;
return (typeid(*ptr) == typeid(Derived)); ],
          [ dpp_cv_CXX_handles_rtti='yes'
            rm -rf ./ii_files ./SunWS_cache ],
          [dpp_cv_CXX_handles_rtti='no'])])

   DPP_LANG_RESTORE

   if test "x$dpp_cv_CXX_handles_rtti" = "xno" ; then
      ifelse([$1], , :, [$1])
   else
      AC_DEFINE(HAVE_RTTI, , [define if the C++ compiler supports RTTI])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Test the C++ compiler's support for static_cast<>.
dnl
dnl Usage:
dnl     DPP_CXX_STATIC_CAST([action-if-not-supported])
dnl
dnl Arguments:
dnl     action-if-not-supported - The action to take if the C++ compiler does
dnl                               not support static_cast<>.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_CXX_STATIC_CAST],
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_PROG_CXX])

   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS

   AC_CACHE_CHECK([whether the C++ compiler supports static_cast<>],
      [dpp_cv_CXX_static_cast],
      [AC_TRY_COMPILE([#include <typeinfo>
class Base
{
public:
   Base (void) {;}
   virtual void f (void) = 0;
};

class Derived : public Base
{
public:
   Derived (void) {;}
   virtual void f (void) {;}
};

int testFunc (Derived& obj)
{
   return 0;
}],
           [
Derived d;
Base& b = d;
Derived& d2 = static_cast<Derived&>(b);
return testFunc(d2); ],
           [ dpp_cv_CXX_static_cast='yes'
             rm -rf ./ii_files ./SunWS_cache ],
           [dpp_cv_CXX_static_cast='no'])])

   DPP_LANG_RESTORE

   if test "x$dpp_cv_CXX_static_cast" = "xno" ; then
      ifelse([$1], , :, [$1])
   else
      AC_DEFINE(HAVE_STATIC_CAST, ,
                [define if the C++ compiler supports static_cast<>])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Test the C++ compiler's support for dynamic_cast<>.
dnl
dnl Usage:
dnl     DPP_CXX_STATIC_CAST([action-if-not-supported])
dnl
dnl Arguments:
dnl     action-if-not-supported - The action to take if the C++ compiler does
dnl                               not support dynamic_cast<>.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_CXX_DYNAMIC_CAST],
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_PROG_CXX])

   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS

   AC_CACHE_CHECK([whether the C++ compiler supports dynamic_cast<>],
      [dpp_cv_CXX_dynamic_cast],
      [AC_TRY_COMPILE([#include <typeinfo>
class Base
{
public:
   Base (void) {;}
   virtual void f (void) = 0;
};

class Derived : public Base
{
public:
   Derived (void) {;}
   virtual void f (void) {;}
};],
          [
Derived d;
Base& b = d;
return (dynamic_cast<Derived*>(&b) ? 0 : 1); ],
          [ dpp_cv_CXX_dynamic_cast='yes'
            rm -rf ./ii_files ./SunWS_cache ],
          [dpp_cv_CXX_dynamic_cast='no'])])

   DPP_LANG_RESTORE

   if test "x$dpp_cv_CXX_dynamic_cast" = "xno" ; then
      ifelse([$1], , :, [$1])
   else
      AC_DEFINE(HAVE_DYNAMIC_CAST, ,
                [define if the C++ compiler supports dynamic_cast<>])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Test the C++ compiler's support for const_cast<>.
dnl
dnl Usage:
dnl     DPP_CXX_STATIC_CAST([action-if-not-supported])
dnl
dnl Arguments:
dnl     action-if-not-supported - The action to take if the C++ compiler does
dnl                               not support const_cast<>.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_CXX_CONST_CAST],
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_PROG_CXX])

   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS

   AC_CACHE_CHECK([whether the C++ compiler supports const_cast<>],
      [dpp_cv_CXX_const_cast],
      [AC_TRY_COMPILE( , [
int x = 0;
const int& y = x;
int& z = const_cast<int&>(y);
return z;],
          [ dpp_cv_CXX_const_cast='yes'
            rm -rf ./ii_files ./SunWS_cache ],
          [dpp_cv_CXX_const_cast='no'])])

   DPP_LANG_RESTORE

   if test "x$dpp_cv_CXX_const_cast" = "xno" ; then
      ifelse([$1], , :, [$1])
   else
      AC_DEFINE(HAVE_CONST_CAST, ,
                [define if the C++ compiler supports const_cast<>])
   fi
])

dnl ---------------------------------------------------------------------------
dnl Test if the C++ compiler supports exceptions.
dnl
dnl Usage:
dnl     DPP_CXX_EXCEPTIONS([action-if-not-supported])
dnl
dnl Arguments:
dnl     action-if-not-supported - The action to take if the C++ compiler does
dnl                               not support exceptions.
dnl ---------------------------------------------------------------------------
AC_DEFUN([DPP_CXX_EXCEPTIONS],
[
   AC_REQUIRE([DPP_SYSTEM_SETUP])
   AC_REQUIRE([DPP_PROG_CXX])

   DPP_LANG_SAVE
   DPP_LANG_CPLUSPLUS

   AC_CACHE_CHECK([for C++ exception support], [dpp_cv_CXX_has_exceptions],
      [AC_TRY_COMPILE( , [try { throw 1; } catch (int i) { return i; }],
         [dpp_cv_CXX_has_exceptions='yes'], [dpp_cv_CXX_has_exceptions='no'])])

   DPP_LANG_RESTORE

   if test "x$dpp_cv_CXX_has_exceptions" = "xno" ; then
      ifelse([$1], , :, [$1])
   else
      AC_DEFINE(HAVE_EXCEPTIONS, ,
                [define if the C++ compiler supports exceptions])
   fi
])
