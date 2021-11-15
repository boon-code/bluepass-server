#!/bin/sh

set -e

bindir="$1"

[ -n "$bindir" ] || { echo "bindir not set $bindir" >&2; exit 1; }

mkdir -p "$bindir"

[ -d "$bindir" ] || { echo "bindir is not a directory: $bindir" >&2; exit 1; }

wdir="$(pwd)"
cd "/tmp"
curl -L --fail -o linuxdeployqt 'https://github.com/probonopd/linuxdeployqt/releases/download/7/linuxdeployqt-7-x86_64.AppImage'

echo "7db8d32fe005ff04fb4876f0018b533bf58cd46f90b838ab0bc97a129f82623cb8c0f83828a58531a1ccf18e3c88ff55e4f512fc809157eb6bef21f1a4ad23a5  linuxdeployqt" | sha512sum -c -

chmod a+x "linuxdeployqt"

cd "$wdir"

cp -v "/tmp/linuxdeployqt" "${bindir}/linuxdeployqt"
echo "linuxdeployqt was installed to $bindir"
