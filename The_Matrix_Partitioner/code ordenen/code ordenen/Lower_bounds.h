#pragma once
#include<vector>
#include<set>




//This function determiens the Lowerbound1 for an assigned rowcol,
//i.e. it determiens the number of cuts in the assigned rowcol.
int LB1(std::vector<bool>State_rowcol);

//This fucntion determines the L2 bound for a rowcol, that is the number of implicit cuts in the rowcol. 
std::vector<std::pair<int, std::vector<bool>>> L2bound(std::vector<bool> Status_intersect_rc, int rc, std::vector<std::pair<int, std::vector<bool>>> Partial_Status_rowcols);

//Function that makes the info for the most simple version of the l3bound.
//Only look at rowcols that are partialy coloured in one colour.
//This function determines & saves the binary indices of the states with only 1 processor, in the external variable Info_L3.
void Basic_L3_info();

// Vector with binary indices of the states with only 1 processor.
//This vector is used in the function L3bound.
extern std::vector<int> Info_L3;

//For now this function is the most basic of possible local L3 bound, only looks at rowcols
//that are partial one color, consequently it  only takes into account partition sizes of  single processors.
//This function calculates the "local" L3 bound = local packing bound.
int L3bound(std::array<std::vector<std::vector<int>>, 2> Packing_Sets, std::vector<int> Partition_Size);