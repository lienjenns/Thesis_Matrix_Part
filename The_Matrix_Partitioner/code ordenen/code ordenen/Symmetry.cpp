#include <iostream>
#include<vector>
#include<set>
#include <algorithm>   //Necessary for find() function & set_intersection
#include "./Global.h"
#include "./Util.h"
#include "./matrix.h"
#include"./Symmetry.h"
#include<numeric>


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

////Function that determines the symmetry set for the 2nd row/column based on the state assigned to the 1st rowcol.
//std::set<std::vector<bool>> Symmetry2(std::vector<bool> previous_State) {
//
//    std::set<std::vector<bool>> Symmetry2_Set;
//
//    std::vector<int> Used_proc = Determine_Set_indices(previous_State);
//    std::vector<int> Unused_Proc = Determine_indices_zeros(previous_State);
//
//    //Make states with i ones.
//    for (int i = 1; i < Processors; i++) {
//        std::vector<bool> Symm2_state = Zero_State;
//        Symm2_state = Zero_State;
//
//
//        for (int j = 0; j <= i; j++) {
//            Symm2_state = Zero_State;
//            if (j <= Used_proc.size() && ((i - j) <= Unused_Proc.size())) {
//
//                for (int l = 0; l < j; l++) {
//
//                    Symm2_state[l] = 1;
//                    //  Symm2_state.assign(j, 1);
//                }
//
//                for (int k = 0; k < i - j; k++) {
//
//                    int index = Unused_Proc[k];
//
//                    Symm2_state[index] = 1;
//                }
//            }
//
//            else continue;
//
//            Symmetry2_Set.insert(Symm2_state);
//        }
//    }
//
//    //AllProc state always a possible state and there are no states and no circumstanves that any state is "symmetric"to this one.
//    Symmetry2_Set.insert(AllProc_State);
//
//    return Symmetry2_Set;
//}

//Function that determines the set of possible states for a rowcol, by comparing the set of feasible states
//with the states in the "symmetry set", only keeping the states in the feasible set that also appear in the symmetry set.
//Returns the set with the remaining feasible states for the rowcol.
std::set<std::vector<bool>> Second_Symmetry_Check(std::set < std::vector<bool>> Symm_Set2, std::set<std::vector<bool>>Feas_set) {

    //This set will contain all the remaining feasible states.
    std::set<std::vector<bool>> Feas_states_Symm;

    


    std::set_intersection(Symm_Set2.begin(), Symm_Set2.end(), Feas_set.begin(), Feas_set.end(), std::inserter(Feas_states_Symm, Feas_states_Symm.end()));
    
    return Feas_states_Symm;
}

//Constructs The symmetry processors class. we start with 1 vector in the different processor set, that contains all processors, i.e. {0, .., p-1}.
//And no_symmetry=0 meaning that there is still symmetry between assignments of the next rowcol.
Symmetry_processors::Symmetry_processors() {
    std::vector<int> initial_set;
    initial_set.resize(Processors);
    std::iota(initial_set.begin(), initial_set.end(), 0);

    Different_processor_sets.push_back(initial_set);
    No_symmetry = 0;
}

//This function determines the new different processor subsets from the old processor subsets. The processor sets can change beacause of the assigment of a state to a rowcol.
//e.g. if different_processor_sets was {{0},{1,2}} (no_symmetry=0), p=3 and new rowcol is assigned state {0,0,1}, then the new Different_processor_sets will be; {{0},{1},{2}},
// and now no_symmetry=1.
void Symmetry_processors::adjust_processor_sets(std::vector<bool> state) {

    std::vector<int> used_procs_state = Determine_Set_indices(state);
    std::sort(used_procs_state.begin(), used_procs_state.end());


    int no_sets = Different_processor_sets.size();

    //Traverse all processor subsets.
    for (int j = 0; j < no_sets; j++) {

        std::vector<int> Proc_setj = Different_processor_sets[j];
        //Containers to store the (possible) new subsets of processors.
        std::vector<int> new_Proc_Set;
        std::vector<int> new_Proc_Set2;

        //Sorting is neccesary in order to use set_interscetion and set_difference
        std::sort(Proc_setj.begin(), Proc_setj.end());

        //Determine processors that are used in "state" and are contained in this subset of processors.
        std::set_intersection(Proc_setj.begin(), Proc_setj.end(), used_procs_state.begin(), used_procs_state.end(), std::back_inserter(new_Proc_Set));

        if (!new_Proc_Set.empty()) {
            //Determine if there are processors contained in this subset of processors that are not used in "state".
            std::set_difference(Proc_setj.begin(), Proc_setj.end(), used_procs_state.begin(), used_procs_state.end(), std::back_inserter(new_Proc_Set2));
            Different_processor_sets[j] = new_Proc_Set;

            if (!new_Proc_Set2.empty()) {
                Different_processor_sets.push_back(new_Proc_Set2);

            }
        }
        else { continue; }

        //If there are p processor sets then there is no symmetry possible between assignments of the next rowcol.
        if (Different_processor_sets.size() == Processors) {
            No_symmetry = 1;
            break;
        }
    }
}

