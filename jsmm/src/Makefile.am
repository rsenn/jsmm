MAINTAINERCLEANFILES = Makefile.in

#LINK = $(LIBTOOL) --mode=link $(CCLD) -o $@

lib_LTLIBRARIES = libjsmm.la

libjsmm_la_LIBADD = $(JS_LIBS)
if SHAREDLIB
libjsmm_la_LDFLAGS = -rpath $(libdir) -version-info 7:0:6 -no-undefined
else
libjsmm_la_LDFLAGS = -rpath $(libdir) -no-undefined
endif
libjsmm_la_SOURCES = \
  api.cpp \
  arrayobject.cpp \
  arrayobjectapi.cpp \
  baseclass.cpp \
  baseconstructor.cpp \
  basefunction.cpp \
  class.cpp \
  classapi.cpp \
  constdoublespec.cpp \
  constructor.cpp \
  constructorapi.cpp \
  constructorobject.cpp \
  constructorobjectapi.cpp \
  context.cpp \
  contextapi.cpp \
  engine.cpp \
  errorreport.cpp \
  exception.cpp \
  fileobject.cpp \
  fileobjectapi.cpp \
  function.cpp \
  functionapi.cpp \
  functionobject.cpp \
  functionobjectapi.cpp \
  functionspec.cpp \
  numberobjectapi.cpp \
  object.cpp \
  objectapi.cpp \
  pointer.cpp \
  propertyspec.cpp \
  prototype.cpp \
  prototypeobject.cpp \
  prototypeobjectapi.cpp \
  reference.cpp \
  runtime.cpp \
  script.cpp \
  scriptapi.cpp \
  stackframe.cpp \
  stdext.cpp \
  string.cpp \
  stringapi.cpp \
  value.cpp

INCLUDES = -I$(top_srcdir)/include
EXTRA_DIST = 

.cpp.s:
@am__fastdepCXX_TRUE@	if $(CXXCOMPILE) -fverbose-asm -MT $@ -MD -MP -MF "$(DEPDIR)/$*.Tpo" -S -o $@ $<; \
@am__fastdepCXX_TRUE@	then mv -f "$(DEPDIR)/$*.Tpo" "$(DEPDIR)/$*.Po"; else rm -f "$(DEPDIR)/$*.Tpo"; exit 1; fi
@AMDEP_TRUE@@am__fastdepCXX_FALSE@	source='$<' object='$@' libtool=no @AMDEPBACKSLASH@
@AMDEP_TRUE@@am__fastdepCXX_FALSE@	DEPDIR=$(DEPDIR) $(CXXDEPMODE) $(depcomp) @AMDEPBACKSLASH@
@am__fastdepCXX_FALSE@	$(CXXCOMPILE) -fverbose-asm -S $<

