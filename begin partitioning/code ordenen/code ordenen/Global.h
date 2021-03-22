#pragma once
#include<set>
#include<algorithm>
#include<vector>
#include <math.h>
extern int Processors;

extern double Epsilon;

//Set of all possible states without the all one state (1, .., 1), for a rwocol if there are p Processors.
//So the vector has size 2^p - 2.
extern std::set<std::vector<bool>> AllStates;

//A vector with the states of AllStates, a state is element v_i of the vector if i is the binary index of this state.
//So for every binairy index the corresponding state is easy to find by using this vector.
extern std::vector <std::vector<bool>>Index_and_Status;

extern std::vector<bool> Zero_State; 
extern std::vector<bool> AllProc_State;

extern int Max_Partition_size;

extern bool Stop_Partition;

//Deze functie maakt alle states die mogelijk zijn voor p processoren 
//int x wil je weg maar moet je ook doorgebn in recursie zou je a=p kunnen zeggen end an gewoon a-1 door gven?

//int g = 0;

