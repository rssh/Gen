dnl@synopsis AC_CXX_STRING
dnl
dnl  check, if C++ string class is aviable.
dnl
dnl@version $Id: ac_cxx_string.m4,v 1.1 2001-06-02 12:39:28 rssh Exp $
dnl@author Ruslan Shevchenko <Ruslan@Shevchenko.Kiev.UA>
AC_DEFUN([AC_CXX_STRING],[
AC_REQUIRE([AC_CXX_NAMESPACES])

AC_MSG_CHECKING("for C++ string class")

AC_LANG_SAVE
AC_LANG_CPLUSPLUS

have_string=no
AC_CHECK_HEADER(string,have_string=yes;AC_DEFINE(HAVE_STRING))

AC_LANG_RESTORE

AC_MSG_RESULT("$have_string")

])dnl
dnl
