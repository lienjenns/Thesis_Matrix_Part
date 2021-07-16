// symmetry2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<vector>
#include<set>

int Processors = 2;
std::vector<bool> AllProc_State (Processors, 1);
std::vector<bool> Zero_State(Processors, 0);


//This function determines Indices of the ones in a vector of booleans
//So it determines the used processors.
std::vector<int> Determine_Set_indices(std::vector<bool> State_Intersect_RC) {

    //This vector stores the indices of the 1 elements in State_Intersect_RC.
    //So This vector gives the numbers of the processors that are used in the intersecting row/column.
    std::vector<int> Set_Indices;

    int Number_of_p = State_Intersect_RC.size();

    //Traverse all the elements of the state and find all the 1 entries.
    for (int i = 0; i < Number_of_p; i++) {

        int A = State_Intersect_RC[i];

        if (A == 1) {

            Set_Indices.push_back(i);
        }

        else {
        }
    }

    //Prints the indices of the ones.
   // for (int j = 0; j < Set_Indices.size(); j++) {
        // std::cout << Set_Indices[j];
   // }

    return Set_Indices;
}

//This fucntion determines and returns all the entries of the state that are 0.
//These entries coreespond with processors that are not used in this particular state.
std::vector<int> Determine_indices_zeros(std::vector<bool> State_Intersect_RC) {

    //This vector stores the indices of the  0 elements in State_Intersect_RC.
    //So this vector gives the numbers of the processors that are NOT used in the intersecting row/column.
    std::vector<int> Indices_zeros;

    int Number_of_p = State_Intersect_RC.size();

    //Traverse all the elements of the state and find all zero elements.
    for (int i = 0; i < Number_of_p; i++) {

        int A = State_Intersect_RC[i];

        if (A == 0) {

            Indices_zeros.push_back(i);
        }

        else {
        }
    }

    //Prints the indices of the zeros.
   /* for (int j = 0; j < Indices_zeros.size(); j++) {
        std::cout << Indices_zeros[j];
    }*/

    return Indices_zeros;
}

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
            if (j <= Used_proc.size() && ( (i - j) <= Unused_Proc.size())) {

                for (int l = 0; l <j; l++) {

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

int main()
{
    std::cout << "Hello World!\n";

    std::vector<bool> A = { 1,0};

    Symmetry2(A);

  
    
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
