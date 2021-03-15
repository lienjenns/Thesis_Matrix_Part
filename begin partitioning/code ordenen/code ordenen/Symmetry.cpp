#include <iostream>
#include<vector>
#include<set>
#include "./Global.h"


//The First symmetry step, only take into account p possible states for the first rowcol.
std::set<std::vector<bool>> First_Symmetry_Set() {
                                 //ToDo in code hoeft dit niet geinitailaiseerd te worden
    std::set<std::vector<bool>> States_1st_rowcol_symm;


    for (int i = 0; i < Processors; i++) {
        std::vector<bool> state(Processors, 0);
        for (int j = 0; j <= i; j++) {

            state[j] = true;

        }

        States_1st_rowcol_symm.insert(state);

    }


    //Print de states waar je naar mag kijken voor de eerste rowcol.
    //Deze states zitten inn States_1st_rowcol_symm
   /* for (auto k = States_1st_rowcol_symm.begin(); k != States_1st_rowcol_symm.end(); k++) {

        std::vector<bool> St = *k;
        for (int l = 0; l < Processors; l++) {

            std::cout << St[l];
        }
        std::cout << " ";
    }*/

    return States_1st_rowcol_symm;
}


std::set<std::vector<bool>> First_Symmetry_Check(std::set<std::vector<bool>> Symm_Set, std::set<std::vector<bool>>Feas_set) {

    std::set<std::vector<bool>> Feas_states_Symm;

    for (auto i = Symm_Set.begin(); i != Symm_Set.end(); i++) {

        std::vector<bool> State = *i;
        std::set<std::vector<bool>>::iterator it;

        it = Feas_set.find(State);

        if (it != Feas_set.end()) {

            Feas_states_Symm.insert(State);

        }

    }
    std::cout << Feas_states_Symm.size();
    return Feas_states_Symm;
}