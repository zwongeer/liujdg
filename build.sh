#!/usr/bin/bash
SHELL_FOLDER=$(dirname $(readlink -f "$0"))
cd $SHELL_FOLDER
mkdir -p build
cd build
if [ "$1" == "d" -o "$1" == "debug" -o "$1" == "Debug" ]; then
    cmake -DCMAKE_BUILD_TYPE=Debug .. || exit 1
else
    cmake -DCMAKE_BUILD_TYPE=Release .. || exit 1
fi
make || exit 1
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
