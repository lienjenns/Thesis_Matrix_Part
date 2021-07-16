#pragma once
#include "./matrix.h"
#include<bitset>
#include<set>
#include "./Global.h"
#include<numeric>
#include"./Lower_bounds.h"




//This function determines the order in which the rows an columns are traversed in the tree,
//This order is absed on the number of nonzeros in a rowcol, no dstinction is made between rows and columns.
std::vector<int> Determine_Order_row_columns(std::vector<int> number_nz);

//A second fucntion to determine the order of row and columns.
//Again the orer is based on the number of nonzeros in a rowcol, but now rows and columns are alternated.
std::vector<int> Determine_Order_row_columns2(std::vector<int> number_nz, int M);

std::vector<int> Determine_Order_row_columns3(std::vector<int> number_nz, matrix A);

//Function that determines the possible states for a rowcol given the states of already assigned rowcols that intersect this rowcol in a nonzero.
std::set<std::vector<bool>> Assigned_States(std::vector<std::vector<bool>> Set_States_nz, std::vector<bool> Unassigned_State);

//This function determines for a rowcol, all possible states for this rowcol, based on already assigned nonzeros in this rowcol
//and based on the free nonzeros in this rowcol.
std::set<std::vector<bool>> Possible_States(std::vector<std::vector<bool>> States_NZs, int No_Free_NZ);

//Given a (partial) partition of a matrix, this fucntion determines the (explicit) communication volume of the (partial) partition.
int LowerBound1(std::vector < std::vector<bool>>The_Partition);


//This function makes the B&B tree, (DFS).
void Partition(std::vector<std::vector<bool>> The_States, std::vector<int> Order_rows_columns, int a, matrix * Info, std::vector<int> Vrije_NZ, std::vector<int> Partition_size, std::vector<std::vector<int>> color_count,
	int lowerbound, std::vector<std::pair<int, std::vector<bool>>>  Partial_Status_rowcols, std::array<std::vector<std::vector<int>>, 2> Packing_Sets, int LB3_oud, std::vector<int>Value_Partial_status, Bi_Graph bigraph);


//This fucntion gives the number of partial partitions that were aborted, beacause LB>=UB.
int get_Aantal();

//The two external variables to store the new better solution/upperbound.
//This variables stores the new better solution, ie. partitionend matrix.
extern std::vector<std::vector<bool>> Best_solution_sofar;
//This variable stores the corresponding communication volume, i.e. the (new) upperbound.
extern int Lowest_cv_sofar;


//This fucntion saves the best solution found sofar, i.e. it stores the (new) upperbound.
void Save_best_solution_sofar(std::vector<std::vector<bool>> The_States, int commvol);
