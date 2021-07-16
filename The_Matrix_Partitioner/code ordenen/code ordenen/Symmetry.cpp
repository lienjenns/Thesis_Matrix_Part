#include <iostream>
#include<vector>
#include<set>
#include <algorithm>   //Necessary for find() function & set_intersection
#include "./Global.h"
#include "./Util.h"
#include "./matrix.h"


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

//Function that determines the symmetry set for the 2nd row/column based on the state assigned to the 1st rowcol.
std::set<std::vector<bool>> Symmetry2(std::vector<bool> previous_State) {

    std::set<std::vector<bool>> Symmetry2_Set;

    std::vector<int> Used_proc = Determine_Set_indices(previous_State);
    std::vector<int> Unused_Proc = Determine_indices_zeros(previous_State);

    //Make states with i ones.
    for (int i = 1; i < Processors; i++) {
        std::vector<bool> Symm2_state = Zero_State;
        Symm2_state = Zero_State;


        for (int j = 0; j <= i; j++) {
            Symm2_state = Zero_State;
            if (j <= Used_proc.size() && ((i - j) <= Unused_Proc.size())) {

                for (int l = 0; l < j; l++) {

                    Symm2_state[l] = 1;
                    //  Symm2_state.assign(j, 1);
                }

                for (int k = 0; k < i - j; k++) {

                    int index = Unused_Proc[k];

                    Symm2_state[index] = 1;
                }
            }

            else continue;

            Symmetry2_Set.insert(Symm2_state);
        }
    }

    //AllProc state always a possible state and there are no states and no circumstanves that any state is "symmetric"to this one.
    Symmetry2_Set.insert(AllProc_State);

    return Symmetry2_Set;
}

//Function that determines the set of possible states for a rowcol, by comparing the set of feasible states
//with the states in the "symmetry set", only keeping the states in the feasible set that also appear in the symmetry set.
//Returns the set with the remaining feasible states for the rowcol.
std::set<std::vector<bool>> Second_Symmetry_Check(std::set < std::vector<bool>> Symm_Set2, std::set<std::vector<bool>>Feas_set) {

    //This set will contain all the remaining feasible states.
    std::set<std::vector<bool>> Feas_states_Symm;

    std::set<std::vector<bool>>::iterator it;


    std::set_intersection(Symm_Set2.begin(), Symm_Set2.end(), Feas_set.begin(), Feas_set.end(), std::inserter(Feas_states_Symm, Feas_states_Symm.end()));
    
    return Feas_states_Symm;
}


