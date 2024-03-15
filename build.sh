#!/bin/bash

rm -rfv build
rm -rfv outputVideo.mp4
rm test

mkdir build
cd build

cmake ../.
make

cp test ../.
cd ..

./test
