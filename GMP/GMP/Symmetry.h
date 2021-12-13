#pragma once
#include<vector>
#include<set>



//This function makes the set containing the states that must be taken into account when assigning the first rowcol a state.
//Using symmetry.
std::set<std::vector<bool>> First_Symmetry_Set();

//Function that determines the set of possible states for a rowcol, by comparing the set of feasible states
//with the states in the "symmetry set
std::set<std::vector<bool>> First_Symmetry_Check(std::set<std::vector<bool>> Symm_Set, std::set<std::vector<bool>>Feas_set);

//Function that determines the set of possible states for a rowcol, by comparing the set of feasible states
//with the states in the "symmetry set
std::set<std::vector<bool>> Second_Symmetry_Check(std::set < std::vector<bool>> Symm_Set2, std::set<std::vector<bool>>Feas_set);

//std::set<std::vector<bool>> Symmetry3(std::vector<std::vector<bool>> Assigned_States);



//Class that contains the information regarding to symmetry between assignments for the next rowcol in the B &B tree.
class Symmetry_processors
{

public:
    std::vector<std::vector<int>> Different_processor_sets;

    bool No_symmetry;

    Symmetry_processors();

    //Function that updates the different_processor_sets.
    void adjust_processor_sets(std::vector<bool> state);

    //Fuction that uses different_processor_sets, to make the symmetry set.
    //This symmetry set  is used to determine whcich of the feasible states for a rowcol needs to be traversed in the B & B tree.
    std::set<std::vector<bool>> make_symmetry_set(int current_set, std::set<std::vector<bool>> symm_set);
};

