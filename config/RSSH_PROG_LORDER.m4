dnl   (C) Ruslan Shevchenko <Ruslan@Shevchenko.Kiev.UA>, 1998
dnl   $Id: RSSH_PROG_LORDER.m4,v 1.4 2001-07-04 14:25:17 rssh Exp $
dnl --------------------------------------------------------------------
dnl RSSH_PPOG_LORDER(argument)
dnl   set LORDER to system program lorder if it exists or to 
dnl  argument/'lorder-<build-os>.sh' 
dnl
AC_DEFUN(RSSH_PROG_LORDER,[
AC_REQUIRE([AC_CANONICAL_SYSTEM])dnl
AC_MSG_CHECKING( "for lorder  " )

 `which lorder > /dev/null` || LORDER=NO
 if test "x$LORDER" = xNO
 then
    case $build_os in
     linux*)
       LORDER="sh `pwd`/$1/lorder-linux.sh"
       ;;
     *)
       AC_MSG_ERROR("Can not find lorder for this os") 
       ;;
     esac
 else
      LORDER=`which lorder`
 fi

AC_MSG_RESULT($LORDER)

AC_SUBST(LORDER)

])dnl
dnl
dnl
