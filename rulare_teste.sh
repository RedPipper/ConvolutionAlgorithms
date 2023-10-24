#!/bin/bash

cd "/home/tefan/Facultate/Programare Paralela si Distribuita/Homework/ConvolutionAlgorithms"
rm ./results/*
algorithms=("indexed" "lined" "columned")
for algorithm in "${algorithms[@]}"; do
	static_result_name="result_static_$algorithm.txt"
	dynamic_result_name="result_dynamic_$algorithm.txt"
	java_result_name="result_java_$algorithm.txt"
	for data_file in $(ls | grep -E "data[0-9].txt"); do
		echo "Starting tests for $algorithm algorithm using $data_file data_file..."
		echo "DATAFILE $data_file" >> "./results/$static_result_name"
		echo "DATAFILE $data_file" >> "./results/$dynamic_result_name"
		
		for i in {0..5}; do
			p=$((2**i))
			
			time_result_static=0
			time_result_dynamic=0
			time_result_java=0
			for j in {1..10}; do
				temp=$(./static $p $algorithm $data_file)
				time_result_static=$((time_result_static + temp))

				temp=$(./dynamic $p $algorithm $data_file)
				time_result_dynamic=$((time_result_dynamic + temp))
				
				temp=$(java --class-path "./conv_algs_java/out" Main $p $algorithm $data_file)
				time_result_java=$((time_result_java + temp))
			done 
			time_result_static=$((time_result_static/10))
			time_result_dynamic=$((time_result_dynamic/10))
			time_result_java=$((time_result_java/10))

			echo "Result for $p threads: $time_result_static" >> "./results/$static_result_name"
			echo "Result for $p threads: $time_result_dynamic" >> "./results/$dynamic_result_name"
			echo "Result for $p threads: $time_result_java" >> "./results/$java_result_name"
			echo "Done for $p"
		done
	done
done


