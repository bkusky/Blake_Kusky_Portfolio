#!/usr/bin/bash

    
echo "---- good files ----"
for tfile in good*.bminor
do
    .././bminor -typecheck $tfile > $tfile.output

    if [ $? -eq 0 ]
    then
        echo "$tfile --> success (as expected)"
    else 
        echo "$tfile --> failure (INCORRECT)"
    fi
done

echo "---- bad files ----"
for tfile in bad*.bminor
do
    .././bminor -typecheck $tfile > $tfile.output

    if [ $? -eq 0 ]
    then
        echo "$tfile --> success (INCORRECT)"
    else 
        echo "$tfile --> failure (as expected)"
    fi
done
