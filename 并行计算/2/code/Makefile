CC = g++ -g -Wall -O3 -march=native
# CC = g++ -g -Wall

deltaStepping5: deltaStepping5.cpp
	$(CC) -fopenmp -o $@ $<

dijkstra: dijkstra.cpp
	$(CC) -o $@ $<

generate: generateGraph genGraph.py
	python genGraph.py 2000000 100000000

generateGraph: generateGraph.cpp
	$(CC) -o $@ $<

test: data.txt deltaStepping5
	./deltaStepping5 1 10 > out.txt
	./deltaStepping5 2 10 > out.txt
	./deltaStepping5 3 10 > out.txt
	./deltaStepping5 4 10 > out.txt

correct: deltaStepping5 dijkstra
	python genGraph.py 1000 10000 > /dev/null
	./dijkstra > correct.txt
	./deltaStepping5 3 10 > out.txt
	diff out.txt correct.txt
