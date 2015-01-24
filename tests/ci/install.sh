#!/usr/bin/env bash

echo -e "\e[0;32mCreate install dir:\e[0m [\e[1;33m${ZEPHIRDIR}\e[0m]\n"
sudo mkdir ${ZEPHIRDIR}

echo -e "\e[0;32mChange permission to:\e[0m [\e[1;33m${USER}\e[0m]\n"
sudo chown ${USER} ${ZEPHIRDIR}

echo -e "\e[0;32mClone Zephir from https://github.com/phalcon/zephir\e[0m [\e[1;33m${ZEPHIR_VER}\e[0m]\n"
git clone https://github.com/phalcon/zephir.git ${ZEPHIRDIR}
cd ${ZEPHIRDIR}
git fetch --all
git checkout -b ${ZEPHIR_VER} ${ZEPHIR_VER}

echo -e "\e[0;32mInit git submodules...\e[0m\n"
git submodule update --init --recursive

echo -e "\e[0;32mBuild and install JSON-C...\e[0m\n"

cd json-c
sh autogen.sh
./configure
make && sudo make install


echo -e "\e[0;32mBuild and install Zephir...\e[0m\n"

cd ..
sudo ./install -c

echo -e "\e[0;32mInstallation Done!\e[0m\n"
