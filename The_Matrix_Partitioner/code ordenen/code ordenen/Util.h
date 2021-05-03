#pragma once
#include<algorithm>
#include<vector>
#include<set>



//This function determines the binary index of a state.
int Binair_index(std::vector<bool> State);

//Function that returns the indices with an zero in the a state.
std::vector<int> Determine_indices_zeros(std::vector<bool> State_Intersect_RC);

//Function that returns the indices of the one elements in a state.
std::vector<int> Determine_Set_indices(std::vector<bool> State_Intersect_RC);

// Function && operator for two vector<bool>.
std::vector<bool> operator &&(std::vector<bool> State_nz, std::vector<bool> State_rowcol);

//Function || operator for two vector<bool>. 
std::vector<bool> operator ||(std::vector<bool> vector1, std::vector<bool> vector2);

//Function that determines the max partition size
int Load_Balance(int nnz);

//This function determines all the possible states when there are p processors, this set is without the all one state (1, ..1).
std::set<std::vector<bool>> States( int start, std::vector<bool> Different_States, int x);

//This function links the binary index of a status from the set AllStates with the status.
std::vector <std::vector<bool>> indexStatus_vs_Status();

