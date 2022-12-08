#!/usr/bin/bash

echo "---- testing files ----"

for tfile in good*.bminor
do
	echo "*testing $tfile*"
	.././bminor -codegen $tfile $tfile.s
	if [ $? -eq 0 ]
	then
		gcc -g $tfile.s ../library.c -o $tfile.exe

		if [ $? -eq 0 ]
		then
			./$tfile.exe > $tfile.output
			
			if [ $? -eq 0 ]
			then
				echo "<> $tfile ---> completion with return code 0 (SUCCESS) <>" 
			else
				echo "<> $tfile ---> completion with nonzero return code (INCORRECT) <>"
			fi
		else
			echo "<> $tfile ---> compiling/linking failure (INCORRECT) <>"
		fi
	else
		echo "<> $tfile ---> codegen failure (INCORRECT) <>"
	fi
done
