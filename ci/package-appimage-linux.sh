#!/bin/sh

set -e

rm -rf appdir/
cp -r appdir-template/ appdir/
install -m 0755 -D -t appdir/usr/bin/ BluePassServer

linuxdeployqt appdir/usr/share/applications/BluePassServer.desktop -appimage
