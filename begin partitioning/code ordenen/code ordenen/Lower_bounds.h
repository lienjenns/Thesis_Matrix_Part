#pragma once
#include "./Possible_States.h"
#include<algorithm>
#include<vector>
#include <math.h>
#include "./matrix.h"
#include "./Global.h"
#include<set>
#include "./Util.h"


std::vector<std::pair<int, std::vector<bool>>> L2bound(std::vector<bool> Status_intersect_rc, int rc, std::vector<std::pair<int, std::vector<bool>>> Partial_Status_rowcols);