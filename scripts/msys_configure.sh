#!/usr/bin/bash
SHELL_FOLDER=$(dirname $(readlink -f "$0"))
cd $SHELL_FOLDER/..

mkdir -p build
cd build

if [ -z $1 ]; then
	TOOLCHAIN_FILE="E:/mdata/msys_vcpkg/scripts/buildsystems/vcpkg.cmake"
else
	TOOLCHAIN_FILE=$1
fi


if [ -z $2 ]; then
	TRIPLET=x64-mingw-static
else
	TRIPLET=$2
fi

cmake .. -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release "-DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE" -DVCPKG_TARGET_TRIPLET=$TRIPLET || exit 1

cd $SHELL_FOLDER/..
./tools/deamon/generate.py
