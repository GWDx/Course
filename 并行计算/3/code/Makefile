AllCpp = $(wildcard *.cpp)
AllTarget = $(patsubst %.cpp, %, $(AllCpp))

cFlags = -g -Wall -O0
cFlags = -g -Wall -O2

all: $(AllTarget)

%: %.cpp
	mpic++ $(cFlags) -o $@ $<

generateMatrix: $(AllTarget)
	./generate 20000 20000 10000000

test: $(AllTarget) generateMatrix
# 	./simple > standard.txt

# 	mpirun -np 1 parallelComm > out.txt
# 	mpirun -np 2 parallelComm > out.txt
# 	mpirun -np 3 parallelComm > out.txt
# 	mpirun -np 4 parallelComm > out.txt

	mpirun -np 1 parallelShare > out.txt
	mpirun -np 2 parallelShare > out.txt
	mpirun -np 3 parallelShare > out.txt
	mpirun -np 4 parallelShare > out.txt

# 	diff standard.txt out.txt
# 	mpirun -np 3 parallelshare
