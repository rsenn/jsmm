
AC_DEFUN([AC_CHECK_READLINE], 
[
ac_cv_with_readline=auto

AC_ARG_WITH([readline],
[  --with-readline=PREFIX        Support for readline commandline.], 
[ac_cv_with_readline=$withval])

AC_ARG_WITH([readline-includes],
[  --with-readline-includes=DIR  Location of readline includes.],
[ac_cv_readline_includes=$withval], 
[ac_cv_readline_includes=no])

AC_ARG_WITH([readline-libs],
[  --with-readline-libs=DIR      Location of readline libraries.],
[ac_cv_readline_libs=$withval], 
[ac_cv_readline_libs=no])

if test "$ac_cv_with_readline" != "no" ; then

  # Default prefix
  READLINE_PREFIX="${prefix}"
  
  if test "$READLINE_PREFIX" = "NONE"; then
    READLINE_PREFIX=$ac_default_prefix
  fi
  
  # Set user-supplied prefix
  if test "$ac_cv_with_readline" != "auto" &&
     test "$ac_cv_with_readline" != "yes" &&
     test "$ac_cv_with_readline" != "no"; then
    READLINE_PREFIX="$ac_cv_with_readline"
  fi
  
  # Default include dir
  if test "$READLINE_PREFIX" != "/usr"; then
    READLINE_CFLAGS="-I$READLINE_PREFIX/include"
  fi

  # Set user-supplied include dir
  if test "$ac_cv_readline_includes" != "yes" &&
     test "$ac_cv_readline_includes" != "no"; then
    READLINE_CFLAGS="-I$ac_cv_readline_includes"
  fi
  
  # Default library dir
  if test "$READLINE_PREFIX" != "/usr"; then
    READLINE_LIBS="-L$READLINE_PREFIX/lib"
  fi

  # Set user-supplied library dir
  if test "$ac_cv_readline_libs" != "yes" &&
     test "$ac_cv_readline_libs" != "no"; then
    READLINE_LIBS="-L$ac_cv_readline_libs"
  fi
  
  GENERIC_CFLAGS="$CFLAGS"
  GENERIC_LIBS="$LIBS"
  
  CFLAGS="$READLINE_CFLAGS"
  LIBS="$READLINE_LIBS"
  
  AC_CHECK_LIB([readline], [readline],
[ac_cv_js_readline=yes], [ac_cv_js_readline=no])

  if test "$ac_cv_js_readline" = "no"; then
    AC_CHECK_LIB([ncurses], [tgetent], 
[ac_cv_have_ncurses=yes], [ac_cv_have_ncurses=no])

    if test "$ac_cv_have_ncurses" = "yes"; then
      LIBS="$LIBS -lncurses"

      unset ac_cv_lib_readline_readline

      AC_CHECK_LIBRARY([readline], [readline], [for readline in -lreadline -lncurses],
[ac_cv_js_readline=yes], [ac_cv_js_readline=no])
    fi
  fi

  if test "$ac_cv_js_readline" = "no"; then
    READLINE_CFLAGS=""
    READLINE_LIBS=""
  else
    READLINE_CFLAGS="$CFLAGS"
    READLINE_LIBS="$LIBS"
  fi
  
  CFLAGS="$GENERIC_CFLAGS"
  LIBS="$GENERIC_LIBS"

  if test "$ac_cv_js_readline" = "no" && 
     test "$ac_cv_with_readline" != "no"; then
    AC_MSG_ERROR([You requested to compile with readline support, but the
readline library could not be found. 
Please specify library and include directories or disable readline support.])
  fi  
fi

  if test "$ac_cv_js_readline" = "yes"; then
    READLINE_LIBS="${READLINE_LIBS:+$READLINE_LIBS }-lreadline -lhistory"
    AC_DEFINE_UNQUOTED([JS_READLINE], [1], [Define this if you have the readline library.])
  fi

AM_CONDITIONAL([READLINE], [test "$ac_cv_js_readline" = "yes"])
  
AC_SUBST([READLINE_CFLAGS])
AC_SUBST([READLINE_LIBS])
])
