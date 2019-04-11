AC_DEFUN([AC_PROG_ACLOCAL], 
[AC_CHECK_PROG([ACLOCAL], [aclocal])])

AC_DEFUN([AC_FIND_ACDIR], 
[AC_CHECK_PROG([ACLOCAL], [aclocal])

AC_MSG_CHECKING([for aclocal directory])

if test "$prefix" = "NONE"; then
  prefix=$ac_default_prefix
  ac_prefix=$prefix
#else
fi

ac_datadir=`eval echo $datadir`

if test -n "$ac_cv_prog_ACLOCAL"; then
  acdir=`eval "$ac_cv_prog_ACLOCAL --print-ac-dir" 2>/dev/null`
else
  acdir=$ac_datadir/aclocal
fi

if test "${acdir#$ac_datadir}" != "$acdir"; then
  acdir='${datadir}'${acdir#$ac_datadir}
fi

if test "${acdir#$ac_prefix}" != "$acdir"; then
  acdir='${prefix}'${acdir#$ac_prefix}
fi
AC_SUBST([acdir])
AC_MSG_RESULT([$acdir])])
