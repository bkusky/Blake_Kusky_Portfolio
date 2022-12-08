#!/usr/bin/bash

./bminor -codegen test.bminor out.s
if [ $? -eq 0 ]
then
    gcc library.c out.s -o program
    if [ $? -eq 0 ]
    then
        ./program
    fi
fi
