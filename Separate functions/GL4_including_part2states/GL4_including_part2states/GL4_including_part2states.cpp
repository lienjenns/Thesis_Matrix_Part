// GL4_including_part2states.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<vector>
#include<array>
#include<algorithm>
#include<numeric>
extern std::vector <std::vector<bool>>Index_and_Status;
int Processors = 3;
//Zero_State = std::vector<bool>(Processors, 0);

//void GL4_verder(std::vector<int> Partial_Status, int M, int N, matrix* A, std::array<std::vector<std::vector<int>>, 2> Packing_Sets, std::vector<int> Partition_Size) {
//    int v_i, v_k;
//   int  binair_index_vi = -Partial_Status[v_i];
//   int binair_index_vk = -Partial_Status[v_k];
//   std::vector<bool> Part_state_vi = Index_and_Status[binair_index_vi];
//   std::vector<bool> Part_state_vk = Index_and_Status[binair_index_vk];
//   std::vector<int> procs_vi = Determine_Set_indices(Part_state_vi);
//  std::vector<int> procs_vk = Determine_Set_indices(Part_state_vk);
//
//  int match_sum_vi, match_sum_vk;
//  for (auto i = procs_vi.begin(); i != procs_vi.end(); i++) {
//
//      if (maintain_color_match[v_k][*i] == 1) {
//
//          match_sum_vk += 1;
//      }
//
//  }
//
//  for (auto i = procs_vk.begin(); i != procs_vk.end(); i++) {
//
//      if (maintain_color_match[v_i][*i] == 1) {
//
//          match_sum_vi += 1;
//      }
//
//  }
//
//    if (Partial_Status[v_k] == -3 || Partial_Status[v_k] == -7 || (Part_state_vi && Part_state_vk) != Zero_State || visited[v_k] == 1 || match_sum_vi !=0 || match_sum_vk !=0) {
//        continue;
//    }
//
//
//
//    else {
//        //If v_k is partially assigned to a processor/color with which vertex v_i has not already a match, MATCH!
//        if (Partial_Status[v_k] != -1) {
//
//            Matched[v_i] = 1;
//            Matched[v_k] = 1;
//
//            for (auto i = procs_vi.begin(); i != procs_vi.end(); i++) {
//
//                maintain_color_match[v_k][*i] = 1;
//            }
//
//            for (auto i = procs_vk.begin(); i != procs_vk.end(); i++) {
//
//                maintain_color_match[v_i][*i] == 1;
//            }
//          
//            no_matches_vi += procs_vk.size();
//            new_matches_vi += 1;
//            parent[v_k] = v_j;
//            Succesfull_Child[v_j] = 1;
//            int v_l = v_j;
//
//            while (v_l != v_i) {
//
//                v_l = parent[v_l];
//                Succesfull_Child[v_l] = 1;
//            }
//        }
//
//        //Else vertex v_k is unassigned.
//        else {
//
//            queue.push_back(v_k);
//            visited[v_k] = 1;
//            parent[v_k] = v_j;
//            Newly_visited.push_back(v_k);
//            x_new += 1;
//        }
//    }
//
//    if (no_matches_vi == max_matches) {
//        break;
//    }
//}
//
//if (y == x) {
//    level += 1;
//    x = x_new;
//    x_new = 0;
//    y = 0;
//}
//
//if (no_matches_vi == max_matches) {
//    break;
//}
//        }
//        //Before we will branch from the next v_i, free all unassigned vertices that were visited, but are not on a "matching" path.
//        for (auto i = Newly_visited.begin(); i != Newly_visited.end(); i++) {
//
//            if (Succesfull_Child[*i] != 1) {
//                visited[*i] = 0;
//            }
//        }
//        //Add the number of new matches of v_i to the GL4 bound.
//        GL4_bound += new_matches_vi;
//    }


bool Sort_by_negative(int a, int b)
{
    return ((a>=0  && b< 0));
}


int main()
{
   
    std::vector<int> v2 ;
    std::vector<int> intersect;
    //std::set_intersection(v.begin(), v.end(), v2.begin(), v2.end(), std::inserter(intersect, intersect.end()) );

    std::cout << intersect.size();

   // std::sort(vl.begin(), vl.end(), Sort_by_negative);

    std::cout << "Hello World!\n";



    //Assume A is a given vector with N elements
    
   
std::vector<int> vl = { 0,  -2,-1 , 2};
    int N = 4;
    std::vector<int> V(N);
    std::iota(V.begin(), V.end(), 0); //Initializing
    std::sort(V.begin(), V.end(), [&vl](int i, int j) {return ((vl[i] >= 0) && (vl[j] < 0)); });
 

    for (int k = 0; k < 4; k++) {
        std::cout << " " << V[k];
    }
    std::vector<int> procs_vk = { 1,2 , 8,9, 10, };
    std::vector<int> procs_vi = { 0,2,3,8 };
    std::vector<int> intersect_procs;

    std::set_difference(procs_vi.begin(), procs_vi.end(), procs_vk.begin(), procs_vk.end(), std::back_inserter(intersect_procs));
    std::cout << "k";
}
//return ( (vl[i] >= 0) && (vl[j]<0)); });
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
