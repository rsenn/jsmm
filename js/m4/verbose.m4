AC_DEFUN([AC_CHECK_VERBOSE],
[
  AC_ARG_ENABLE([verbose], [  --enable-verbose        Verbose libtool build.], [ac_cv_verbose=yes], [ac_cv_verbose=no])

  if test "$ac_cv_verbose" != "no"; then
    LIBTOOL="$LIBTOOL --verbose"
    AC_SUBST([LIBTOOL])
  fi

  AM_CONDITIONAL([VERBOSE], [test "$ac_cv_verbose" = "yes"])
])  
