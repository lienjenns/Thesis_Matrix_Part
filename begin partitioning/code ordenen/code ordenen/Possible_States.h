#pragma once
#include "./matrix.h"
#include<bitset>
#include<set>
#include "./Global.h"
#include<numeric>


//int AAnt = 0;

//Function that makes the Unassigned State
std::vector<bool> Determine_Unassigned_State(int p);



std::vector<int> Determine_Order_row_columns(std::vector<int> number_nz);

std::vector<std::vector<bool>> Partition(std::vector<std::vector<bool>> The_States, std::vector<int> Order_rows_columns, int a, matrix Info, std::vector<int> Vrije_NZ, std::vector<int> Partition_size, std::vector<std::vector<int>> color_count,
	int lowerbound);
std::set<std::vector<bool>> Possible_States(std::vector<std::vector<bool>> States_NZs, int No_Free_NZ);



std::vector<int> Update_NO_Freenz(std::vector<int> No_Freenz_perRowCol, std::vector<int> Indices_intersectRowCol);


int LowerBound1(std::vector < std::vector<bool>>The_Partition);
int LB1(std::vector<bool>State_rowcol);


int get_Aantal();

int Load_Balance(int nnz);

//void ub(matrix A);


void Save_best_solution_sofar(std::vector<std::vector<bool>> The_States, int commvol);



extern std::vector<std::vector<bool>> Best_solution_sofar;
extern int Lowest_cv_sofar;









//Bijhoduen partitie grote of eigenlijk de verschillende kleuren per rij en kolom

//int options = pow(2, Processors) - 2;

//Nu nog 1 supercontainer

//std::vector<int> Partition_size(options, 0);





//std::vector<std::vector<int>> color_count;

//Hier wordt color_count gemaakt ToDo moet netter cpp




//struct Possible_States {
//
//
//public:
////Gives the status of the nonzero, for instance if p=3 "110" means this nonzero is assigned to processor 2 or 1.
//std::bitset<3> Status_nz; 
//
////All possible states for a row and column, described by a vector of bitsets;
//
//std::vector<std::bitset<3>> All_States;
//
//
//
//};