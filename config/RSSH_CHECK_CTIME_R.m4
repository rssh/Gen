#@synonpsis RSSH_CHECK_CTIME_R
#  check for ctime_r system interface
#  set HAVE_CTIME_R if ctime_r is exists
#      CTIME_R_HAVE_3_ARGS if ctime_r have 3 arguments (3-rd: length of buffer)
#
#@author   (C) Ruslan Shevchenko <Ruslan@Shevchenko.Kiev.UA>, 1998
#@id $Id: RSSH_CHECK_CTIME_R.m4,v 1.6 2001-10-31 04:02:13 rssh Exp $
#
AC_DEFUN(RSSH_CHECK_CTIME_R,[
AC_REQUIRE([RSSH_ENABLE_PTHREADS])
AC_CHECK_FUNC(ctime_r,
[rssh_ctimerexists=yes
AC_DEFINE(HAVE_CTIME_R)],
[rssh_ctimerexists=no])
if test "$rssh_ctimerexists" = "yes"
then
  AC_CACHE_CHECK([whether ctime_r accepts 3 arguments],
                 [rssh_cv_ctimer3args],
[AC_LANG_SAVE
 AC_LANG_C
 AC_TRY_COMPILE([
#ifndef
#define _POSIX_PTHREAD_SEMANTICS
#endif
#include <sys/types.h>
#include <time.h>
],
[
char buff[26];
time_t t;
ctime_r(&t,buff,26);
], rssh_cv_ctimer3args=yes,rssh_cv_ctimer3args=no)
 AC_LANG_RESTORE
  ])
  if test "$rssh_cv_ctimer3args" = "yes"
  then
     AC_DEFINE(CTIME_R_HAVE_3_ARGS)
  fi
fi
]) 
