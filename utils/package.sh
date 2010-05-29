#!/bin/sh

set +x

branch="${1:-master}"
test -n $branch || exit 1

version="$2"
test -n "${version}" || \
  version="$(awk -f `dirname $0`/version.awk CMakeLists.txt)"

echo -e '\E[1;37;44m' " Create Archive xhtmldbg-${version}.tar.bz2" '\033[0m'
git archive --format tar --output=xhtmldbg-${version}.tar $branch
bzip2 -z -9 xhtmldbg-${version}.tar

echo -e '\E[1;37;44m' " Create Archive xhtmldbg-${version}.tar.lzma" '\033[0m'
git archive --format tar --output=xhtmldbg-${version}.tar $branch
lzma --compress -9 xhtmldbg-${version}.tar

echo "ready"
exit 0
