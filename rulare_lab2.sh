#!/bin/bash

java_command="java -cp /home/tefan/Facultate/PPD/ConvolutionAlgorithms/blockingJava/src/out Main "
cpp_command="./blockingCppAlg"
verifier="./verifyAlg"
echo $(pwd)
for file in $(ls ./data); do
	./verifyAlg "./data/$file" ./out3.txt
	$java_command "./data/$file" 2 ./out2.txt
	$cpp_command 2 "./data/$file" ./out1.txt
	diff -qs out1.txt out3.txt
	diff -qs  out2.txt out3.txt
	rm ./out*.txt
	

done



