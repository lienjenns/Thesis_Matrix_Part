#pragma once
#include<algorithm>
#include<vector>
#include<set>


int Binair_index(std::vector<bool> State);

std::vector<int> Determine_indices_zeros(std::vector<bool> State_Intersect_RC);

std::vector<int> Determine_Set_indices(std::vector<bool> State_Intersect_RC);

std::vector<bool> operator &&(std::vector<bool> State_nz, std::vector<bool> State_rowcol);

std::vector<bool> operator ||(std::vector<bool> vector1, std::vector<bool> vector2);