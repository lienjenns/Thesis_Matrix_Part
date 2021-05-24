#pragma once
#include<vector>
#include<set>



//This function makes the set containing the states that must be taken into account when assigning the first rowcol a state.
//Using symmetry.
std::set<std::vector<bool>> First_Symmetry_Set();

//Function that determines the set of possible states for a rowcol, by comparing the set of feasible states
//with the states in the "symmetry set
std::set<std::vector<bool>> First_Symmetry_Check(std::set<std::vector<bool>> Symm_Set, std::set<std::vector<bool>>Feas_set);


//Function to make a priority queue for the state for the first rowcol
//(This is only used when activated, i.e. PQ=1)
//std::vector<std::vector<bool>> Priority_Queue(std::set<std::vector<bool>> Feasible_rowcol0, float The_ratio);


//The sanity check to check the partition sizes
//This check is used in the Update function.
//void Sanity_Check_PartitionSize(std::vector<int> Current_PArtition_sizes);