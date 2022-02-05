cd ..
outputFile=output/result.txt
rm $outputFile
mkdir temp
g++ -o temp/1 src/1.cpp

i=0
cat input/1_1_input.txt | while read n && read p;
do
    i=$(($i+1))

    inputFile=temp/$i.txt
    echo $n > $inputFile
    echo $p >> $inputFile

    time temp/1 1000000 < $inputFile >> $outputFile
done
