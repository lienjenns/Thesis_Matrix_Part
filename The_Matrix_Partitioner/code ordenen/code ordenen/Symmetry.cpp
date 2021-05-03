#include <iostream>
#include<vector>
#include<set>
#include <algorithm>   //Necessary for find() function
#include "./Global.h"
#include "./Util.h"


//This function makes the set containing the states that must be taken into account when assigning the first rowcol a state.
//The First symmetry step, only take into account p possible states for the first rowcol.
//If p=3, the function returns the set {100,110,111}.
std::set<std::vector<bool>> First_Symmetry_Set() {
    
    //Set that will hold all the states for the first rowcol.
    std::set<std::vector<bool>> States_1st_rowcol_symm;

    //Make the states
    for (int i = 0; i < Processors; i++) {

        std::vector<bool> state(Processors, 0);

        for (int j = 0; j <= i; j++) {

            state[j] = true;

        }

        States_1st_rowcol_symm.insert(state);

    }


    //code to print all the states in the set States_1st_rowcol_symm.
   /* for (auto k = States_1st_rowcol_symm.begin(); k != States_1st_rowcol_symm.end(); k++) {

        std::vector<bool> St = *k;
        for (int l = 0; l < Processors; l++) {

            std::cout << St[l];
        }
        std::cout << " ";
    }*/

    return States_1st_rowcol_symm;
}

//Function that determines the set of possible states for a rowcol, by comparing the set of feasible states
//with the states in the "symmetry set", only keeping the states in the feasible set that also appear in the symmetry set.
//Returns the set with the remaining feasible states for the rowcol.
std::set<std::vector<bool>> First_Symmetry_Check(std::set<std::vector<bool>> Symm_Set, std::set<std::vector<bool>>Feas_set) {

    //This set will contain all the remaining feasible states.
    std::set<std::vector<bool>> Feas_states_Symm;


    for ( auto i = Symm_Set.begin(); i != Symm_Set.end(); i++) {

        std::vector<bool> State = *i;
        std::set<std::vector<bool>>::iterator it;

        //Look if the state of the symmetry set also appears in the feasible set.
        it = Feas_set.find(State);

        //If the state appears n the feasible set, keep it.
        if (it != Feas_set.end()) {

            Feas_states_Symm.insert(State);

        }
    }

    return Feas_states_Symm;
}

//  
//std::vector<int>  Info_sanity_1() {
//
//std::vector<int> MaxPArt_size_Vector;
//  
//    int x = pow(2, Processors) - 2;
//    MaxPArt_size_Vector.resize(x);
//    for (int i=0; i < x; i++){
//
//       std::vector<int> set_indices= Determine_Set_indices(Index_and_Status[i]);
//       int no_ones = set_indices.size();
//
//       MaxPArt_size_Vector[i] = no_ones * Max_Partition_size;
//
//    }
//
//    return MaxPArt_size_Vector;
//}
//
//std::vector<int> MaxValue_PartSize_vector = Info_sanity_1();
//
////void Sanity_Check_PartitionSize(std::vector<int> Current_PArtition_sizes){
////
////    int options = Current_PArtition_sizes.size();
////
////    for (int i = 0; i < options; i++) {
////
////        if (Current_PArtition_sizes[i] > MaxValue_PartSize_vector[i]) {
////
////            throw("Error in determining the Partition size");
////        }
////
////    }
////
////}
//
//
//void Sanity_Check_PartitionSize(std::vector<int> Current_PArtition_sizes) {
//
//    int options = Current_PArtition_sizes.size();
//
//    for (int i = 0; i < options; i++) {
//
//        if (Current_PArtition_sizes[i] > MaxValue_PartSize_vector[i]) {
//
//            std::cerr << "Error in determining the Partition size of State: " << i;
//            break;
//        }
//
//    }

//}
