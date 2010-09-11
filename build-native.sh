#!/bin/sh

test -f /usr/share/cdbs/1/class/cmake.mk || {
  echo "cdbs (>= 0.4.8) not Installed."
  exit 1
}

test -x "$(which git)" || {
  echo "git not Installed - can not read configuration"
  exit 1
}

test -f $PWD/.git/config || {
  echo "this is not a git repository"
  exit 1
}

VERSION="`git config --get packager.version`"
PACKAGER="`git config --get packager.maintainer`"
EMAIL="`git config git config --get packager.email`"
SIGN_KEY="`git config --get packager.signingkey`"

export DEBSIGN_KEYID="$SIGN_KEY"
export DEBUILD_LINTIAN=yes
export DEBUILD_LINTIAN_OPTS="-i -I.git --show-overrides"

if test -n "$SIGN_KEY" ; then
  dpkg-buildpackage -rfakeroot -k$SIGN_KEY \
    -v${VERSION} -F -sn -tc -Rdebian/rules
else
  dpkg-buildpackage -rfakeroot -v${VERSION} \
     -F -sn -tc -Rdebian/rules
fi
