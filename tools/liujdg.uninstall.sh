#!/usr/bin/bash
if [ `whoami` = "root" ];then
	rm -f /usr/local/bin/liujdg.*
	echo "Success"
else
	echo "The shell must be run as root"
	exit
fi