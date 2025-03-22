#!/bin/bash
cd "$(dirname "$0")"
mkdir -p build;
cd build;
qmake ../src/tablet-switch.pro;
make -j$(nproc);
