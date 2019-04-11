
AC_DEFUN([AM_PATH_MATH], 
[
ac_cv_with_math=yes

AC_ARG_WITH([math],
[  --with-math=PREFIX        Use external math library.],
[ac_cv_with_math=$withval])

AC_ARG_WITH([math-includes],
[  --with-math-includes=DIR  Location of math includes.],
[ac_cv_math_includes=$withval], 
[ac_cv_math_includes=no])

AC_ARG_WITH([math-libs],
[  --with-math-libs=DIR      Location of math libraries.],
[ac_cv_math_libs=$withval], 
[ac_cv_math_libs=no])

if test "$ac_cv_with_math" != "no" ; then

  # Default prefix
  MATH_PREFIX="${prefix}"
  
  if test "$MATH_PREFIX" = "NONE"; then
    MATH_PREFIX=$ac_default_prefix
  fi
  
  # Set user-supplied prefix
  if test "$ac_cv_with_math" != "auto" &&
     test "$ac_cv_with_math" != "yes" &&
     test "$ac_cv_with_math" != "no"; then
    MATH_PREFIX="$ac_cv_with_math"
  fi
  
  # Default include dir
  if test "$MATH_PREFIX" != "/usr"; then
    MATH_CFLAGS="-I$MATH_PREFIX/include"
  fi

  # Set user-supplied include dir
  if test "$ac_cv_math_includes" != "yes" &&
     test "$ac_cv_math_includes" != "no"; then
    MATH_CFLAGS="-I$ac_cv_math_includes"
  fi
  
  # Default library dir
  if test "$MATH_PREFIX" != "/usr"; then
    MATH_LIBS="-L$MATH_PREFIX/lib"
  fi

  # Set user-supplied library dir
  if test "$ac_cv_math_libs" != "yes" &&
     test "$ac_cv_math_libs" != "no"; then
    MATH_LIBS="-L$ac_cv_math_libs"
  fi
  
  GENERIC_CFLAGS="$CFLAGS"
  GENERIC_LIBS="$LIBS"
  
  CFLAGS="$MATH_CFLAGS"
  LIBS=""
  
  AC_CHECK_LIBRARY([m], [sqrt], [for sqrt in -lm],
[ac_cv_js_math=yes], [ac_cv_js_math=no], [$MATH_LIBS])

  if test "$ac_cv_js_math" = "no"; then
    MATH_CFLAGS=""
    MATH_LIBS=""
  else
#    MATH_CFLAGS="$CFLAGS"
    MATH_LIBS="${MATH_LIBS:+$MATH_LIBS }-lm"
  fi
  
  CFLAGS="$GENERIC_CFLAGS"
  LIBS="$GENERIC_LIBS"

  if test "$ac_cv_js_math" = "no" && 
     test "$ac_cv_with_math" != "no"; then
    AC_MSG_ERROR([You requested to compile with external math library, but the
math library could not be found. 
Please specify library and include directories or disable external math.])
  fi  
fi

  if test "$ac_cv_js_math" = "yes"; then
    AC_DEFINE_UNQUOTED([JS_MATH], [1], [Define this if you have a system math library.])
  fi

AM_CONDITIONAL([MATH], [test "$ac_cv_js_math" = "yes"])
  
AC_SUBST([MATH_CFLAGS])
AC_SUBST([MATH_LIBS])
])
