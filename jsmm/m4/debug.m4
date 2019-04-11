AC_DEFUN([AC_CHECK_DEBUG],
[
  AC_ARG_ENABLE([debug], [  --enable-debug          Build a debug version.], [ac_cv_js_debug=$withval], [ac_cv_js_debug=no])

  CFLAGS="$(echo $CFLAGS | sed 's,-g *,, ; s,-ggdb *,, ; s,-O2 *,, ; s,-Wall,,')"
  CXXFLAGS="$(echo $CXXFLAGS | sed 's,-g *,, ; s,-ggdb *,, ; s,-O2 *,, ; s,-Wall,,')"

  if test "$ac_cv_js_debug" != "no"; then
    CFLAGS="${CFLAGS:+$CFLAGS }-g -ggdb -O0 -Wall"
    CXXFLAGS="${CXXFLAGS:+$CXXFLAGS }-g -ggdb -O0 -Wall"
    
    AC_DEFINE_UNQUOTED([JS_DEBUG], [1], [Define this if you want to build a debug version.])
  else
    CFLAGS="${CFLAGS:+$CFLAGS }-Os -fexpensive-optimizations -fomit-frame-pointer"
    CXXFLAGS="${CXXFLAGS:+$CXXFLAGS }-Os -fexpensive-optimizations -fomit-frame-pointer"
  fi

  AM_CONDITIONAL([DEBUG], [test "$ac_cv_js_debug" = "yes"])
])  
