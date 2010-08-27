#!/bin/sh
#################################################################
# Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
#
# Copyright: See COPYING file that comes with this distribution
#################################################################
# -pipe
# -Wall
# -W
# -Wextra
# -Wno-inline
# -Wvla
# -Wundef
# -Wformat=2
# -Wlogical-op
# -Wsign-compare
# -Wformat-security
# -Wmissing-include-dirs
# -Wformat-nonliteral
# -Wold-style-definition
# -Wpointer-arith
# -Winit-self
# -Wdeclaration-after-statement
# -Wfloat-equal
# -Wmissing-prototypes
# -Wstrict-prototypes
# -Wredundant-decls
# -Wmissing-declarations
# -Wmissing-noreturn
# -Wshadow
# -Wendif-labels
# -Wcast-align
# -Wstrict-aliasing=2
# -Wwrite-strings
# -Wp,-D_FORTIFY_SOURCE=2
# -ffast-math
# -fno-common
# -fdiagnostics-show-option

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

##-Wfloat-equal
MY_CFLAGS="${TUNE} -ggdb3 -gstabs+ -Wformat -Woverloaded-virtual -Wmissing-field-initializers -pedantic"
MY_CFLAGS="$MY_CFLAGS -Wwrite-strings -Wlogical-op -Wmissing-include-dirs -Wwrite-strings -Wp,-D_FORTIFY_SOURCE=2"
MY_CFLAGS="$MY_CFLAGS -Wmissing-declarations -Wshadow"
MY_CFLAGS="$MY_CFLAGS -ffast-math -fstrict-aliasing -finline-functions -fomit-frame-pointer"
MY_CFLAGS="$MY_CFLAGS -funwind-tables -fasynchronous-unwind-tables -fexpensive-optimizations -pipe"

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
