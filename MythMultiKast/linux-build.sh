#!/bin/bash
rm *.lib
rm -rf SDL2
rm -rf live555

make -j && sudo make install
