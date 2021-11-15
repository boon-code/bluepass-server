#!/bin/sh

set -e

rm -rf appdir/
cp -r appdir-template/ appdir/
install -m 0755 -D -t appdir/usr/bin/ BluePassServer

# DEBUG:
ldd $HOME/Qt/5.15.2/gcc_64/plugins/platforms/libqxcb.so

linuxdeployqt appdir/usr/share/applications/BluePassServer.desktop -appimage
