#@synonpsis RSSH_CHECK_PTHREADS
#  check for pthreads system interfaces.
#  set CFLAGS_PTHREADS,  CXXFLAGS_PTHREADS and LIBS_PTHREADS to
#  flags to compiler option, which denote multithread program compilation 
# (if one exists),
#  and multithread library, if one required.
#
#@author   (C) Ruslan Shevchenko <Ruslan@Shevchenko.Kiev.UA>, 1998
#@id $Id: RSSH_CHECK_PTHREADS.m4,v 1.7 2008-01-08 17:15:00 rssh Exp $
#
AC_DEFUN(RSSH_CHECK_PTHREADS,[
AC_REQUIRE([AC_CANONICAL_SYSTEM])dnl
AC_REQUIRE([RSSH_CHECK_SUNPRO_C])dnl
AC_REQUIRE([RSSH_CHECK_SUNPRO_CC])dnl
AC_CHECK_HEADER(pthread.h,AC_DEFINE(HAVE_PTHREAD_H))
AC_MSG_CHECKING(parameters for using pthreads: )
if test x$rssh_cv_check_sunpro_c = xyes
then
  CFLAGS_PTHREADS="-mt"
fi
if test x$rssh_cv_check_sunpro_cc = xyes
then
  CXXFLAGS_PTHREADS="-mt"
fi
case $build_os in
  freebsd*)
    CFLAGS_PTHREADS="-pthread"
    CXXFLAGS_PTHREADS="-pthread"
    freebsd_pthreads=yes
    ;;
  *)
    freebsd_pthreads=no
    ;;
esac
AC_CHECK_LIB(pthread,pthread_create, LIBS_PTHREADS="-lpthread")
AC_CHECK_LIB(posix4,nanosleep, LIBS_PTHREADS="$LIBS_PTHREADS -lposix4")
AC_MSG_RESULT([flags $CFLAGS_PTHREADS\;libs: $LIBS_PTHREADS])
])dnl
