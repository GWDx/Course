allCppSource = $(wildcard *.cpp)
allHppSource = $(wildcard *.hpp)
allTarget = $(patsubst %.cpp, %, $(allCppSource))

CFlags = -g
CFlags = -g -march=native -O3 -Wall

all: $(allTarget)

%: %.cpp Makefile $(allHppSource)
	g++ $(CFlags) $< -o $@

test: $(allTarget)
	./csp

clean:
	rm $(allTarget)