//This function makes the symmetry set based on the different processor sets.
std::set<std::vector<bool>> Symmetry_processors::make_symmetry_set(int current_set, std::set<std::vector<bool>> symm_set) {

    //Containers to store the symmetery set.
    std::set<std::vector<bool>> Symm3_set;
    std::set<std::vector<bool>> Final_Symm3_set;

    int no_processor_sets = Different_processor_sets.size();

    //If we already "traversed" every processor set, then symm set is our symmetry set.
    if (current_set == no_processor_sets) {
        symm_set.erase(Zero_State);

        return symm_set;
    }

    else {
        std::vector<int> current_proc_set = Different_processor_sets[current_set];
        int no_proc_currentset = current_proc_set.size();

        for (int i = 0; i <= Processors; i++) {

            if (i <= no_proc_currentset) {

                for (auto l = symm_set.begin(); l != symm_set.end(); l++) {
                    std::vector<bool> adjust_State = *l;

                    for (int j = 0; j < i; j++) {

                        int index = current_proc_set[j];
                        adjust_State[index] = 1;

                    }

                    Symm3_set.emplace(adjust_State);
                }
            }
        }

        Final_Symm3_set = make_symmetry_set(current_set + 1, Symm3_set);
    }
    return Final_Symm3_set;
}


//std::set<std::vector<bool>> Make_symm3_set(std::vector<std::vector<int>> Processor_sets, int current_set, std::set<std::vector<bool>> symm_set) {
//
//    std::set<std::vector<bool>> Symm3_set;
//    std::set<std::vector<bool>> Final_Symm3_set;
//
//
//
//
//    if (current_set == 4) {
//        symm_set.erase(Zero_State);
//
//        return symm_set;
//
//    }
//
//    else {
//        std::vector<int> current_proc_set = Processor_sets[current_set];
//        int no_proc_currentset = current_proc_set.size();
//
//        for (int i = 0; i <= Processors; i++) {
//
//            if (i <= no_proc_currentset) {
//
//                for (auto l = symm_set.begin(); l != symm_set.end(); l++) {
//                    std::vector<bool> adjust_State = *l;
//
//                    for (int j = 0; j < i; j++) {
//
//                        int index = current_proc_set[j];
//                        adjust_State[index] = 1;
//
//                    }
//
//                    Symm3_set.emplace(adjust_State);
//                }
//
//            }
//
//        }
//
//        Final_Symm3_set = Make_symm3_set(Processor_sets, current_set + 1, Symm3_set);
//
//
//    }
//    return Final_Symm3_set;
//}
//
//std::set<std::vector<bool>> Symmetry3(std::vector<std::vector<bool>> Assigned_States) {
//
//    //The 4 vectors over which we divide the processors.
//    std::vector<int> First;
//    std::vector<int> Second;
//    std::vector<int> Third;
//    std::vector<int> Fourth;
//
//    //States assigned untill now
//    std::vector<bool> State1 = Assigned_States[0];
//    std::vector<bool> State2 = Assigned_States[1];
//
//    //some state used to divide the processors
//    std::vector<bool> State_intersect;
//    std::vector<bool> state_xor;
//    std::vector<bool> State_or;
//    std::vector<bool> only_state1;
//    std::vector<bool> only_state2;
//
//    State_intersect = State1 && State2;
//    First = Determine_Set_indices(State_intersect);
//
//    State_or = State1 || State2;
//    Second = Determine_indices_zeros(State_or);
//
//    state_xor = XOR(State1, State2);
//    only_state1 = state_xor && State1;
//    only_state2 = state_xor && State2;
//
//    Third = Determine_Set_indices(only_state1);
//    Fourth = Determine_Set_indices(only_state2);
//
//
//    //if (!First.empty() && !Second.empty() && !Third.empty() && !Fourth.empty()) { //ToDo dit geldt alleen voor p=4, voor is 3 hoeven er maar 3 niet leeg te zijn vierde is altijd leeg
//
//    //    std::cout << "geen symmetry weg te halen";
//    //    //Hier moet all states inclusief all proc state uitgepoept worden
//    //}
//
//  //  else {
//    std::vector<std::vector<int>> Proc_sets = { First, Second, Third, Fourth };
//    std::set < std::vector<bool>> start_symm = { Zero_State };
//    std::set < std::vector<bool>>Symmetry_set = Make_symm3_set(Proc_sets, 0, start_symm);
//    //  std::cout << " ";
//    // }
//    return Symmetry_set;
//}


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

            Prior_Queue = { Proc4,Proc3, Proc2, Proc1 };

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
