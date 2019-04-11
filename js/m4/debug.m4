AC_DEFUN([AC_CHECK_DEBUG],
[
  AC_ARG_ENABLE([debug], [  --enable-debug          Build a debug version.], [ac_cv_js_debug=$withval], [ac_cv_js_debug=no])

  if test "$ac_cv_js_debug" != "no"; then
    CFLAGS="$(echo $CFLAGS | sed 's,\-g *,,;;s,\-ggdb *,,;;s,\-O2 *,,;;s,\-Wall,,') -g -ggdb -O0 -Wall"
    
    AC_DEFINE_UNQUOTED([JS_DEBUG], [1], [Define this if you want to build a debug version.])
  fi

  AM_CONDITIONAL([DEBUG], [test "$ac_cv_js_debug" = "yes"])
])  
