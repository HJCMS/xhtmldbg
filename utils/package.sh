#!/bin/sh

set +x

branch="${1:-master}"
test -n $branch || exit 1

version="$2"
test -n "${version}" || \
  version="$(awk -f `dirname $0`/version.awk CMakeLists.txt)"

ORIG_VERSION="`git config --get package.version`"
ORIG_PACKAGER="`git config --get user.name`"
ORIG_EMAIL="`git config --get user.email`"
ORIG_SIGN_KEY="`git config --get user.signingkey`"

echo "make Package xhtmldbg-${version}"
rm -rf /tmp/xhtmldbg-${version}*
git clone ~/hjcms/xhtmldbg /tmp/xhtmldbg-${version}
pushd /tmp/xhtmldbg-${version}
  touch .git/config
  git prune master
  git config --file .git/config --remove-section core
  git config --file .git/config --remove-section remote.origin
  git config --file .git/config --remove-section remote.webmast
  git config --file .git/config branch.master.remote "$PWD"
  git config --file .git/config --add packager.version "$ORIG_VERSION"
  git config --file .git/config --add packager.maintainer "$ORIG_PACKAGER"
  git config --file .git/config --add packager.email "$ORIG_EMAIL"
  git config --file .git/config --add packager.signingkey "$ORIG_SIGN_KEY"
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
