#include "./Util.h"
#include<algorithm>
#include<vector>
#include <math.h>
#include<set>
#include <iostream>
#include "./Global.h"
#include "./matrix.h"

//This function determines the index of a state by using binary arithmetric .
//So index of state (0,1,1) is 2^1+2^2 -1=4. The minus one at the end is in order to start at index 0.
int Binair_index(std::vector<bool> State) {
    int index = 0;

    for (int i = 0; i < Processors; i++) {

        index += pow(2, i) * State[i];

    }
    index -= 1;

    return index;
}


//This function detemrines the inititial upperbound used in the B &B tree
void init_UB(matrix *info) {

   UB= std::min(info->M, info->N)* (Processors - 1) + Processors - 1;
   
}

//Given p Processors this function determines the set of all possible states.
//Input; at the first step; start=0, Different_States=Zero_State, x= # Processors.
std::set<std::vector<bool>> States( int start, std::vector<bool> Different_States, int x) { 

    //Container that will hold all the states.
    std::set<std::vector<bool>> AllOptions;
    
    int entry = start;


    if (x == 0) {
        AllOptions.emplace(Different_States);

        //Pints the states when they are added to AllStates:
           /* for (int j = 0; j < Processors; j++) {
                std::cout << Different_States[j] << " ";
            }
            std::cout << "\n";*/
    }
    else {


        for (int k = 0; k < 2; k++) {

            Different_States[entry] = k;


            int b = x - 1;
            int entry = start + 1;

            std::set<std::vector<bool>> New_States = States( entry, Different_States, b);


            for (auto l = New_States.begin(); l != New_States.end(); l++) {

                AllOptions.emplace(*l);
            }
        }
    }

    //Erase the zero state (0,..0) and the all one state (1, ..,1) from the set.
    std::vector<bool> NoProcessor(Processors, 0);
    std::vector<bool> AllProcessors(Processors, 1);
    AllOptions.erase(NoProcessor);
    AllOptions.erase(AllProcessors);

    return AllOptions;
}

//Function that determines the maximum partition size 
int Load_Balance(int nnz) {
    int Max_part_size;
    Max_part_size = (1 + Epsilon) * ceil((float)nnz / (float)Processors); //gaat goed rond t naar beneden af ook als bv 4.8 dan max parititon size 4 moet k nog floor en dan int? ToDo

    if ((Processors * Max_part_size) < nnz) {

        std::cerr << "Something went wrong with determining the Maximum partition size" << "\n";
        return 0;
    }
    return Max_part_size;
}

//This function stores the status in a vector at the entry equal to the bianry index of that status.
std::vector <std::vector<bool>> indexStatus_vs_Status() {
    std::vector<std::vector<bool>> Index_Status;

    int x = pow(2, Processors) - 2;

    //Make sure that the vector has the correct size; 2^p-2.
    Index_Status.resize(x); 


    for (auto i = AllStates.begin(); i != AllStates.end(); i++) {
        int index = Binair_index(*i);

        Index_Status[index] = *i;

    }

    return Index_Status;
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

