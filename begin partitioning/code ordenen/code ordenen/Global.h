#pragma once
#include<set>
#include<algorithm>
#include<vector>
#include <math.h>
extern int Processors;

extern double Epsilon;

extern std::set<std::vector<bool>> AllStates;

extern std::vector<bool> Zero_State; 
extern std::vector<bool> AllProc_State;

extern int Max_Partition_size;

extern bool Stop_Partition;

//Deze functie maakt alle states die mogelijk zijn voor p processoren 
//int x wil je weg maar moet je ook doorgebn in recursie zou je a=p kunnen zeggen end an gewoon a-1 door gven?

//int g = 0;

