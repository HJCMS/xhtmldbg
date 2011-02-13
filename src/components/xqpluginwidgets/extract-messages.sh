#!/bin/sh

base=`dirname $0`

test -d "${base}/po" || {
  echo "Missing Directory"
  exit 1
}

test -d ../../../build/src/components/xqpluginwidgets && {
  ln -sf ../../../build/src/components/xqpluginwidgets build
}

TEMPFILE=/tmp/XQPluginWidgets_gettext.log
BASEDIR="src"
PROJECT="XQPluginWidgets"
BUGADDR="nomail@nospam.com"

> ${TEMPFILE}

echo 'i18nc("NAME OF TRANSLATORS","Your names");' >> ${TEMPFILE}

pushd ${base}
  echo "Preparing build files"
  find build -name 'moc*.cxx' -o -name 'ui*.h' | sort > ${TEMPFILE}

  test -f rc.cpp || extractrc `find . -maxdepth 1 -name "*.ui"` >> rc.cpp || exit 11

  echo "Preparing src files"
  find . -name '*.cpp' -o -name '*.h' | sort >> ${TEMPFILE}

  echo "Extracting messages"
  xgettext --from-code=UTF-8 -C -kde -ci18n -ki18n:1 -ki18nc:1c,2 -ki18np:1,2 -ki18ncp:1c,2,3 -ktr2i18n:1 \
    -kI18N_NOOP:1 -kI18N_NOOP2:1c,2 -kaliasLocale -kki18n:1 -kki18nc:1c,2 \
    -kki18np:1,2 -kki18ncp:1c,2,3 -ktranslate:2,1c \
    --msgid-bugs-address="${BUGADDR}" \
    --files-from=${TEMPFILE} -D ${BASEDIR} -D $PWD -o po/${PROJECT}.pot || {
      echo "error while calling xgettext. aborting.";
      exit 1;
  }

  rm -f build rc.cpp

  echo "Merging translations"
  catalogs=`find . -name '*.po'`
  for c in po/${PROJECT}*.po; do
    echo $c
    msgmerge -o $c.new $c po/${PROJECT}.pot
    mv -b $c.new $c
  done

popd

echo "Done ..."
