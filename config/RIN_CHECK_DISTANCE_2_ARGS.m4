dnl @synopsis RIN_CHECK_DISTANCE_2_ARGS
dnl
dnl Check wheither we could use 2-arguments call to distance
dnl Defines DISTANCE_HAVE_2_ARGS.
dnl
dnl @version $Id: RIN_CHECK_DISTANCE_2_ARGS.m4,v 1.2 2003-03-29 16:00:46 rin Exp $
dnl @author Bannij Michael
dnl
AC_DEFUN(RIN_CHECK_DISTANCE_2_ARGS,
[AC_CACHE_CHECK(whether the distance has 2 arguments,
 distance_allow_2_argument_calls,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <list>
void checkIfDistanceHas2args(
 std::list<int>::iterator l1,
 std::list<int>::iterator l2)
{std::distance(l1,l2);};],
[],
 distance_allow_2_argument_calls=yes,
 distance_allow_2_argument_calls=no)
 AC_LANG_RESTORE
])
if test "$distance_allow_2_argument_calls" = yes; then
  AC_DEFINE(DISTANCE_HAVE_2_ARGS,,[define when the distance declared with 2 arguments])
fi
])
