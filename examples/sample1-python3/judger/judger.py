#!/usr/bin/python3

import os
import sys
import random
import time

playerNum = int(input())
path = input()
with open(path + "fk.txt", 'w') as out:
    out.write("start\n")
    
    out.write("{}\n{}\n".format(playerNum, path))

    print("#send 1")
    print("1")
    print("#continue 1")
    sys.stdout.flush()

    pair = input().split()
    p = int(pair[0])
    ret = str(pair[1])
    print("#send 2")
    print("2")
    print("#continue 2")
    sys.stdout.flush()
    
    print("#log")
    print("test log")
    sys.stdout.flush()

    pair = input().split()
    p = int(pair[0])
    ret = str(pair[1])
    print("#win 1")
    print("1 0")
    print("MLG pro from python")
    sys.stdout.flush()
