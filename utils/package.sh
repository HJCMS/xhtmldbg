#!/bin/sh

set +x

LC_ALL=C

branch="${1:-master}"
test -n $branch || {
  echo "usage $0 <branch>"
  exit 1
}

## read configure
version="`awk -f utils/version.awk CMakeLists.txt`"
packager="`git config --get user.name`"
memail="`git config --get user.email`"
signkey="`git config --get user.signingkey`"
gitrev="git$(date +"%j")"

echo "make Package xhtmldbg-${version}"

## cleanup destdir
rm -rf /tmp/xhtmldbg-${version}*

## clone my repository
git clone ~/hjcms/xhtmldbg /tmp/xhtmldbg-${version}

pushd /tmp/xhtmldbg-${version}
  ## cleanup Repository configuration
  touch .git/config
  git gc --aggressive --prune=now
  git config --file .git/config --remove-section core
  git config --file .git/config remote.origin.url "$PWD"
  git config --file .git/config --add i18n.commitencoding "UTF-8"
  git config --file .git/config --add packager.version "${version}"
  git config --file .git/config --add packager.maintainer "$packager"
  git config --file .git/config --add packager.email "$memail"
  git config --file .git/config --add packager.signingkey "$signkey"
## Edit debian/watch for ftp Patch Level Updates
cat > debian/watch <<EOF
# Compulsory line, this is a version 3 file
version=3
# only for patch levels
ftp://ftp.hjcms.de/xhtmldbg/${version}/xhtmldbg-${version}(.*)\.tar\.bz2 debian uupdate
EOF
  git commit -m "(XHTMLDBG) Version ${version}" .
popd

pushd /tmp/
## Edit Group
chgrp -R public xhtmldbg-${version}

## Build md5sum file
> md5sums.txt

## Generate Repository Archive
rm -f xhtmldbg-${version}.${gitrev}.tar.{bz2,lzma}
echo -e '\E[1;37;44m' " Create Archive xhtmldbg-${version}.${gitrev}.tar.bz2" '\033[0m'
tar -cjf xhtmldbg-${version}.${gitrev}.tar.bz2 xhtmldbg-${version}
md5sum xhtmldbg-${version}.${gitrev}.tar.bz2 > md5sums.txt

## remove repository data to make compressed archives smaller ;-)
rm -rf xhtmldbg-${version}/.git*

## Generate bz2 Archive
rm -f xhtmldbg-${version}.tar.{bz2,lzma}
echo -e '\E[1;37;44m' " Create Archive xhtmldbg-${version}.tar.bz2" '\033[0m'
tar -cjf xhtmldbg-${version}.tar.bz2 xhtmldbg-${version}
md5sum xhtmldbg-${version}.tar.bz2 >> md5sums.txt

## Generate lzma Archive
echo -e '\E[1;37;44m' " Create Archive xhtmldbg-${version}.tar.lzma" '\033[0m'
tar -cJf xhtmldbg-${version}.tar.lzma xhtmldbg-${version}
md5sum xhtmldbg-${version}.tar.lzma >> md5sums.txt
popd

echo "ready"

exit 0
