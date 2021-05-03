// symmetry.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include<vector>
#include<set>



std::set<std::vector<bool>> First_Symmetry_Set() {
    int Processors = 4;                                 //ToDo in code hoeft dit niet geinitailaiseerd te worden
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

        it=Feas_set.find(State);

        if (it != Feas_set.end()) {

            Feas_states_Symm.insert(State);

        }

      


    }
    std::cout << Feas_states_Symm.size();
    return Feas_states_Symm;
}

int main()
{
    First_Symmetry_Set();
   
    std::vector<bool> F = { 1,1,0,0 };
    std::vector<bool> A = { 0,1,0,0 };
    std::vector<bool> B = {0,1,1,0};
    std::vector<bool> C = { 1,0,0,0 };
    std::vector<bool> D = { 1,1,1,1 };
    std::vector<bool> E = { 1,1,1,0 };

    std::set<std::vector<bool>> The_Set = { A,B,C,D,E,F };
    First_Symmetry_Check(First_Symmetry_Set(), The_Set);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
