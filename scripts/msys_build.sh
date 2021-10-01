#!/usr/bin/bash
SHELL_FOLDER=$(dirname $(readlink -f "$0"))
cd $SHELL_FOLDER/..

mkdir -p build
cd build

make -j$(nproc) || exit 1

read -n1 -p "Do you want to copy dll(s)?[y/n]" answer
echo
case $answer in
Y | y)
    ;;
N | n)
    exit;;
*)
    echo "error choice"
    exit 1;;
esac
cd tools
for f in *.exe; do
	echo "Copying Dll(s) for $f"
	$SHELL_FOLDER/copyDlls.sh "$MSYSTEM_PREFIX/bin" $f > /dev/null 2>&1 || exit 1
done