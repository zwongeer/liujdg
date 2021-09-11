#!/usr/bin/bash
SHELL_FOLDER=$(dirname $(readlink -f "$0"))
cd $SHELL_FOLDER
rm -rf judger/data
rm -f judger/judger
rm -rf p1/data
rm -f p1/p1
rm -rf p2/data
rm -f p2/p2
rm -rf log
