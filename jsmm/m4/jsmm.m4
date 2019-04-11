AC_DEFUN([AM_PATH_JSMM],
[
ac_cv_with_jsmm=auto

AC_ARG_WITH([jsmm],
[  --with-jsmm=PREFIX        Location of the jsmm wrapper.],
[ac_cv_with_jsmm=$withval])

AC_ARG_WITH([jsmm-includes],
[  --with-jsmm-includes=DIR  Location of jsmm includes.],
[ac_cv_jsmm_includes=$withval],
[ac_cv_jsmm_includes=no])

AC_ARG_WITH([jsmm-libs],
[  --with-jsmm-libs=DIR      Location of jsmm libraries.],
[ac_cv_jsmm_libs=$withval], 
[ac_cv_jsmm_libs=no])

if test "$ac_cv_with_jsmm" != "no" ; then

  # Default prefix
  : ${JSMM_PATH=$prefix}
  
  if test "$JSMM_PATH" = "NONE"; then
    JSMM_PATH="$ac_default_prefix"
  fi
  
  # Set user-supplied prefix
  if test "$ac_cv_with_jsmm" != "auto" &&
     test "$ac_cv_with_jsmm" != "yes" &&
     test "$ac_cv_with_jsmm" != "no"; then
    JSMM_PATH="$ac_cv_with_jsmm"
  fi

  AC_CHECK_PROG([JSMM_CONFIG], [jsmm-config], [$as_dir/$ac_word$ac_exec_ext], [], [$JSMM_PATH/bin:$PATH])

  : ${JSMM_PREFIX="$JSMM_PATH"}

  AC_MSG_CHECKING([for jsmm includes])

  # Set user-supplied include dir
  if test "$ac_cv_jsmm_includes" != "yes" &&
     test "$ac_cv_jsmm_includes" != "no"; then
    JSMM_CFLAGS="-I$ac_cv_jsmm_includes"
  elif test -n "$JSMM_CONFIG"; then
    JSMM_CFLAGS=`$JSMM_CONFIG --cflags`
  fi
  
  # Default include dir
  if test -z "$JSMM_CFLAGS"; then
    JSMM_CFLAGS="-I$JSMM_PREFIX/include"
  fi
  
  AC_MSG_RESULT([$JSMM_CFLAGS])

  AC_MSG_CHECKING([for jsmm libs])
  
  # Set user-supplied library dir
  if test "$ac_cv_jsmm_libs" != "yes" &&
     test "$ac_cv_jsmm_libs" != "no"; then
    JSMM_LIBS="-L$ac_cv_jsmm_libs"
  elif test -n "$JSMM_CONFIG"; then
    JSMM_LIBS=`$JSMM_CONFIG --libs`
  fi

  # Default library dir
  if test -z "$JSMM_LIBS"; then
    JSMM_LIBS="-L$JSMM_PREFIX/lib"
  fi

  AC_MSG_RESULT([$JSMM_LIBS])
  
  GENERIC_CFLAGS="$CFLAGS"
  GENERIC_LIBS="$LIBS"
  
  CFLAGS="$JSMM_CFLAGS"
  LIBS="$JSMM_LIBS"
  
#  echo "LIBS: $LIBS"

  AC_CHECK_CXXLIBRARY([jsmm], [Engine::getImplementationVersion], [for Engine::getImplementationVersion in -ljsmm],
[ac_cv_have_jsmm=yes], [ac_cv_have_jsmm=no])

dnl  if test "$ac_cv_have_jsmm" = "no"; then
dnl#    AC_CHECK_LIB([m], [sqrt],dnl
dnl#[ac_cv_have_m=yes], [ac_cv_have_m=no])
dnl#
dnl#    if test "$ac_cv_have_m" = "yes"; then
dnl#      LIBS="-lm${LIBS:+ }$LIBS"
dnl#
dnl#      AC_CHECK_LIB([jsmm], [JSMM_ConstructObject],#
dnl#[ac_cv_have_jsmm=yes], [ac_cv_have_jsmm=no])
dnl
dnl      if test "$ac_cv_have_jsmm" = "yes"; then
dnl        LIBS="-ljsmm${LIBS:+ }$LIBS"
dnl      fi
dnl    fi
dnl  fi

  if test "$ac_cv_have_jsmm" = "no"; then
    JSMM_CFLAGS=""
    JSMM_LIBS=""
  else
    JSMM_CFLAGS="$CFLAGS"
    JSMM_LIBS="$LIBS"
  fi

  CFLAGS="$GENERIC_CFLAGS"
  LIBS="$GENERIC_LIBS"

  if test "$ac_cv_have_jsmm" = "no"; then
    AC_MSG_ERROR([The jsmm library could not be found.
It is required to compile jsmmmm, please try to specify library
and include directories.])
  fi  
fi

AC_SUBST([JSMM_CFLAGS])
AC_SUBST([JSMM_LIBS])
])
