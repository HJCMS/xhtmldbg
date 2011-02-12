#!/bin/sh

PRJ_HOME=$HOME/hjcms/xhtmldbg

test -d ${PRJ_HOME}/src || exit 1

ctags -R --c++-types=+px --excmd=pattern -f utils/qt.ctags /usr/include/qt4
ctags -R --c++-types=+px --excmd=pattern -f utils/kde.ctags /usr/include/kde4

ctags -R --c++-types=+px --excmd=pattern --exclude=Makefile \
  --exclude=CMakeLists.txt -f utils/xhtmldbg.ctags src/

exit $?
