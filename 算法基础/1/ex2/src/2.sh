cd ..
outputFile1=output/result.txt
outputFile2=output/result2.txt
rm $outputFile1 $outputFile2
mkdir temp
g++ -o temp/2-1 src/2-1.cpp

i=0
cat input/1_2_input.txt | while read length;
do
    read X
    read Y
    i=$(($i+1))

    inputFile=temp/$i.txt
    echo $length > $inputFile
    echo $X >> $inputFile
    echo $Y >> $inputFile

#     time temp/2-1 1 < $inputFile >> /dev/null
    time temp/2-1 1000000 < $inputFile >> $outputFile1
#     time temp/2-1 10000 < $inputFile >> $outputFile1

    python3 src/2-2.py < $inputFile >> $outputFile2
done
