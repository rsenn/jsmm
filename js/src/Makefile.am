SUBDIRS = fdlibm
LINK = $(LIBTOOL) --mode=link $(CCLD) -o $@

#all: jsconfig.h jscpucfg.h
#	make all-recursive

lib_LTLIBRARIES = libjs.la
bin_PROGRAMS = js
noinst_HEADERS = prmjtime.h jsconfig.h.in
pkginclude_HEADERS = jsapi.h jsarena.h jsarray.h jsatom.h jsautocfg.h jsbit.h jsbool.h jsclist.h jscntxt.h jscompat.h jsdate.h jsdbgapi.h jsdhash.h jsdtoa.h jsemit.h jsexn.h jsfile.h jsfun.h jsgc.h jshash.h jsinterp.h jslibmath.h jslock.h jslong.h jsmath.h jsnum.h jsobj.h jsopcode.h jsosdep.h jsotypes.h jsparse.h jsprf.h jsprvtd.h jspubtd.h jsregexp.h jsscan.h jsscope.h jsscript.h jsstddef.h jsstr.h jstypes.h jsutil.h jsxdrapi.h jsxml.h
pkginclude_DATA = jscpucfg.h jsconfig.h js.msg jsfile.msg jsshell.msg jsopcode.tbl

libjs_la_SOURCES = jsapi.c jsarena.c jsarray.c jsatom.c jsbool.c jscntxt.c jsdate.c jsdbgapi.c jsdhash.c jsdtoa.c jsemit.c jsexn.c jsfile.c jsfun.c jsgc.c jshash.c jsinterp.c jslock.c jslog2.c jslong.c jsmath.c jsnum.c jsobj.c jsopcode.c jsparse.c jsprf.c jsregexp.c jsscan.c jsscope.c jsscript.c jsstr.c jsutil.c jsxdrapi.c jsxml.c prmjtime.c
libjs_la_DEPENDENCIES = $(DATA)

pkgincludedir = $(includedir)/js

if USE_FDLIBM_MATH
libjs_la_LIBADD = -Lfdlibm -lfdm $(NSPR_LIBS) $(MATH_LIBS)
else
libjs_la_LIBADD = $(NSPR_LIBS) $(MATH_LIBS)
endif

if SHAREDLIB
libjs_la_LDFLAGS = -rpath $(libdir) -version-info 7:0:6 -no-undefined
else
libjs_la_LDFLAGS = -rpath $(libdir) -no-undefined
endif

INCLUDES = -I. -I$(srcdir)

EXTRA_DIST = jscpucfg.c js.msg jsfile.msg jsshell.msg jsopcode.tbl \
             js.mdp js.pkg js3240.rc jsOS240.def jsify.pl jslocko.asm lock_SunOS.s perfect.js plify_jsdhash.sed resource.h win32.order \
             README.html SpiderMonkey.rsp

js_SOURCES = js.c
js_CFLAGS = $(READLINE_CFLAGS)
js_LDADD = libjs.la $(READLINE_LIBS)

DISTCLEANFILES = jscpucfg.h
MAINTAINERCLEANFILES = Makefile.in *~

noinst_PROGRAMS = jscpucfg
jscpucfg_SOURCES = jscpucfg.c

jscpucfg.h: jscpucfg$(EXEEXT)
	./jscpucfg$(EXEEXT) > jscpucfg.h
