#!/usr/bin/bash

    
echo "---- good files ----"
for testfile in good*.bminor
do
    .././bminor -print $testfile > $testfile.out1
    ./../bminor -print $testfile.out1 > $testfile.out2

    if diff -q $testfile.out1 $testfile.out2;
    then
        echo "$testfile --> success (as expected)"
    else 
        echo "$testfile --> failure (INCORRECT)"
        diff -q temp temp2
    fi
done

echo "---- bad files ----"
for tfile in bad*.bminor
do
    if ./../bminor -print $tfile > /dev/null
    then
        echo "$tfile --> success (INCORRECT)"
    else 
        echo "$tfile --> failure (as expected)"
    fi
done
