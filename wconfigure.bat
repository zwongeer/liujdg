@echo off
:: set current working path to the path of the script file
cd /d %~dp0
mkdir build
cd build
:: note: may need to change the vcpkg path
cmake -G "NMake Makefiles" "-DCMAKE_TOOLCHAIN_FILE=E:/vcpkg/scripts/buildsystems/vcpkg.cmake" -DCMAKE_BUILD_TYPE=Release -DBoost_NO_WARN_NEW_VERSIONS=1 -D_WIN32_WINNT=0x0601 ..
:: uncomment to build the project
::nmake