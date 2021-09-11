#!/usr/bin/bash
SHELL_FOLDER=$(dirname $(readlink -f "$0"))
cd $SHELL_FOLDER
mkdir -p build
cd build
make -j$(nproc) || exit 1
echo "(root permission required)"
read -n1 -p "Do you want to install liujdg?[y/n]" answer
echo
case $answer in
Y | y)
    sudo ./liujdg.install;;
N | n)
    echo "not install liujdg"
    true;;
*)
    echo "error choice"
    exit 1;;
esac
