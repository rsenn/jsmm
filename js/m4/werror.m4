AC_DEFUN([AC_CHECK_WERROR],
[
  AC_ARG_ENABLE([werror], [  --enable-werror         Treat warnings as errors.], [ac_cv_werror=yes], [ac_cv_werror=no])
  
  if test "$ac_cv_werror" != "no"; then
    CFLAGS="$(echo $CFLAGS | sed 's,\-Werror *,,') -Werror"
  fi

  AM_CONDITIONAL([WERROR], [test "$ac_cv_werror" = "yes"])
])  
