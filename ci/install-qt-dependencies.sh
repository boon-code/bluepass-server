#!/bin/sh

set -e

export DEBIAN_FRONTEND=noninteractive

sudo apt-get update
sudo apt-get install -y \
    ca-certificates \
    curl \
    libglib2.0-bin \
    libgl-dev \
    libdbus-1-3 \
    libxrender1 \
    libfontconfig1 \
    libxkbcommon-x11-0 \
    libegl1-mesa \
    libxcb-icccm4 \
    libxcb-image0 \
    libxcb-keysyms1 \
    libxcb-render-util0-dev \
    libxcb-xinerama0 \
    file
