#!/bin/bash
# Figuring out what version is in configure.in
YANOIDVERSION=`cat configure.in | grep "AM_INIT_AUTOMAKE" | sed -e's/.*"\(.*\)".*/\1/'`
# Make a distribution
./configure
make dist
# Let RPM do its thing (se the yanoid.spec.in file)
rpm -ta yanoid-$YANOIDVERSION.tar.gz
