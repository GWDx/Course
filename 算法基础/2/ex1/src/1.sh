make || exit

# ./test
./task 1>../output/result.txt 2>../output/time.txt
echo
cat ../output/result.txt

echo
cat ../output/time.txt
