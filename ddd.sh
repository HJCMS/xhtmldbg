#!/bin/bash

base=$HOME/hjcms/xhtmldbg

test -d ${base}/build/app || {
  base="`dirname $0`"
}

test -x ${base}/build/app/xhtmldbg || {
  echo "Can not found ${base}/build/app/xhtmldbg"
  exit 1
}

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

  ## kdevelop
  ide)
    export QT_LAYOUT_DEBUG=1
    ${base}/build/app/xhtmldbg
  ;;

  ## using -graphicssystem opengl
  gl)
    export DISPLAYMANAGER_XGL_OPTS="-accel glx:pbuffer -accel xv:fbo"
    export KDE_SESSION_VERSION=4
    export KDE_COLOR_DEBUG=1
    export QT_USE_X11GL_PIXMAPS=1
    unset LIBGL_ALWAYS_INDIRECT
    ${base}/build/app/xhtmldbg -graphicssystem opengl
  ;;

  ## using -graphicssystem raster
  raster)
    ${base}/build/app/xhtmldbg -graphicssystem raster
  ;;

  ## full debug with ddd
  full)
    export QDBUS_DEBUG=1
    export QT_LAYOUT_DEBUG=1
    /usr/bin/ddd -x --directory=${base} ${base}/build/app/xhtmldbg
  ;;

  ## strace output
  strace)
    echo "strace ${base}/build/app/xhtmldbg 2> /tmp/xhtmldbg.log"
    strace ${base}/build/app/xhtmldbg 2> /tmp/xhtmldbg.log
  ;;

  ##  http://doc.qt.nokia.com/4.6/webintegration.html
  ##  Running with Firefox (Netscape Plugin API)
  npapi)
    export QT_LAYOUT_DEBUG=1
    export MOZILLA_HOME="$HOME/Programme/firefox3"
    export QTWEBKIT_PLUGIN_PATH="$MOZILLA_HOME/plugins"
    export MOZ_PLUGIN_PATH="$MOZILLA_HOME/plugins"
    ${base}/build/app/xhtmldbg
  ;;

  h|help)
    echo "./`basename $0` {ide|gl|raster|strace|npapi}"
    echo " ide     (with kdevelop)"
    echo " gl      (with -graphicssystem opengl)"
    echo " raster  (with -graphicssystem raster)"
    echo " strace  (write strace to /tmp/xhtmldbg.log)"
    echo " npapi   (mozilla plugin test enviroment)"
    exit 0
  ;;

  *)
    echo "ddd -x --directory=${base} ${base}/build/app/xhtmldbg"
    /usr/bin/ddd -x --directory=${base} ${base}/build/app/xhtmldbg
  ;;
esac
