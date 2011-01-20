#!/bin/sh

for i in de ; do
  xsltproc --encoding utf-8 --output po/xhtmldbg_${i}.po ts2gettext.xslt xhtmldbg_${i}.ts qt_${i}.ts
done
