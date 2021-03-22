#include "./Util.h"
#include<algorithm>
#include<vector>
#include <math.h>
#include<set>
#include <iostream>
#include "./Global.h"

//This function determines the index of the state by using binary arithmetric .
//So index of state (0,1,1) is 2^1+2^2 -1=4. The minus one is in order to start at index 0.
int Binair_index(std::vector<bool> State) {
    int index = 0;

    for (int i = 0; i < Processors; i++) {

        index += pow(2, i) * State[i];

    }
    index -= 1;

    return index;
}

std::vector<int> Determine_indices_zeros(std::vector<bool> State_Intersect_RC) {

    //This vector stores the indices of the  0 elements in State_Intersect_RC.
    //So this vector gives the numbers of the processors that are NOT used in the intersecting row/column.

    std::vector<int> Indices_zeros;
    int Number_of_p = State_Intersect_RC.size();

    //The for loop finds all indices of the zeros and stores it in a vector.
    for (int i = 0; i < Number_of_p; i++) {

        //std::cout << State_Intersect_RC[i];
        int A = State_Intersect_RC[i];
        if (A == 0) {

            Indices_zeros.push_back(i);
        }

        else {
        }
    }

    //Prints the indices of the zeros.
    for (int j = 0; j < Indices_zeros.size(); j++) {
        //std::cout << Indices_zeros[j];
    }

    return Indices_zeros;

}


//Function && operator for two vector<bool>. 
//For instance if a=[0,1,0] and b=[1,1,0], then the function returns a $$ b =[0,1,0]
std::vector<bool> operator &&(std::vector<bool> State_nz, std::vector<bool> State_rowcol) {

    int p = State_nz.size();
    int lengthState_row = State_rowcol.size();

    if (p == lengthState_row) {


        std::vector<bool> New_state_nz(p, 0);

        for (int i = 0; i < p; i++) {

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

        std::cout << "Error in determining New_state of NZ";
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

        std::cout << "Error in determining New_state of NZ";
        std::vector<bool> Notassigned = { 0, 0, 0, 0 };
        return Notassigned;
    }
}

//Function determines Indices of the ones in a vector of booleans
//So it determines the used processors.
std::vector<int> Determine_Set_indices(std::vector<bool> State_Intersect_RC) {

    //This vector stores the indices of the 1 elements in State_Intersect_RC.
    //So This vector gives the numbers of the processors that are used in the intersecting row/column.

    std::vector<int> Set_Indices;
    int Number_of_p = State_Intersect_RC.size();

    //The for loop finds all indices of the ones and stores it in a vector.
    for (int i = 0; i < Number_of_p; i++) {

        //std::cout << State_Intersect_RC[i];
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