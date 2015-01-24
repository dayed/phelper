#!/usr/bin/env bash

INSTALL_DIR=/usr/local/src/zephir

echo -e "\n\e[0;32mCreate install dir:\e[0m [\e[1;33m${INSTALL_DIR}\e[0m]"
echo -e ""

sudo mkdir ${INSTALL_DIR}

echo -e "\n\e[0;32mChange permission to:\e[0m [\e[1;33m${USER}\e[0m]"
echo -e ""

sudo chown ${USER} ${INSTALL_DIR}

echo -e "\n\e[0;32mClone Zephir from https://github.com/phalcon/zephir\e[0m [\e[1;33m${ZEPHIR_VER}\e[0m]"
echo -e ""

git clone https://github.com/phalcon/zephir.git ${INSTALL_DIR}
cd ${INSTALL_DIR}
git fetch --all
git checkout -b ${ZEPHIR_VER} ${ZEPHIR_VER}
git submodule update --init

echo -e "\n\e[0;32mBuild and install JSON-C...\e[0m"
echo -e ""

cd json-c
sh autogen.sh
./configure
make && sudo make install


echo -e "\n\e[0;32mBuild and install Zephir...\e[0m"
echo -e ""

cd ..
./install -c
