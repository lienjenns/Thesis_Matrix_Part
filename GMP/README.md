Given a number of processors p, and a load-imbalance parameter epsilon, the algorithm determines
the optimal partition of a matrix A into p parts. This algorithm uses a branch and bound approach.

The number of parts/processors p, the load-imbalance parameter epsilon,
the name of the matrix and the location of the matrix (in matrix market format) are all user inputs.

# Build:

## Windows:
Build with micrososft visual studio in release mode(x64), the executable can be found in the x64 directory!

## Linux(Ubuntu)
Build on Ubuntu 18.04, in order to build the project Windows specific code needs to be removed. 
This includes :#include "windows.h" in Main.cpp and Possible_States.cpp. 
In Possible_States.cpp line 1142-1156 needs to be removed, since this uses the windows.h
   


# Input ;

```
.\GMP "name_matrix" "location_matrix" "p" "epsilon"
```

For example:
```
.\GMP "b1_ss" "matrix/b1_ss.mtx" "4" "0.03"
```
This command will partition the matrix "b1_ss" that can be found in the directory "matrix" into 4 parts, 
and the load imbalance parameter epsilon is set to 0.03.

# Output:

Every time the algorithm finds an improvement over the previous best found solution 
it is printed in the console screen (both the solution and the communication value are printed).
The algorithm also keeps track of the number of partitions that were aborted, because LB>=UB.
Before the improved solution is printed, it first prints the current number of aborted solutions.

The algorithm prints the number of aborted solutions 
each time the number of aborted solutions is a multiple of ten million.

At the end, the time it took to compute the optimal solution, the number of aborted partitions,
an optimal solution and the corresponding optimal communication value are printed.


# Output files:

Partitioned matrix:
The partitioned matrix in matrix market format is printed in the file p="#processors" "matrix", OUT.txt , where #processors and matrix are input variables.
In the comments of this file the value of p is mentioned, the maximum allowed size of a part is mentioned and the found optimal communication volume is mentioned.
Also for all possible subsets of processors the number of nonzeros assigned to that subset is mentioned, in order of binary index of subset of processors.
For example the subset of processors {0,1} has binary index 2^0 *1+2^1 *1 -1=2, and the subset {2} has binary index 2^2 *1 -1=3.
After the comments for every nonzero (a_ij) the location and  the binary index of the subset of processors to which it is assigned is mentioned.
So the file has the folowing form:

```
%%MatrixMarket matrix coordinate general integer
% Matrix partitioned into p="..." parts.
% Max. allowed partition size was: "..." 
% Optimal communication volume is: "..."
% Size of the different parts (using binary index part): "..."
m n nnz
i j BinIndex_subset
...
...
i j BinIndex_subset
```

Information about the partitioning:
In the file  p="#processors" "matrix".txt , where #processors and matrix are input variables information about the partitioning of the matrix is stored.
It stores the name of the martix, p, value of epsilon and maximum allowed part size. It stores which iterative upperbounds it has already tried and
it stores the improvements over the previous best solution that the algorithm finds (both the solution and the communication value are printed).
Before the improved solution is printed, it first prints the current number of aborted solutions.
It also prints the optimal partitioning and the communication volume and the time it took to find the optimal partitioning.


# For the user (on windows);

**Alt button**
	    Press the alt button to see which subtree the algorithm is currently in
	    When the Alt button is pressed the state of the first rowcol and the second rowcol that is
 	    partitioned is printed. This gives an indication how far the algorithm has progressed in the tree.


