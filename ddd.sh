#!/bin/bash

base=$HOME/hjcms/xhtmldbg

test -x ${base}/build/app/xhtmldbg || exit 1

unset XDG_DATA_HOME
unset XDG_DATA_DIRS

export QDBUS_DEBUG=1
export QT_DEBUG_PLUGINS=0
export PHONON_GST_DEBUG=0
export QT_CRASH_OUTPUT=${base}/build/app/crash.log

case "$1" in
  full)
    ## export QT_WEBKIT_LOG=${base}/build/app/webkit.log
    export QT_LAYOUT_DEBUG=1
    /usr/bin/ddd -x --directory=${base} ${base}/build/app/xhtmldbg
  ;;
  *)
    /usr/bin/ddd -x --directory=${base} ${base}/build/app/xhtmldbg
  ;;
esac
