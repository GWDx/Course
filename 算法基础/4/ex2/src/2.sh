g++ -Wall task.cpp -o task || exit
./task 1>../output/result.txt 2>../output/time.txt

cat ../output/result.txt

echo
cat ../output/time.txt
