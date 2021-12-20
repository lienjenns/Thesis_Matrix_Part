#  ILP

Given a number of processors p, and a load-imbalance parameter epsilon, the algorithm determines
the optimal partition of a matrix A into p parts. This algorithm uses ILP to solve the sparse matrix partitioning problem. 
The ILP model is implemented using IBM ILOG CPLEX 20.1.0 <sup>[1](#1)</sup>.
(For the parameter settings of cplex we adjusted the number of threads cplex may use to 1.   See main.cpp r.185)



The number of parts/processors p, the load-imbalance parameter epsilon,
and the matrix in Matrix Market format are all user inputs.


# Use:
To use this program you need to install IBM ILOG CPLEX , see  <sup>[1](#1)</sup>. 
Build with micrososft visual studio in release mode(x64), the executable can be found in the x64 directory.


## Run:

```
 .\ILP "name_matrix" "location_matrix" "p" "epsilon"
```

For example:

```
.\ILP "b1_ss" "matrix/b1_ss.mtx" "4" "0.03"
```

This command will partition the matrix "b1_ss" , which can be found in the directory "matrix", into 4 parts. 
The load imbalance parameter epsilon is set to 0.03.

## Output:

The output of the program cplex is showed in the terminal. 
After the ouput of the cplex proram we print the optimal comunication volume and the status of this value to check wheter or not it is optimal.
(It should be optimal.)


# Output file:

The partitioned matrix in matrix market format is given in the file p="#processors" "matrix", OUT.txt, where #processors and matrix are input variables.
In the comments of this file the value of p, the maximum allowed size of a part, the found optimal communication volume and the status of the found communciation volume are mentioned.
If CPLEX determined the optimal communciation volume the status should be "Optimal".
Additionally, for each processor the number of nonzeros owned by that processor are mentioned in order of the processors.
For example, if p=3, and the file states "Size of different parts: 3 5 4 ", it means that processor 0 owns 3 nonzeros, processor 1 owns 5 nonzeros and processor 2 owns 4 nonzeros.
After the comments for every nonzero (a_ij), the location and the processor to which it is assigned is mentioned.
So the file has the folowing form:

```
%%MatrixMarket matrix coordinate general integer
% Matrix partitioned into p="..." parts.
% Max. allowed partition size was: "..." 
% Optimal communication volume is: "..."  with status " ..." .
% Size of the different parts: "..."
m n nnz
i j Proc.
...
...
i j Proc.
```
<a name="1">[1]</a>:[ https://www.ibm.com/products/ilog-cplex-optimization-studio](https://www.ibm.com/products/ilog-cplex-optimization-studio)
