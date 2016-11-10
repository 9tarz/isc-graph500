# Hybrid MPI and OpenMP Graph500 implementation 
Based on Boston University High Performance Computing's implementation [isc16-graph500](https://github.com/buhpc/isc16-graph500).

## Make 
1. Run $make

## Running
Typical usage:
- Single node: $mpirun ./main scale edgefactor
- Cluster:  $mpirun -machinefile machinefile -np numberofprocesses ./main scale edgefactor
