#!/usr/bin/env sh
#################################################################
# Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
#
# Copyright: See COPYING file that comes with this distribution
#################################################################

## -silent
for i in xhtmldbg_de xhtmldbg_untranslated ; do
	lupdate -noobsolete -extensions h,cpp,cxx \
		src/ build/src -ts i18n/${i}.ts
done

## -silent
for i in xhtmldbgwidgets_de xhtmldbgwidgets_untranslated ; do
  lupdate -noobsolete -extensions h,cpp,cxx \
    src/components/ build/src/components/ -ts i18n/${i}.ts
done
