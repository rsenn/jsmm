# AC_CHECK_LIB(LIBRARY, FUNCTION, [MESSAGE],
#              [ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND],
#              [LIBRARY-PATHS])
AC_DEFUN([AC_CHECK_LIBRARY],
[m4_ifval([$4], , [AH_CHECK_LIB([$1])])dnl
AS_LITERAL_IF([$1],
        [AS_VAR_PUSHDEF([ac_Lib], [ac_cv_lib_$1_$2])],
        [AS_VAR_PUSHDEF([ac_Lib], [ac_cv_lib_$1''_$2])])dnl
AC_CACHE_CHECK(m4_ifval([$3], [$3], [for $2 in -l$1]), ac_Lib,
[ac_check_lib_save_LIBS=$LIBS
LIBS="$6 -l$1 $LIBS"
AC_LINK_IFELSE([AC_LANG_CALL([], [$2])],
         [AS_VAR_SET(ac_Lib, yes)],
         [AS_VAR_SET(ac_Lib, no)])
LIBS=$ac_check_lib_save_LIBS])
AS_IF([test AS_VAR_GET(ac_Lib) = yes],
      [m4_default([$4], [AC_DEFINE_UNQUOTED(AS_TR_CPP(HAVE_LIB$1))
  LIBS="-l$1 $LIBS"
])],
      [$5])dnl
AS_VAR_POPDEF([ac_Lib])dnl
])# AC_CHECK_LIB

# AC_CHECK_LIB(LIBRARY, FUNCTION, [MESSAGE],
#              [ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND],
#              [LIBRARY-PATHS])
AC_DEFUN([AC_CHECK_CXXLIBRARY],
[m4_ifval([$4], , [AH_CHECK_LIB([$1])])dnl
AS_LITERAL_IF([$1],
        [AS_VAR_PUSHDEF([ac_Lib], [ac_cv_lib_$1_$2])],
        [AS_VAR_PUSHDEF([ac_Lib], [ac_cv_lib_$1''_$2])])dnl
AC_CACHE_CHECK(m4_ifval([$3], [$3], [for $2 in -l$1]), ac_Lib,
[ac_check_lib_save_LIBS=$LIBS
LIBS="$6 -l$1 $LIBS"
AC_LINK_IFELSE([AC_LANG_CALL(C++)([], [$2])],
         [AS_VAR_SET(ac_Lib, yes)],
         [AS_VAR_SET(ac_Lib, no)])
LIBS=$ac_check_lib_save_LIBS])
AS_IF([test AS_VAR_GET(ac_Lib) = yes],
      [m4_default([$4], [AC_DEFINE_UNQUOTED(AS_TR_CPP(HAVE_LIB$1))
  LIBS="-l$1 $LIBS"
])],
      [$5])dnl
AS_VAR_POPDEF([ac_Lib])dnl
])# AC_CHECK_LIB

