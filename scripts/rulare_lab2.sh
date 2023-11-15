#!/bin/bash

java_command="java -cp $(pwd)/blockingJava/src/out Main "
cpp_command="./blockingCpp/main"
verifier="./verifyAlg"
echo $(pwd)
file1="./data/data1.txt"
file2="./data/data2.txt"
file3="./data/data3.txt"
echo "" > results.csv
echo "Started test for data1.txt..."
$verifier $file1 "out_correct.txt"
for i in {0..1}; do
	p=$((2**i))
      	echo "Threads $p" 
	time_result_cpp=0
	time_result_java=0

        for j in {1..10}; do
		temp=$($cpp_command $p $file1 "out_cpp.txt")
                time_result_cpp=$((time_result_cpp + temp))
                                
                temp=$($java_command $file1 $p "out_java.txt" )
                time_result_java=$((time_result_java + temp))
        	if [[ -n "$(diff -q out_cpp.txt out_correct.txt)" ]]; then
			echo "Wrong answer for cpp algorithm on $file1"
			exit 1
		fi
		if [[ -n "$(diff -q out_java.txt out_correct.txt)" ]]; then
			echo "Wrong answer for java algorithm on $file1"
			exit 1
		fi


	done 
        time_result_cpp=$((time_result_cpp/10))
        time_result_java=$((time_result_java/10))
	echo "n=10, m=10 : $p : $time_result_cpp : $time_result_java" >> results.csv
done
echo "Started test for data2.txt..."
$verifier $file2 "out_correct.txt"
for i in {0..4}; do
	p=$((2**i))
       	echo "Threads $p"
	time_result_cpp=0
	time_result_java=0
	
        for j in {1..10}; do
		temp=$($cpp_command $p $file2 "out_cpp.txt")
                time_result_cpp=$((time_result_cpp + temp))
                                
                temp=$($java_command $file2 $p "out_java.txt" )
                time_result_java=$((time_result_java + temp))
        	if [[ -n "$(diff -q out_cpp.txt out_correct.txt)" ]]; then
			echo "Wrong answer for cpp algorithm on $file2"
			exit 1
		fi
		if [[ -n "$(diff -q out_java.txt out_correct.txt)" ]]; then
			echo "Wrong answer for java algorithm on $file2"
			exit 1
		fi


	done 
        time_result_cpp=$((time_result_cpp/10))
        time_result_java=$((time_result_java/10))
	echo "n=1000, m=1000 : $p : $time_result_cpp : $time_result_java" >> results.csv
done
echo "Started test for data3.txt..."
$verifier $file3 "out_correct.txt"

for i in {0..4}; do
	p=$((2**i))
	echo "Threads $p"
       	time_result_cpp=0
	time_result_java=0
	        for j in {1..10}; do
		echo "Run $j"
		temp=$($cpp_command $p $file3 "out_cpp.txt")
                time_result_cpp=$((time_result_cpp + temp))
                                
                temp=$($java_command $file3 $p "out_java.txt" )
                time_result_java=$((time_result_java + temp))
        	if [[ -n "$(diff -q out_cpp.txt out_correct.txt)" ]]; then
			echo "Wrong answer for cpp algorithm on $file3"
			exit 1
		fi
		if [[ -n "$(diff -q out_java.txt out_correct.txt)" ]]; then
			echo "Wrong answer for java algorithm on $file3"
			exit 1
		fi


	done 
        time_result_cpp=$((time_result_cpp/10))
        time_result_java=$((time_result_java/10))
	echo "n=10000, m=10000 : $p : $time_result_cpp : $time_result_java" >> results.csv
done

rm ./out*.txt



