MAINTAINERCLEANFILES = Makefile.in

noinst_PROGRAMS = \
  example1 \
  example2 \
  example3 \
  example4
  
example1_SOURCES = example1.cpp
example2_SOURCES = example2.cpp
example3_SOURCES = example3.cpp
example4_SOURCES = example4.cpp

INCLUDES = -I$(top_srcdir)/include
LDADD = -L$(top_builddir)/src -ljsmm

EXTRA_DIST = \
  example1.js \
  example2.js \
  example3.js \
  example4.js
