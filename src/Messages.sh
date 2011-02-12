#!/usr/bin/env bash

for i in `find . -type d` ; do
  case "$i" in
    ./components)
      continue;
    ;;
    *)
      $EXTRACTRC `find ${i} -name "*.ui"` >> ${i}/rc.cpp || exit 11
      $XGETTEXT `find ${i} -name "*.cpp"` -o $podir/xhtmldbg.pot
      rm -f ${i}/rc.cpp
    ;;
  esac
done
