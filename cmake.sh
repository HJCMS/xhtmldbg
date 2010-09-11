#!/bin/sh
#################################################################
# Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
#
# Copyright: See COPYING file that comes with this distribution
#################################################################

base=$HOME/hjcms/xhtmldbg

test -d "${base}" || {
  base="`dirname $0`/"
}

debug_build_target=${base}/build

if test "`hostname`" = "webmast" ; then
TUNE="-O3 -mtune=athlon-xp -march=i686 -mno-sse3"
else
TUNE="-O2 -mtune=generic -march=`uname -i`"
fi

## needed for some development tests
MDEFS=""
if test -d /home/$USER/hjcms/xhtmldbg ; then
MDEFS="-DMAINTAINER_REPOSITORY"
fi

MY_CFLAGS="${TUNE} -ggdb3 -gstabs+ -Wformat -Woverloaded-virtual -Wmissing-field-initializers -pedantic ${MDEFS}"
MY_CFLAGS="$MY_CFLAGS -Wlogical-op -Wmissing-declarations -ffast-math -fstrict-aliasing -finline-functions"
MY_CFLAGS="$MY_CFLAGS -fomit-frame-pointer -funwind-tables -fasynchronous-unwind-tables -fexpensive-optimizations -pipe"

INSTALL_DESTDIR="`mktemp --tmpdir=/tmp xhtmldbg.XXXXXXXXXX`"

function runcmake()
{
  cmake \
    -DCMAKE_CXX_FLAGS:STRING="$MY_CFLAGS" \
    -DCMAKE_INSTALL_PREFIX:PATH=/usr \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DXHTMLDBG_BUILD_VERBOSE:BOOL=ON \
    -DCMAKE_SKIP_RPATH:BOOL=ON \
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
    -DXHTMLDBG_BETA_EXTENSION:BOOL=ON \
    -DCPACK_PACKAGE_INSTALL_DIRECTORY:PATH="$INSTALL_DESTDIR" \
    ../
}

echo "cmake"

if test "$PWD" = "${debug_build_target}" ; then
  if test -f cmake_install.cmake ; then
    rm -rf ${debug_build_target}/*
  fi
  mkdir -p ${debug_build_target}/app/{designer,plugins}
  runcmake
else
  mkdir -p build/app/{designer,plugins}
  pushd build
    runcmake
  popd
fi
