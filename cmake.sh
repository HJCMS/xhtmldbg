#!/bin/sh
#################################################################
# Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
#
# Copyright: See COPYING file that comes with this distribution
#################################################################

debug_build_target=$HOME/hjcms/xhtmldbg/build

# -DCMAKE_CXX_COMPILER:FILEPATH=$(which mpic++) \
# -DCMAKE_C_COMPILER:FILEPATH=$(which mpicc) \

MY_CFLAGS="-O3 -mtune=athlon-xp -march=i686 -Wformat -Woverloaded-virtual -pedantic"
MY_CFLAGS="$MY_CFLAGS -ffast-math -fstrict-aliasing -finline-functions -fomit-frame-pointer -fexpensive-optimizations -pipe"

function runcmake()
{
  cmake \
    -DCMAKE_CXX_FLAGS:STRING="$MY_CFLAGS" \
    -DCMAKE_INSTALL_PREFIX:PATH=/usr \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DXHTMLDBG_BUILD_VERBOSE:BOOL=ON \
    -DCMAKE_SKIP_RPATH:BOOL=ON \
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
    -DXHTMLDBG_BETA_EXTENSION:BOOL=ON ../
}

echo "cmake"

if test "$PWD" = "${debug_build_target}" ; then
  if test -f cmake_install.cmake ; then
    rm -rf ${debug_build_target}/*
  fi
  runcmake
else
  mkdir -p build
  pushd build
    runcmake
  popd
fi
