import random
import os
import sys

lenV = int(sys.argv[1])
lenE = int(sys.argv[2])

os.system(
    f'./PaRMAT -nVertices {lenV} -nEdges {lenE} -output data.txt -noEdgeToSelf -sorted -threads 6 -noDuplicateEdges')

os.system(f'./generateGraph {lenV} {lenE}')
