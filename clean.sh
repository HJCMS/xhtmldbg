#!/bin/sh

test -d build || exit 1

rm --recursive --force build/*

cat > build/README << EOF
If this directory is empty, you probably forgot to compile xhtmldbg.
EOF

cat > build/.gitignore << EOF
Makefile
*.h
*.txt
*.cmake
*.spec
*.conf
*.service
*.xml
CMakeFiles/
app/
data/
i18n/
src/
EOF
