// symm3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<vector>
#include<set>


int Processors = 4;
std::vector<bool> Zero_State(Processors, 0);
//Function && operator for two vector<bool>. 
//For instance if a=[0,1,0] and b=[1,1,0], then the function returns a && b =[0,1,0]
std::vector<bool> operator &&(std::vector<bool> State_nz, std::vector<bool> State_rowcol) {

    int p = State_nz.size();
    int lengthState_row = State_rowcol.size();

    if (p == lengthState_row) {

        std::vector<bool> New_state_nz(p, 0);

        for (int i = 0; i < p; i++) {

            //If in both vectors the ith entry is 1, then the ith entry of the New_State_nz wil also be 1.
            if (State_nz[i] == 1 && State_rowcol[i] == 1) {

                New_state_nz[i] = 1;
            }

            else {
                New_state_nz[i] = 0;
            }
        }

        return New_state_nz;
    }


    else {

        std::cout << "Error in using && operator when determining New_state of NZ";
        std::vector<bool> Notassigned = { 0, 0, 0, 0 };
        return Notassigned;
    }
}


//Function || operator for two vector<bool>. 
//For instance if a=[0,1,0] and b=[1,1,0], then the function returns a || b =[1,1,0]
std::vector<bool> operator ||(std::vector<bool> vector1, std::vector<bool> vector2) {

    int p = vector1.size();
    int lengthState_row = vector2.size();

    if (p == lengthState_row) {

        std::vector<bool> New_state_nz(p, 0);

        for (int i = 0; i < p; i++) {

            //If in one of the two vectors the ith entry is 1, 
            //then the ith entry of the New_State_nz wil also be 1.
            if (vector1[i] == 1 || vector2[i] == 1) {

                New_state_nz[i] = 1;
            }

            else {
                New_state_nz[i] = 0;
            }
        }

        return New_state_nz;
    }


    else {

        std::cout << "Error in using || operator for vectors";
        std::vector<bool> Notassigned = { 0, 0, 0, 0 };
        return Notassigned;
    }
}


//Function || operator for two vector<bool>. 
//For instance if a=[0,1,0] and b=[1,1,0], then the function returns a || b =[1,1,0]
std::vector<bool> XOR (std::vector<bool> vector1, std::vector<bool> vector2) {

    int p = vector1.size();
    int lengthState_row = vector2.size();

    if (p == lengthState_row) {

        std::vector<bool> New_state_nz(p, 0);

        for (int i = 0; i < p; i++) {

            //If in one of the two vectors the ith entry is 1, 
            //then the ith entry of the New_State_nz wil also be 1.
            if (vector1[i] != vector2[i] ) {

                New_state_nz[i] = 1;
            }

            else {
                New_state_nz[i] = 0;
            }
        }

        return New_state_nz;
    }


    else {

        std::cout << "Error in using || operator for vectors";
        std::vector<bool> Notassigned = { 0, 0, 0, 0 };
        return Notassigned;
    }

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

std::set<std::vector<bool>> Make_symm3_set(std::vector<std::vector<int>> Processor_sets, int current_set,std::set<std::vector<bool>> symm_set) {

    std::set<std::vector<bool>> Symm3_set;
    std::set<std::vector<bool>> Final_Symm3_set;
        

    

    if (current_set == Processors) {
        symm_set.erase(Zero_State);
        
        return symm_set;

    }

    else {
        std::vector<int> current_proc_set = Processor_sets[current_set];
        int no_proc_currentset = current_proc_set.size();

        for (int i = 0; i < Processors; i++) {

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

      Final_Symm3_set=  Make_symm3_set(Processor_sets, current_set + 1, Symm3_set);

     
    }
 return Final_Symm3_set;
}

void Symmetry3(std::vector<std::vector<bool>> Assigned_States) {

    //The 4 vectors over which we divide the processors.
    std::vector<int> First;
    std::vector<int> Second;
    std::vector<int> Third;
    std::vector<int> Fourth;

    //States assigned untill now
    std::vector<bool> State1=Assigned_States[0];
    std::vector<bool> State2=Assigned_States[1];

    //some state used to divide the processors
    std::vector<bool> State_intersect;
    std::vector<bool> state_xor;
    std::vector<bool> State_or;
    std::vector<bool> only_state1;
    std::vector<bool> only_state2;

    State_intersect = State1 && State2;
   First= Determine_Set_indices(State_intersect);

    State_or = State1 || State2;
    Second=Determine_indices_zeros(State_or);

    state_xor = XOR(State1, State2);
    only_state1 = state_xor && State1;
    only_state2 = state_xor && State2;

    Third = Determine_Set_indices(only_state1);
    Fourth = Determine_Set_indices(only_state2);


    if (!First.empty() && !Second.empty() && !Third.empty() && !Fourth.empty()) {

        std::cout << "geen symmetry weg te halen";
        //Hier moet all states inclusief all proc state uitgepoept worden
    }

    else {
        std::vector<std::vector<int>> Proc_sets = { First, Second, Third, Fourth };
        std::set < std::vector<bool>> start_symm = { Zero_State };
        std::set < std::vector<bool>>Symmetry_set= Make_symm3_set(Proc_sets, 0, start_symm);
     std::cout << " ";
    }

    
}




int main()
{
   

    std::vector<bool> v = { 0, 1, 1, 0 };
    std::vector<bool> w = { 1, 1, 1, 0 };
    std::vector<std::vector<bool>> g = { v,w };


    Symmetry3(g);

    std::vector<int> k = { 0,2 };
    std::vector<int> l = {  };
    std::vector<int>r = { 3 };
    std::vector<int> f = { 1 };
    std::set < std::vector<bool>> symmy = { Zero_State };
    std::vector<std::vector<int>> probeer = { l,k,f,r};
   // Make_symm3_set(probeer, 0, symmy);

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
