#!/bin/bash

runner=$1

echo "Attempting to compile c code..."
gcc -o main *.c
runner="./main "

for value in {1..9}
do
	echo ""
	echo "Running ${value}.code"
	timeout 5 ${runner} P4TestCases/${value}.code P4TestCases/${value}.data > P4TestCases/${value}.student
	echo "Running diff with ${value}.expected"
	grep -o '[[:digit:]]\+' P4TestCases/${value}.student > P4TestCases/temp1
	grep -o '[[:digit:]]\+' P4TestCases/${value}.expected > P4TestCases/temp2
	if cmp -s "P4TestCases/temp1" "P4TestCases/temp2"; then
		echo "Print looks good"
		score=$(($score + 1))
	else
		echo "Student output and expected output are different"
	fi
done

rm P4TestCases/temp1
rm P4TestCases/temp2

echo ""
echo "Running error cases:"
echo ""

echo "Running 00.error:"
echo "-----Student Output Starts-----"
timeout 5 ${runner} P4Error/00.code P4Error/00.data
echo "-----Student Output Ends-----"
read -n 1 -p "Error function body missing (no stmt-seq). Error message related to that? (y/n)" mainmenuinput
if [ $mainmenuinput = "y" ]; then
	error=$(($error + 1))
fi
echo ""
echo ""

echo "Running 01.error:"
echo "-----Student Output Starts-----"
timeout 5 ${runner} P4Error/01.code P4Error/01.data
echo "-----Student Output Ends-----"
read -n 1 -p "Error calling a function that has not been declared. Error message related to that? (y/n)" mainmenuinput
if [ $mainmenuinput = "y" ]; then
	error=$(($error + 1))
fi
echo ""
echo ""

echo "Running 02.error:"
echo "-----Student Output Starts-----"
timeout 5 ${runner} P4Error/02.code P4Error/02.data
echo "-----Student Output Ends-----"
read -n 1 -p "Error number of arguments does not match number of formal parameters. Error message related to that? (y/n)" mainmenuinput
if [ $mainmenuinput = "y" ]; then
	error=$(($error + 1))
fi
echo ""
echo ""

echo "Running 03.error:"
echo "-----Student Output Starts-----"
timeout 5 ${runner} P4Error/03.code P4Error/03.data
echo "-----Student Output Ends-----"
read -n 1 -p "Semantic error, duplicate function name. Error message related to that? (y/n)" mainmenuinput
if [ $mainmenuinput = "y" ]; then
	error=$(($error + 1))
fi
echo ""
echo ""



echo "Correct cases score out of 9:"
echo $score
echo "Error cases score out of 4:"
echo $error

echo "Done!"