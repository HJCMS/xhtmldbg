#!/usr/bin/env bash

$EXTRACTRC `find ${i} -name "*.ui"` >> ${i}/rc.cpp || exit 11
$XGETTEXT `find ${i} -name "*.cpp"` -o $podir/xhtmldbg.pot
