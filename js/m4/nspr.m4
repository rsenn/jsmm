
AC_DEFUN([AM_PATH_NSPR],
[
ac_cv_with_nspr=auto

AC_ARG_WITH([nspr],
[  --with-nspr=PREFIX        Mozillas NSPR library.],
[ac_cv_with_nspr=$withval])

AC_ARG_WITH([nspr-includes],
[  --with-nspr-includes=DIR  Location of NSPR includes.],
[ac_cv_nspr_includes=$withval], 
[ac_cv_nspr_includes=no])

AC_ARG_WITH([nspr-libs],
[  --with-nspr-libs=DIR      Location of NSPR libraries.],
[ac_cv_nspr_libs=$withval], 
[ac_cv_nspr_libs=no])

if test "$ac_cv_with_nspr" != "no" ; then

  # Default prefix
  NSPR_PREFIX="${prefix}"
  
  if test "$NSPR_PREFIX" = "NONE"; then
    NSPR_PREFIX=$ac_default_prefix
  fi
  
  # Set user-supplied prefix
  if test "$ac_cv_with_nspr" != "auto" &&
     test "$ac_cv_with_nspr" != "yes" &&
     test "$ac_cv_with_nspr" != "no"; then
    NSPR_PREFIX="$ac_cv_with_nspr"
  fi
  
  # Default include dir
  if test "$NSPR_PREFIX" != "/usr"; then
    NSPR_CFLAGS="-I$NSPR_PREFIX/include"
  fi

  # Set user-supplied include dir
  if test "$ac_cv_nspr_includes" != "yes" &&
     test "$ac_cv_nspr_includes" != "no"; then
    NSPR_CFLAGS="-I$ac_cv_nspr_includes"
  fi
  
  # Default library dir
  if test "$NSPR_PREFIX" != "/usr"; then
    NSPR_LIBS="-L$NSPR_PREFIX/lib"
  fi

  # Set user-supplied library dir
  if test "$ac_cv_nspr_libs" != "yes" &&
     test "$ac_cv_nspr_libs" != "no"; then
    NSPR_LIBS="-L$ac_cv_nspr_libs"
  fi
  
  GENERIC_CFLAGS="$CFLAGS"
  GENERIC_LIBS="$LIBS"
  
  CFLAGS="$NSPR_CFLAGS"
  LIBS="$NSPR_LIBS"
  
  AC_CHECK_LIB([nspr4], [PR_Initialize],
[ac_cv_nspr=yes], [ac_cv_nspr=no])

  if test "$ac_cv_nspr" = "no"; then
    NSPR_CFLAGS=""
    NSPR_LIBS=""
  else
    NSPR_CFLAGS="$CFLAGS"
    NSPR_LIBS="$LIBS"
  fi
  
  CFLAGS="$GENERIC_CFLAGS"
  LIBS="$GENERIC_LIBS"

  if test "$ac_cv_nspr" = "no" && 
     test "$ac_cv_with_nspr" != "no"; then
    AC_MSG_ERROR([You requested to compile with nspr support, but the
nspr library could not be found. 
Please specify library and include directories or disable nspr support.])
  fi  
fi

  if test "$ac_cv_nspr" = "yes"; then
    NSPR_LIBS="${NSPR_LIBS:+$NSPR_LIBS }-lnspr4"
    AC_DEFINE_UNQUOTED([NSPR], [1], [Define this if you have the nspr library.])
  fi

AM_CONDITIONAL([NSPR], [test "$ac_cv_nspr" = "yes"])
  
AC_SUBST([NSPR_CFLAGS])
AC_SUBST([NSPR_LIBS])
])
