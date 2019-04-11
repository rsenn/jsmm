#!/bin/sh
#
# 20190411


aclocal --force -I m4
autoheader --force
automake --force --copy --foreign --add-missing --foreign --add-missing
aclocal --force -I m4
autoconf --force
