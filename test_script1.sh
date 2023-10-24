#!/bin/bash
for i in {1..10}; do
	"./$1" $2 $3 $4
	echo ""
done

