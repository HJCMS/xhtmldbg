#!/bin/bash

base=$HOME/hjcms/xhtmldbg

test -x ${base}/build/app/xhtmldbg || exit 1

export QT_DEBUG_PLUGINS=0
export PHONON_GST_DEBUG=0
export QT_CRASH_OUTPUT=${base}/build/app/crash.log

if test -n "$LD_LIBRARY_PATH" ; then
  case ":$LD_LIBRARY_PATH:" in
    *:${base}/build/app:*)
      continue;
    ;;
    *)
      export LD_LIBRARY_PATH="${base}/build/app:$LD_LIBRARY_PATH"
    ;;
  esac
else
  export LD_LIBRARY_PATH="${base}/build/app"
fi

case "$1" in
  ide)
    export QT_LAYOUT_DEBUG=1
    ${base}/build/app/xhtmldbg
  ;;
  gl)
    export DISPLAYMANAGER_XGL_OPTS="-accel glx:pbuffer -accel xv:fbo"
    export KDE_SESSION_VERSION=4
    export KDE_COLOR_DEBUG=1
    export QT_USE_X11GL_PIXMAPS=1
    unset LIBGL_ALWAYS_INDIRECT
    ${base}/build/app/xhtmldbg -graphicssystem opengl
  ;;
  raster)
    ${base}/build/app/xhtmldbg -graphicssystem raster
  ;;
  full)
    export QDBUS_DEBUG=1
    export QT_LAYOUT_DEBUG=1
    /usr/bin/ddd -x --directory=${base} ${base}/build/app/xhtmldbg
  ;;
  trace)
    echo "strace ${base}/build/app/xhtmldbg 2> /tmp/xhtmldbg.log"
    strace ${base}/build/app/xhtmldbg 2> /tmp/xhtmldbg.log
  ;;
  *)
    echo "ddd -x --directory=${base} ${base}/build/app/xhtmldbg"
    /usr/bin/ddd -x --directory=${base} ${base}/build/app/xhtmldbg
  ;;
esac
