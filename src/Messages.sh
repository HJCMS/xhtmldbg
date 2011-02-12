#!/usr/bin/env bash

$EXTRACTRC `find ${i} -name "*.ui"` >> ./rc.cpp || exit 11
$XGETTEXT `find ${i} -name "*.cpp"` -o ../po/xhtmldbg.pot
rm -f ./rc.cpp
