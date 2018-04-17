#!/bin/bash
cd ..;
rm -fr build;
mkdir build;
meson build;
cd build;
ninja;
