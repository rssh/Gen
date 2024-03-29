dnl @synopsis AC_CXX_PARTIAL_SPECIALIZATION
dnl
dnl If the compiler supports partial specialization,
dnl define HAVE_PARTIAL_SPECIALIZATION.
dnl
dnl @version $Id: ac_cxx_partial_specialization.m4,v 1.1 2000-07-17 13:31:29 rssh Exp $
dnl @author Luc Maisonobe
dnl
AC_DEFUN(AC_CXX_PARTIAL_SPECIALIZATION,
[AC_CACHE_CHECK(whether the compiler supports partial specialization,
ac_cv_cxx_partial_specialization,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([
template<class T, int N> class A            { public : enum e { z = 0 }; };
template<int N>          class A<double, N> { public : enum e { z = 1 }; };
template<class T>        class A<T, 2>      { public : enum e { z = 2 }; };
],[return (A<int,3>::z == 0) && (A<double,3>::z == 1) && (A<float,2>::z == 2);],
 ac_cv_cxx_partial_specialization=yes, ac_cv_cxx_partial_specialization=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_partial_specialization" = yes; then
  AC_DEFINE(HAVE_PARTIAL_SPECIALIZATION,,
            [define if the compiler supports partial specialization])
fi
])
