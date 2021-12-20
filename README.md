# Thesis_Matrix_Part

Two exact programs "GMP" and "ILP" that both solve the sparse matrix partitioning problem. 
That is  given a number of processors p, and a load-imbalance parameter epsilon, the algorithm determines
the optimal partitioning of a matrix A into p parts. 

The "GMP" program is based on the branch and bound method and uses the same approach as the exact matrix bipartitioners 
MondriaanOpt <sup>[1](#1)</sup> and  MatrixPartitioner MP  <sup>[2](#2)</sup>.

The "ILP" program is based on ILP.

The user inputs for both pograms are; 
The number of parts/processors p, the load-imbalance parameter epsilon,
and the matrix in Matrix Market format.




<a name="1">[1]</a>:[*An exact algorithm for sparse matrix bipartitioning*, Daniel M. Pelt and Rob H. Bisseling, *Journal of Parallel and Distributed Computing*, **85** (2015) pp. 79-90.](https://doi.org/10.1016/j.jpdc.2015.06.005)  

<a name="2">[2]</b>:[ *An improved exact algorithm and an NP-completeness proof for sparse matrix bipartitioning*,  Timon E. Knigge and Rob H. Bisseling, *Parallel Computing*, **96**  (2020) 102640](https://doi.org/10.1016/j.parco.2020.102640) 
