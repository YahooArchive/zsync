#!/bin/sh

set -e
set -x
export DESTDIR=$HOME/prefix

rm -rf $DESTDIR || exit 1
make clean || true # ignore failures

aclocal || exit 1
#libtoolize --force || exit 1
autoheader || exit 1
automake -a || exit 1
autoconf || exit 1
./configure --prefix=/usr || exit 1
make || exit 1
make install  || exit 1

