#!/bin/bash

base=$HOME/hjcms/xhtmldbg

test -x ${base}/build/app/xhtmldbg || exit 1

export QT_DEBUG_PLUGINS=0
export PHONON_GST_DEBUG=0
export QT_CRASH_OUTPUT=${base}/build/app/crash.log
# export NSPLUGIN_DIR=/usr/lib/nspluginwrapper/i386/linux

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
  full)
    export QDBUS_DEBUG=1
    export QT_LAYOUT_DEBUG=1
    /usr/bin/ddd -x --directory=${base} ${base}/build/app/xhtmldbg
  ;;
  *)
    /usr/bin/ddd -x --directory=${base} ${base}/build/app/xhtmldbg
  ;;
esac
