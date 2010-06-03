#!/bin/sh
#
# Slackware build script for xhtmldbg
#
# Copyright 2009-2010 Corrado Franco (http://conraid.net)
# All rights reserved.
#
# Redistribution and use of this script, with or without modification, is
# permitted provided that the following conditions are met:
#
# 1. Redistributions of this script must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
# EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Exit on most errors
set -e

# Set variables:
CWD=$(pwd)

PRGNAME=${PRGNAME:-xhtmldbg}
PKGNAME=$(echo $PRGNAME | tr A-Z a-z)
VERSION=${VERSION:-@XHTMLDBG_PACKAGE_VERSION@}
PKGVERSION=$(echo $VERSION | tr - _)
ARCH=${ARCH:-i686}
BUILD=${BUILD:-1}
TAG=${TAG:-cf}
TMP=${TMP:-/tmp/pkg}
JOBS=${JOBS:--j2}
SOURCE="http://gitweb.hjcms.de/cgi-bin/index.cgi/xhtmldbg/snapshot/xhtmldbg-$VERSION.tar.gz"
PKG=$TMP/package-$PKGNAME
DOCS="$PKG/usr/doc/$PKGNAME-$PKGVERSION"

# Insert document files in this variable
DOCFILES="AUTHORS COPYING ChangeLog INSTALL NEWS README"

# Set compiling FLAGS
case "$ARCH" in
  i486)
    SLKCFLAGS="-O2 -march=i486 -mtune=i686"; CHOST="i486"
    SLKLDFLAGS=""; LIBDIRSUFFIX=""
  ;;
  i686)
    SLKCFLAGS="-O2 -march=i686 -pipe -fomit-frame-pointer"; CHOST="i486"
    SLKLDFLAGS=""; LIBDIRSUFFIX=""
  ;;
  x86_64)
    SLKCFLAGS="-O2 -fPIC -pipe -fomit-frame-pointer"; CHOST="x86_64"
    SLKLDFLAGS="-L/lib64 -L/usr/lib64"; LIBDIRSUFFIX="64"
  ;;
esac

# Download source, if not exist
if ! [ -e $(basename $SOURCE) ]; then
    wget $SOURCE
fi

# Extract source and prepare:
rm -rf $PKG
mkdir -p $TMP $PKG
cd $TMP
rm -rf $PRGNAME-$VERSION
tar xvf $CWD/$(basename $SOURCE)
cd $PRGNAME-$VERSION

# Make sure ownerships and permissions are sane:
chown -R root:root .
chmod -R u+w,go+r-w,a-s .

# Configure
mkdir -p build
cd build
cmake -Wno-dev \
  -DCMAKE_C_FLAGS:STRING="$SLKCFLAGS" \
  -DCMAKE_CXX_FLAGS:STRING="$SLKCFLAGS" \
  -DCMAKE_LD_FLAGS:STRING="$SLKLDFLAGS" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX:PATH=/usr \
  -DSYSCONF_INSTALL_DIR:PATH=/etc \
  -DLIB_SUFFIX=${LIBDIRSUFFIX} \
  -DCSS_VALIDATOR_JAR=/opt/w3c-css-validator/css-validator.jar \
  ..

# Build and install:
make $JOBS
make DESTDIR=$PKG install/strip

# Compress and link manpages, if any:
if [ -d $PKG/usr/man ]; then
( cd $PKG/usr/man
  find -type f -exec gzip -9 {} \;
  for i in $(find -type l); do ln -s $(readlink $i).gz $i.gz; rm $i; done
)
fi

# Add a documentation directory:
mkdir -p ${DOCS}
for i in ${DOCFILES}; do
    if [ -e "${i}" ]; then
        if ! [ -L "${i}" ]; then
	    cp -a "${i}" ${DOCS}
	else
	    cp -LRp "${i}" ${DOCS}
        fi
    fi;
done

# Add a slackbuild script:
cat $CWD/$PKGNAME.SlackBuild > $DOCS/$PKGNAME.SlackBuild

# Add slack-desc
mkdir -p $PKG/install
cat $CWD/slack-desc > $PKG/install/slack-desc

# Add a doinst.sh, if any
if [ -f $CWD/doinst.sh.gz ]; then
    zcat $CWD/doinst.sh.gz > $PKG/install/doinst.sh
elif [ -f $CWD/doinst.sh ]; then
    cat $CWD/doinst.sh > $PKG/install/doinst.sh
fi

# Create package:
cd $PKG
/sbin/makepkg -l y -c n $CWD/$PKGNAME-$PKGVERSION-$ARCH-${BUILD}${TAG}.txz

# Clean file, if option is set: 
if [ "$1" = "--cleanup" ]; then
  rm -rf $PKG
  rm -rf ${TMP}/${PRGNAME}-${VERSION}
fi
