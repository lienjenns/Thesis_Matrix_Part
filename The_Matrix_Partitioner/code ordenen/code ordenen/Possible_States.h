#pragma once
#include "./matrix.h"
#include<bitset>
#include<set>
#include "./Global.h"
#include<numeric>






//This function determines the order in which the rows an columns are traversed in the tree,
//This order is absed on the number of nonzeros in a rowcol, no dstinction is made between rows and columns.
std::vector<int> Determine_Order_row_columns(std::vector<int> number_nz);

//A second fucntion to determine the order of row and columns.
//Again the orer is based on the number of nonzeros in a rowcol, but now there is alternation between rows and columns.
std::vector<int> Determine_Order_row_columns2(std::vector<int> number_nz, int M);



std::set<std::vector<bool>> Assigned_States(std::vector<std::vector<bool>> Set_States_nz, std::vector<bool> Unassigned_State);


//This function makes the B&B tree
std::vector<std::vector<bool>> Partition(std::vector<std::vector<bool>> The_States, std::vector<int> Order_rows_columns, int a, matrix Info, std::vector<int> Vrije_NZ, std::vector<int> Partition_size, std::vector<std::vector<int>> color_count,
	int lowerbound, std::vector<std::pair<int, std::vector<bool>>>  Partial_Status_rowcols, std::array<std::vector<std::vector<int>>, 2> Packing_Sets, int LB3_oud);
std::set<std::vector<bool>> Possible_States(std::vector<std::vector<bool>> States_NZs, int No_Free_NZ);


//ToDo gebruikte eerst deze functie nu zit deze in update functie.
//std::vector<int> Update_NO_Freenz(std::vector<int> No_Freenz_perRowCol, std::vector<int> Indices_intersectRowCol);


int LowerBound1(std::vector < std::vector<bool>>The_Partition);
int LB1(std::vector<bool>State_rowcol);



int Load_Balance(int nnz);


//This fucntion gives the number of partial partitions that wre aborted, beacause LB>=UB.
int get_Aantal();

//The two external variables to store the new better solution/upperbound.
//This variables stores the new better solution, ie. partitionend matrix
extern std::vector<std::vector<bool>> Best_solution_sofar;
//This variable stores the corresponding communication volume, i.e. the (new) upperbound.
extern int Lowest_cv_sofar;


//This fucntion saves the best solution found ssofar, i.e. it stores the (new) upperbound.
void Save_best_solution_sofar(std::vector<std::vector<bool>> The_States, int commvol);
