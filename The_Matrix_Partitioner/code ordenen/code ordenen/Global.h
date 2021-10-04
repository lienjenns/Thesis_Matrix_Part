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

extern std::set<int> Indices_2proc_states;

//The all zero state (0,..,0).
extern std::vector<bool> Zero_State; 
//The all one state (1,...,1).
extern std::vector<bool> AllProc_State;

//The maximum size of a partition.
//This depends on the number of nnz, Processors and Epsilon.
extern int Max_Partition_size;

//The Upperbound used in the branch and bound tree
extern int UB;

//The overall Lowerbound used in the branch and bound tree.
//If a solution is found equal to this lowerbound we know it must be optimal.
extern int Overall_LB;

//Maximum length of path in bfs in Global L4 bound.
extern int length_path;

//This variable is watched in Possible_States.cpp:

//A boolean that starts at 0, and becomes one when the escape button is pushed.
//If Stop_partition=1 the execution of the tree stops and the best found solution until that moment is printed.
extern bool Stop_Partition;


//Name of output stream and the file to store all information about a matrix new upperbounds etc. in a file
extern std::ofstream Solution_and_info;
extern std::string filename_Sol_info;

extern std::string nameMatrix;


extern int no_tobeAssigned;


//Some parameters to turn some features on and off.
extern bool PQ;
extern bool s2;
extern bool s3;
extern bool GL4_on;
extern bool GL3_on;
extern bool CombL3_L4;
extern bool Overall_LB_on;
extern bool Iterative_UB;

extern int aantalGL4;
extern int combolocal;
extern int L4groter;
extern int GL4groter;
extern int L3gr;
extern int gelijk;
extern int GL42;

extern int Check;