std::vector<std::vector<bool>> Priority_Queue(std::set<std::vector<bool>> Feasible_rowcol0, float The_ratio) {

    //The vector with the states for the first rowcol in order of priotity. 
    std::vector<std::vector<bool>> Feas_and_Prior;

    // This is the  priority queue for the different situations
    std::vector<std::vector<bool>> Prior_Queue;

    if (Processors == 3) {

        std::vector<bool> Proc1{1,0,0 };
        std::vector<bool> Proc2{ 1,1,0 };
        std::vector<bool> Proc3{ 1,1,1 };

       

        if (The_ratio > 0.5 && The_ratio <= 1.0) {

            Prior_Queue={ Proc2,Proc1, Proc3 };

            for (int i = 0; i < Prior_Queue.size(); i++) {

                std::set<std::vector<bool>>::iterator it;

                //Look if the state of the in the priority queue also appears in the feasible set.
                it = Feasible_rowcol0.find(Prior_Queue[i]);


                //If the state appears in the feasible set, push it back.
                if (it != Feasible_rowcol0.end()) {     //To do deze controle verbodig ?

                    Feas_and_Prior.push_back(Prior_Queue[i]);

                }
            }
        }

        else if (The_ratio > 1) {

            Prior_Queue = { Proc2, Proc3 };
            for (int i = 0; i < Prior_Queue.size(); i++) {

                std::set<std::vector<bool>>::iterator it;

                //Look if the state of the in the priority queue also appears in the feasible set.
                it = Feasible_rowcol0.find(Prior_Queue[i]);


                //If the state appears in the feasible set, push it back.
                if (it != Feasible_rowcol0.end()) {     //To do deze controle verbodig ?

                    Feas_and_Prior.push_back(Prior_Queue[i]);

                }
            }
        }

        else {

            Prior_Queue = { Proc1,Proc2, Proc3 };

            for (int i = 0; i < Prior_Queue.size(); i++) {

                std::set<std::vector<bool>>::iterator it;

                //Look if the state of the in the priority queue also appears in the feasible set.
                it = Feasible_rowcol0.find(Prior_Queue[i]);


                //If the state appears in the feasible set, push it back.
                if (it != Feasible_rowcol0.end()) {     //To do deze controle verbodig ?

                    Feas_and_Prior.push_back(Prior_Queue[i]);

                }
            }
        }
        return Feas_and_Prior;


    }

    else {

        std::vector<bool> Proc1{ 1,0,0,0 };
        std::vector<bool> Proc2{ 1,1,0 ,0};
        std::vector<bool> Proc3{ 1,1,1 ,0};
        std::vector<bool> Proc4{ 1,1,1 ,1 };


        if (The_ratio > 0.35 && The_ratio <= 1.0) {

            Prior_Queue = { Proc2,Proc1, Proc3, Proc4 };

            for (int i = 0; i < Prior_Queue.size(); i++) {

                std::set<std::vector<bool>>::iterator it;

                //Look if the state of the in the priority queue also appears in the feasible set.
                it = Feasible_rowcol0.find(Prior_Queue[i]);


                //If the state appears in the feasible set, push it back.
                if (it != Feasible_rowcol0.end()) {     //To do deze controle verbodig ?

                    Feas_and_Prior.push_back(Prior_Queue[i]);

                }
            }
        }

        else if (The_ratio > 1) {

            Prior_Queue = { Proc2, Proc3, Proc4 };
            for (int i = 0; i < Prior_Queue.size(); i++) {

                std::set<std::vector<bool>>::iterator it;

                //Look if the state of the in the priority queue also appears in the feasible set.
                it = Feasible_rowcol0.find(Prior_Queue[i]);


                //If the state appears in the feasible set, push it back.
                if (it != Feasible_rowcol0.end()) {     //To do deze controle verbodig ?

                    Feas_and_Prior.push_back(Prior_Queue[i]);

                }
            }
        }

        else {

            Prior_Queue = { Proc1,Proc2, Proc3, Proc4 };

            for (int i = 0; i < Prior_Queue.size(); i++) {

                std::set<std::vector<bool>>::iterator it;

                //Look if the state of the in the priority queue also appears in the feasible set.
                it = Feasible_rowcol0.find(Prior_Queue[i]);


                //If the state appears in the feasible set, push it back.
                if (it != Feasible_rowcol0.end()) {     //To do deze controle verbodig ?

                    Feas_and_Prior.push_back(Prior_Queue[i]);

                }
            }
        }
        return Feas_and_Prior;
    }

}


// //This function makes the information needed for the sanity check of the (just calculated)
////Partition sizes. 
////Returns a vector with at index i the maximum allowed size of partition i.
//std::vector<int>  Info_sanity_1() {
//
//    std::vector<int> MaxPArt_size_Vector;
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
//void Sanity_Check_PartitionSize(std::vector<int> Current_Partition_sizes) {
//
//    int options = Current_Partition_sizes.size();
//
//    for (int i = 0; i < options; i++) {
//
//        if (Current_Partition_sizes[i] > MaxValue_PartSize_vector[i]) {
//
//            std::cerr << "Error in determining the Partition size of State: " << i;
//            break;
//        }
//
//    }
//
//}
//
//void Sanity_Check_usefullness(matrix Inform) {
//
//    if(Inform.nnz < Processors) {
//
//        std::cerr << "Not very Usefull to Partition this matrix in "<< Processors<< " processors.  (matrix contains less nonzeros than no. of Processors)";
//
//    }
//
//
//
//}
