#!/bin/sh

for i in xhtmldbg xhtmldbgwidgets ; do
  xsltproc --encoding utf-8 --output po/${i}.pot ts2gettext.xslt ${i}_untranslated.ts
done
