#!/bin/sh

set +x

branch="${1:-master}"
test -n $branch || exit 1

version="`awk -f utils/version.awk CMakeLists.txt`"
packager="`git config --get user.name`"
memail="`git config --get user.email`"
signkey="`git config --get user.signingkey`"

echo "make Package xhtmldbg-${version}"
rm -rf /tmp/xhtmldbg-${version}*
git clone ~/hjcms/xhtmldbg /tmp/xhtmldbg-${version}
pushd /tmp/xhtmldbg-${version}
  touch .git/config
  git gc --aggressive --prune=now
  git config --file .git/config --remove-section core
  git config --file .git/config remote.origin.url "$PWD"
  git config --file .git/config --add i18n.commitencoding "UTF-8"
  git config --file .git/config --add packager.version "${version}"
  git config --file .git/config --add packager.maintainer "$packager"
  git config --file .git/config --add packager.email "$memail"
  git config --file .git/config --add packager.signingkey "$signkey"
popd

pushd /tmp/
> md5sums.txt
rm -f xhtmldbg-${version}.git.tar.{bz2,lzma}
echo -e '\E[1;37;44m' " Create Archive xhtmldbg-${version}.git.tar.bz2" '\033[0m'
tar -cjf xhtmldbg-${version}.git.tar.bz2 xhtmldbg-${version}
md5sum xhtmldbg-${version}.git.tar.bz2 > md5sums.txt

echo -e '\E[1;37;44m' " Entferne Repository und Daten" '\033[0m'
rm -rf xhtmldbg-${version}/.git*

rm -f xhtmldbg-${version}.tar.{bz2,lzma}
echo -e '\E[1;37;44m' " Create Archive xhtmldbg-${version}.tar.bz2" '\033[0m'
tar -cjf xhtmldbg-${version}.tar.bz2 xhtmldbg-${version}
md5sum xhtmldbg-${version}.tar.bz2 >> md5sums.txt

echo -e '\E[1;37;44m' " Create Archive xhtmldbg-${version}.tar.lzma" '\033[0m'
tar -cJf xhtmldbg-${version}.tar.lzma xhtmldbg-${version}
md5sum xhtmldbg-${version}.tar.lzma >> md5sums.txt
popd

echo "ready"
exit 0
