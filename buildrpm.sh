#!/bin/bash
YANOIDVERSION=`cat configure.in | grep "AM_INIT_AUTOMAKE" | sed -e's/.*"\(.*\)".*/\1/'`
PACKDIR=yanoid-$YANOIDVERSION
rm -rf /tmp/$PACKDIR
mkdir /tmp/$PACKDIR
cp -r * /tmp/$PACKDIR
pushd /tmp/$PACKDIR
cat yanoid.spec.in | sed -e"s/@VERSION@/$YANOIDVERSION/" > yanoid.spec
make distclean
cd ..
tar -czf yanoid-$YANOIDVERSION.tar.gz $PACKDIR
rpm -ta yanoid-$YANOIDVERSION.tar.gz
popd
# OK all is well - make some cleaning up
rm -rf /tmp/$PACKDIR
