#! /bin/bash


mainPath="/home/tefan/Facultate/PPD/ConvolutionAlgorithms/lab3/main.cpp"
declare -a arr=(5 9 21)
cd "/home/tefan/Facultate/PPD/ConvolutionAlgorithms"
echo "localhost slots=21" > hostfile

mpic++.openmpi -D TIME_T1 "$mainPath" -o "mainT1"
mpic++.openmpi -D TIME_T2 "$mainPath" -o "mainT2"
verifyAlg="$(pwd)/verify/verifyAlg"
inputFile="$(pwd)/verify/input_file.txt"
outputVerify="$(pwd)/output_verify.txt"

outputProgram="$(pwd)/output_program.txt"
touch $outputProgram
mainT1="./mainT1"
mainT2="./mainT2"

$verifyAlg $inputFile $outputVerify
echo -n "" >results_lab3.csv
echo "Started T1"
for i in "${arr[@]}"; do
  time_result=0
  echo "P=$i"
  for j in {1..10}; do
    echo -n "*" 
    temp=$(mpirun.openmpi --hostfile hostfile -n $i $mainT1 $i $inputFile $outputProgram )
    if [[ -n "$(diff -q $outputVerify $outputProgram)" ]]; then
			echo "Wrong answer for cpp algorithm on test with $i programs"
			exit 1
		fi
    time_result=$((time_result + temp))
	done
  echo ""
  time_result=$((time_result/10))
  echo "T1: n=1000, m=1000 : $i : $time_result" >> results_lab3.csv
done

echo "Started T2"
for i in "${arr[@]}"; do
  time_result=0
  echo "P=$i"
  for j in {1..10}; do
    echo -n "*"
    temp=$(mpirun.openmpi --hostfile hostfile -n $i $mainT2 $i $inputFile $outputProgram )
    if [[ -n "$(diff -q $outputVerify $outputProgram)" ]]; then
			echo "Wrong answer for cpp algorithm on test with $i programs"
			exit 1
		fi
    time_result=$((time_result + temp))
	done
  echo ""
  time_result=$((time_result/10))
  echo "T2: n=1000, m=1000 : $i : $time_result" >> results_lab3.csv
done

rm "$outputVerify"
rm "$outputProgram"
rm "$mainT1"
rm "$mainT2"
