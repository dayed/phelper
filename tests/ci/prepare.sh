#!/usr/bin/env bash

declare -r CURRENT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")"/../../ && pwd)"
cd "${CURRENT_DIR}"
echo -e "\e[0;32mCurrent dir:\e[0m [\e[1;33m${CURRENT_DIR}\e[0m]\n"

echo -e "\e[0;32mBuild Phelper extension: \e[0m\n"
sudo zephir build && echo -e '[\033[0;32mBuild Done!\033[0m]\n' || echo -e '[\033[0;31mBuild Fail!\033[0m]'; exit 1

phpenv config-add ./tests/ci/phelper.ini
phpenv rehash

cd "${CURRENT_DIR}"
