#!/bin/sh
#################################################################
##  example script for distributed package building
#################################################################

LC_ALL=C

test -x $(which xsltproc) || exit 1

## xhtmldbg download location
baseurl=http://gitweb.hjcms.de/cgi-bin/index.cgi/xhtmldbg/snapshot

## xhtmldbg version.xml location
xmlurl=http://xhtmldbg.hjcms.de/version.xml

## current stable package version
version="$(xsltproc `dirname $0`/version.xslt ${xmlurl})"

## release date
released="$(xsltproc `dirname $0`/released.xslt ${xmlurl})"

## convert from 0000-00-00T00:00:+00:00 to %c
formated="`date -u -d "$(echo ${released} | cut -d: -f1,2)" +"%c"`"

## print summary
cat <<EOF
  Name:     xhtmldbg
  Version:  ${version}
  Date:     ${released}
  Formated: ${formated}
  Download: ${baseurl}/xhtmldbg-${version}.tar.gz
EOF
