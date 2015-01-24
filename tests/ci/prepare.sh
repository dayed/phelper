#!/usr/bin/env bash

declare -r CURRENT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")"/../../ && pwd)"
cd "${CURRENT_DIR}"
echo -e "\e[0;32mCurrent dir:\e[0m [\e[1;33m${CURRENT_DIR}\e[0m]\n"

echo -e "\e[0;32mGenerate C code from the Zephir code\e[0m\n"
zephir generate

echo -e "\e[0;32mGeneration Done!\e[0m\n"

cd ./ext

phpize --silent
./configure --silent
make --silent && sudo make --silent install

phpenv config-add ../tests/ci/phelper.ini
phpenv rehash

cd "${CURRENT_DIR}"
