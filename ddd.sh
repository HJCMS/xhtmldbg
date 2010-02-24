#!/bin/bash

base=$HOME/hjcms/xhtmldbg

test -x ${base}/build/app/xhtmldbg || exit 1

unset XDG_DATA_HOME
unset XDG_DATA_DIRS

export QT_DEBUG_PLUGINS=0
export LD_LIBRARY_PATH=${base}/build/app:$LD_LIBRARY_PATH

/usr/bin/ddd -x --directory=$PWD ${base}/build/app/xhtmldbg
