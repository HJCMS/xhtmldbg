#!/bin/sh

kdesdkdir=$HOME/hjcms/Translation/scripts/
base=$HOME/hjcms/xhtmldbg

test -d "${base}" || {
  echo "Missing Directory"
  exit 1
}

XGETTEXT=${XGETTEXT:-xgettext}
EXTRACTRC=${EXTRACTRC:-extractrc}
MSGMERGE=${MSGMERGE:-msgmerge}
TEMPFILE=/tmp/xhtmldbg_gettext.log
BASEDIR="src"
PROJECT="xhtmldbg"
BUGADDR="`git config author.email`"
XGETTEXT_FLAGS_KDE="-ci18n -ki18n:1 -ki18nc:1c,2 -ki18np:1,2 -ki18ncp:1c,2,3 -ktr2i18n:1 -kI18N_NOOP:1 -kI18N_NOOP2:1c,2 -kaliasLocale -kki18n:1 -kki18nc:1c,2 -kki18np:1,2 -kki18ncp:1c,2,3"
XGETTEXT_FLAGS_QT="-ctrUtf8 -ktrUtf8:1"

> ${TEMPFILE}

echo 'i18nc("NAME OF TRANSLATORS","Juergen Heinemann");' >> ${TEMPFILE}

pushd ${base}
  echo "Preparing build files"
  find build -name 'moc*.cpp' -o -name '*_automoc.cpp' -o -name 'ui*.h' -o -name '*.cxx' | sort > ${TEMPFILE}

  echo "Preparing src files"
  find src -name '*.cpp' -o -name '*.h' | sort >> ${TEMPFILE}

  echo "Extracting messages"
  ${XGETTEXT} --from-code=UTF-8 -C -kde ${XGETTEXT_FLAGS_KDE} ${XGETTEXT_FLAGS_QT} \
    --msgid-bugs-address="${BUGADDR}" --copyright-holder="LGPLv3" \
    --files-from=${TEMPFILE} -D ${BASEDIR} -D $PWD -o po/${PROJECT}.pot || {
      echo "error while calling xgettext. aborting.";
      exit 1;
  }

  echo "Merging translations"
  catalogs=`find . -name '*.po'`
  for c in po/${PROJECT}*.po; do
    echo $c
    ${MSGMERGE} -o $c.new $c po/${PROJECT}.pot
    mv -b $c.new $c
  done

popd

echo "Done ..."
