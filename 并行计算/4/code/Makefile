test: single test2

mnist.pkl:
	python3 mnist.py

single: mnist.pkl
	export OMP_NUM_THREADS=1; python3 single.py

test1: mnist.pkl
	export OMP_NUM_THREADS=1; mpirun -n 2 python3 parallel.py

test2: mnist.pkl
	export OMP_NUM_THREADS=1; mpirun -n 3 python3 parallel.py

