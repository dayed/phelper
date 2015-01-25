#!/usr/bin/env bash

# Build and install Zephir https://github.com/phalcon/zephir
sudo mkdir ${ZEPHIRDIR}
sudo chown ${USER} ${ZEPHIRDIR}
git clone https://github.com/phalcon/zephir.git ${ZEPHIRDIR}
cd ${ZEPHIRDIR}
git fetch --all
git checkout -b ${ZEPHIR_VER} ${ZEPHIR_VER}
git submodule update --init --recursive

# json-c
cd json-c
sh autogen.sh
./configure
make && sudo make install

# zephir
cd ..
sudo ./install -c

echo -e "\e[0;32mBuild and install Zephir Done!\e[0m\n"
