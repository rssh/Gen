dnl @synopsis AC_CXX_TYPENAME
dnl
dnl If the compiler recognizes the typename keyword, define HAVE_TYPENAME.
dnl
dnl @version $Id: ac_cxx_typename.m4,v 1.1 2000-10-10 21:16:33 rssh Exp $
dnl @author Luc Maisonobe
dnl
AC_DEFUN(AC_CXX_TYPENAME,
[AC_CACHE_CHECK(whether the compiler recognizes typename,
ac_cv_cxx_typename,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([template<typename T>class X {public:X(){}};],
[X<float> z; return 0;],
 ac_cv_cxx_typename=yes, ac_cv_cxx_typename=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_typename" = yes; then
  AC_DEFINE(HAVE_TYPENAME,,[define if the compiler recognizes typename])
fi
])
