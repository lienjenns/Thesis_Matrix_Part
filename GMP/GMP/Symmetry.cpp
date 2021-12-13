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


