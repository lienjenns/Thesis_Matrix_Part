#pragma once
#include<vector>
#include<set>



//This function makes the set containing the states that must be taken into account when assigning the first rowcol a state.
//Using symmetry.
std::set<std::vector<bool>> First_Symmetry_Set();

//Function that determines the set of possible states for a rowcol, by comparing the set of feasible states
//with the states in the "symmetry set
std::set<std::vector<bool>> First_Symmetry_Check(std::set<std::vector<bool>> Symm_Set, std::set<std::vector<bool>>Feas_set);

//Function that determines the symmetry set for the 2nd row/column based on the state assigned to the 1st rowcol.
//The output are the states that need to be traversed to miss no possible partitioning of the matrix.
std::set<std::vector<bool>> Symmetry2(std::vector<bool> previous_State);

//Function that determines the set of possible states for a rowcol, by comparing the set of feasible states
//with the states in the "symmetry set
std::set<std::vector<bool>> Second_Symmetry_Check(std::set < std::vector<bool>> Symm_Set2, std::set<std::vector<bool>>Feas_set);

std::set<std::vector<bool>> Symmetry3(std::vector<std::vector<bool>> Assigned_States);
//Function to make a priority queue for the state for the first rowcol
//(This is only used when activated, i.e. PQ=1)
std::vector<std::vector<bool>> Priority_Queue(std::set<std::vector<bool>> Feasible_rowcol0, float The_ratio);



class Symmetry_processors
{

public:
    std::vector<std::vector<int>> Different_processor_sets;

    bool No_symmetry;

    Symmetry_processors();
    void adjust_processor_sets(std::vector<bool> state);

    std::set<std::vector<bool>> make_symmetry_set(int current_set, std::set<std::vector<bool>> symm_set);


};


//The sanity check to check the partition sizes
//This check is used in the Update function.
//void Sanity_Check_PartitionSize(std::vector<int> Current_PArtition_sizes);