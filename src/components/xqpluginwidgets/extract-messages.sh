#!/bin/sh

base=/usr/src/packages/Heinemann/Projekte/xhtmldbg/src/components/xqpluginwidgets

test -d "${base}" || {
  echo "Missing Directory"
  exit 1
}

TEMPFILE=/tmp/xhtmldbgwidgets_gettext.log
BASEDIR="src"
PROJECT="xhtmldbgwidgets"
BUGADDR="nomail@nospam.com"

> ${TEMPFILE}

echo 'i18nc("NAME OF TRANSLATORS","Your names");' >> ${TEMPFILE}

pushd ${base}
  echo "Preparing build files"
  find ../../../build/src/components/xqpluginwidgets -name 'moc*.cpp' -o -name 'ui*.h' -o -name '*.cxx' | sort > ${TEMPFILE}

  echo "Preparing uic rc files"
  test -x "$EXTRACTRC" && $EXTRACTRC `find . -o -name "*.ui"` >> rc.cpp

  echo "Preparing src files"
  find . -name '*.cpp' -o -name '*.h' | sort >> ${TEMPFILE}

  echo "Extracting messages"
  xgettext --from-code=UTF-8 -C -kde -ci18n -ki18n:1 -ki18nc:1c,2 -ki18np:1,2 -ki18ncp:1c,2,3 -ktr2i18n:1 \
    -kI18N_NOOP:1 -kI18N_NOOP2:1c,2 -kaliasLocale -kki18n:1 -kki18nc:1c,2 -kki18np:1,2 -kki18ncp:1c,2,3 \
    --msgid-bugs-address="${BUGADDR}" \
    --files-from=${TEMPFILE} -D ${BASEDIR} -D $PWD -o po/${PROJECT}.pot || {
      echo "error while calling xgettext. aborting.";
      exit 1;
  }
  rm -f rc.cpp

  echo "Merging translations"
  catalogs=`find . -name '*.po'`
  for c in po/${PROJECT}*.po; do
    echo $c
    msgmerge -o $c.new $c po/${PROJECT}.pot
    mv -b $c.new $c
  done
popd

echo "Done ..."
