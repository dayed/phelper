#!/usr/bin/env bash

INSTALL_DIR=/usr/local/src/zephir

echo -e "\n\e[0;32mBuild and install Zephir from https://github.com/phalcon/zephir\e[0m [\e[1;33m${ZEPHIR_VER}\e[0m]"
echo -e ""

sudo mkdir $INSTALL_DIR
sudo chown $USER $INSTALL_DIR
git clone https://github.com/phalcon/zephir.git $INSTALL_DIR

cd $INSTALL_DIR
git checkout ${ZEPHIR_VER}
git submodule update --init

cd json-c
sh autogen.sh
./configure
make && sudo make install

cd ..
sudo ./install -c