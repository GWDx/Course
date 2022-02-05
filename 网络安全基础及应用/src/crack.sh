g++ -Wall -O3 crack.cpp -o crack || exit
rm out.txt
./crack 2 1 | tee -a out.txt &
time ./crack 2 2 | tee -a out.txt
