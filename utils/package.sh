#!/bin/sh

set +x

branch="${1:-master}"
test -n $branch || exit 1

version="$2"
test -n "${version}" || \
  version="$(awk -f `dirname $0`/version.awk CMakeLists.txt)"

rm -f /tmp/xhtmldbg-${version}_list.tar
git archive --format=tar --output=/tmp/xhtmldbg-${version}_list.tar $branch

mkdir /tmp/xhtmldbg-${version}
pushd /tmp/xhtmldbg-${version}
  tar -xf /tmp/xhtmldbg-${version}_list.tar
popd

pushd /tmp/
> md5sums.txt
rm -f xhtmldbg-${version}.tar.{bz2,lzma}
echo -e '\E[1;37;44m' " Create Archive xhtmldbg-${version}.tar.bz2" '\033[0m'
tar -cjf xhtmldbg-${version}.tar.bz2 xhtmldbg-${version}
md5sum xhtmldbg-${version}.tar.bz2 > md5sums.txt

echo -e '\E[1;37;44m' " Create Archive xhtmldbg-${version}.tar.lzma" '\033[0m'
tar -cJf xhtmldbg-${version}.tar.lzma xhtmldbg-${version}
md5sum xhtmldbg-${version}.tar.lzma >> md5sums.txt
popd

echo "ready"
exit 0
