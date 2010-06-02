#!/bin/sh

set +x

out=/tmp/css.list
dst=$HOME/hjcms/xhtmldbg/utils
in=${dst}/css.html

# iconv --from-code=ISO_8859-1 --to-code=UTF-8 ${in} > /tmp/converted.html

xsltproc \
  --output ${out} \
  ${dst}/css2list.xslt ${in}

perl -pi -e 's,^[ \t]+,,g' ${out}
perl -pi -e 's,^[:@\[].+$,,g' ${out}

sed '/^[ \t]*$/d' ${out} > ${out}.2
sort ${out}.2 > ${out}

> ${dst}/css.list
while read -r i ; do
  found="`grep "$i" ${dst}/css.list 2> /dev/null`"
  test -n "$found" || echo "$i" >> ${dst}/css.list
done < ${out}

echo "Ready"
