allCppSource = $(wildcard *.cpp)
allHppSource = $(wildcard *.hpp)
allTarget = $(patsubst %.cpp, %, $(allCppSource))

CFlags = -g
CFlags = -g -march=native -O3

all: $(allTarget)

a.out: a.cpp Makefile
	g++ $(CFlags) $< -o $@

test: a.out
	rm *.txt
	./a.out


clean:
	rm $(allTarget)
