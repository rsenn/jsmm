AC_DEFUN([AM_PATH_JS],
[
ac_cv_with_js=auto

AC_ARG_WITH([js],
[  --with-js=PREFIX        Location of the SpiderMonkey JavaScript engine.],
[ac_cv_with_js=$withval])

AC_ARG_WITH([js-includes],
[  --with-js-includes=DIR  Location of js includes.],
[ac_cv_js_includes=$withval],
[ac_cv_js_includes=no])

AC_ARG_WITH([js-libs],
[  --with-js-libs=DIR      Location of js libraries.],
[ac_cv_js_libs=$withval], 
[ac_cv_js_libs=no])

if test "$ac_cv_with_js" != "no" ; then

  # Default prefix
  : ${JS_PATH=$prefix}
  
  if test "$JS_PATH" = "NONE"; then
    JS_PATH="$ac_default_prefix"
  fi
  
  # Set user-supplied prefix
  if test "$ac_cv_with_js" != "auto" &&
     test "$ac_cv_with_js" != "yes" &&
     test "$ac_cv_with_js" != "no"; then
    JS_PATH="$ac_cv_with_js"
  fi

  AC_CHECK_PROG([JS_CONFIG], [js-config], [$as_dir/$ac_word$ac_exec_ext], [], [$JS_PATH/bin:$PATH])

  : ${JS_PREFIX="$JS_PATH"}

  AC_MSG_CHECKING([for js includes])

  # Set user-supplied include dir
  if test "$ac_cv_js_includes" != "yes" &&
     test "$ac_cv_js_includes" != "no"; then
    JS_CFLAGS="-I$ac_cv_js_includes"
  elif test -n "$JS_CONFIG"; then
    JS_CFLAGS=`$JS_CONFIG --cflags`
  fi
  
  # Default include dir
  if test -z "$JS_CFLAGS"; then
    JS_CFLAGS="-I$JS_PREFIX/include"
  fi
  
  AC_MSG_RESULT([$JS_CFLAGS])

  AC_MSG_CHECKING([for js libs])
  
  # Set user-supplied library dir
  if test "$ac_cv_js_libs" != "yes" &&
     test "$ac_cv_js_libs" != "no"; then
    JS_LIBS="-L$ac_cv_js_libs"
  elif test -n "$JS_CONFIG"; then
    JS_LIBS=`$JS_CONFIG --libs`
  fi

  # Default library dir
  if test -z "$JS_LIBS"; then
    JS_LIBS="-L$JS_PREFIX/lib"
  fi

  AC_MSG_RESULT([$JS_LIBS])
  
  GENERIC_CFLAGS="$CFLAGS"
  GENERIC_LIBS="$LIBS"
  
  CFLAGS="$JS_CFLAGS"
  LIBS="$JS_LIBS"
  
#  echo "LIBS: $LIBS"

  AC_CHECK_LIB([js], [JS_NewObject],
[ac_cv_have_js=yes], [ac_cv_have_js=no])

  if test "$ac_cv_have_js" = "no"; then
    AC_CHECK_LIB([m], [sqrt],
[ac_cv_have_m=yes], [ac_cv_have_m=no])

    if test "$ac_cv_have_m" = "yes"; then
      LIBS="-lm${LIBS:+ }$LIBS"

      AC_CHECK_LIB([js], [JS_ConstructObject],
[ac_cv_have_js=yes], [ac_cv_have_js=no])

      if test "$ac_cv_have_js" = "yes"; then
        LIBS="-ljs${LIBS:+ }$LIBS"
      fi
    fi
  fi

  if test "$ac_cv_have_js" = "no"; then
    JS_CFLAGS=""
    JS_LIBS=""
  else
    JS_CFLAGS="$CFLAGS"
    JS_LIBS="$LIBS"
  fi

  CFLAGS="$GENERIC_CFLAGS"
  LIBS="$GENERIC_LIBS"

  if test "$ac_cv_have_js" = "no"; then
    AC_MSG_ERROR([libjs could not be found.
It is required to compile jsmm, please try to specify library
and include directories.])
  fi  
fi

AC_SUBST([JS_CFLAGS])
AC_SUBST([JS_LIBS])
])
