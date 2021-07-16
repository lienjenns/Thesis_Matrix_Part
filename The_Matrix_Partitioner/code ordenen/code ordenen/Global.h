#pragma once
#include<set>
#include<algorithm>
#include<vector>
#include <math.h>
#include<string>
#include<fstream>

//This file contains # variables that need to be accessible everywhere.

//These variables are calculated in the main.cpp:


//Number of processors
extern int Processors;

//The load (in)balance paramter
extern double Epsilon;

//Set of all the possible states if there are p Processors, without the all one state (1, .., 1) and the all zero state (0,..,0).
//So the set has size 2^p - 2.
extern std::set<std::vector<bool>> AllStates;

//A vector with the states of AllStates, a state is element v_i of the vector if i is the binary index of this state.
//So for every binairy index the corresponding state is easy to find by using this vector.
extern std::vector <std::vector<bool>>Index_and_Status;

//The all zero state (0,..,0).
extern std::vector<bool> Zero_State; 
//The all one state (1,...,1).
extern std::vector<bool> AllProc_State;

//The maximum size of a partition.
//This depends on the number of nnz, Processors and Epsilon.
extern int Max_Partition_size;

//The Upperbound used in the branch and bound tree
extern int UB;



//This variable is watched in Possible_States.cpp:

//A boolean that starts at 0, and becomes one when the escape button is pushed.
//If Stop_partition=1 the execution of the tree stops and the best found solution until that moment is printed.
extern bool Stop_Partition;


//Name of output stream and the file to store all information about a matrix new upperbounds etc. in a file
extern std::ofstream Solution_and_info;
extern std::string filename_Sol_info;


//Some parameters to turn some features on and off.
extern bool PQ;
extern bool s2;
