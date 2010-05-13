#!/bin/sh

set +x

branch="${1:-master}"
test -n $branch || exit 1

version="$(awk -f `dirname $0`/version.awk CMakeLists.txt)"

echo -e '\E[1;37;44m' " Create Archive xhtmldbg-${version}.tar" '\033[0m'
git archive --format tar --output=xhtmldbg-${version}.tar $branch


echo -e '\E[1;37;44m' " Compress xhtmldbg-${version}.tar.lzma" '\033[0m'
lzma --compress -9 xhtmldbg-${version}.tar

echo ""
