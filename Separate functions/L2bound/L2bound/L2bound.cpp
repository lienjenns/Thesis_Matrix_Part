// L2bound.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<algorithm>
#include<vector>
#include<set>
#include <utility> 

int Processors = 3;

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

//Dit kan r dadelijk uit nu nodig om allstates heir te maken ToDo
std::set<std::vector<bool>> States(int Number_of_processors, int start, std::vector<bool> Different_States, int x) { //ToDo moet netter uiteindelik in cpp

    std::set<std::vector<bool>> AllOptions;

    int a = x;
    int p = Number_of_processors;
    int entry = start;


    if (a == 0) {
        AllOptions.emplace(Different_States);
        //Pints the states when they are added to AllStates:
            /*std::cout << g << ":   " << "\n";
            for (int j = 0; j < p; j++) {
                std::cout << Different_States[j] << " ";
            }
            std::cout << "\n";
            g++;*/
    }
    else {


        for (int k = 0; k < 2; k++) {

            Different_States[entry] = k;


            int b = a - 1;
            int entry = start + 1;

            std::set<std::vector<bool>> New_States = States(p, entry, Different_States, b);


            for (auto l = New_States.begin(); l != New_States.end(); l++) {

                AllOptions.emplace(*l);
            }

        }


    }

    std::vector<bool> NoProcessor(Processors, 0);
    std::vector<bool> AllProcessors(Processors, 1);
    AllOptions.erase(NoProcessor);
    AllOptions.erase(AllProcessors);

    return AllOptions;
}
// Tot hier eruit ToDo


//Deze aan begin maken zodat algemeen toegankelijk, dus in global.h.
//Zodat voor elke index status van index makkelijk te vinden en dit is dan 1 keer bepaald aan begin tree en hoeft niet elke keer opnieuw bepaald te worden.
std::vector <std:: vector<bool>> indexStatus_vs_Status() {
    std::vector<std::vector<bool>> Index_and_Status;
    Index_and_Status.resize(6); //Moet naar options resize dus 2^p-2
    std::set<std::vector<bool>> AllStates = States(Processors, 0, { 0,0,0 }, Processors);

    for (auto i = AllStates.begin(); i != AllStates.end(); i++) {
        int index = Binair_index(*i);

        Index_and_Status[index]=*i;

    }



    return Index_and_Status;

}

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


//std::vector<bool> State1, std::vector<bool> State2
std::set<std::vector<bool>> Assigned_States(std::vector<std::vector<bool>> Set_States_nz, std::vector<bool> Unassigned_State) {

    //This will be our container with all correct states for row/column, 
    //if we only look at the already assigned nz in a row/column. 
    std::set<std::vector<bool>> AllPossible_States_assignedNZ;


    //std::set<std::vector<bool>> AllPossibilities; 

    std::vector<bool> Basic_State;
    Basic_State = Unassigned_State;
    std::vector<bool> B = Unassigned_State;
    int Size_of_Set = Set_States_nz.size();
    if (Size_of_Set == 0) {
        AllPossible_States_assignedNZ.emplace(Basic_State);


    }

    else {


        //Here we will determine the indices of the one's and how many one's there are of the first element in the set.
        std::vector<bool> Statenz = Set_States_nz[0];


        std::vector <int> Indices_set = Determine_Set_indices(Statenz);
        //Determine number of used processor for intersecting row/column.
        int Number_Set = Indices_set.size();



        Set_States_nz.erase(Set_States_nz.begin());
        std::vector<std::vector<bool>> NewSet_States_of_nz = Set_States_nz;



        //One of the indices that is one is set to 1 rest is kept zero.
        for (int i = 0; i < Number_Set; i++) {



            int A = Indices_set[i];
            Basic_State[A] = 1;
            std::vector<bool> Basic_State2 = Basic_State;
            Basic_State = B;

            std::set<std::vector<bool>> result = Assigned_States(NewSet_States_of_nz, Basic_State2);


            for (auto l = result.begin(); l != result.end(); l++) {

                AllPossible_States_assignedNZ.insert(*l);
            }



        }
        // std::cout << "tot hier einde else";
    }
    // Print the states;
    std::cout << "\n" << "Hoeveelheid states in verzameling: " << AllPossible_States_assignedNZ.size() << "\n";
    for (auto j = AllPossible_States_assignedNZ.begin(); j != AllPossible_States_assignedNZ.end(); j++) {

        //std::cout << "State " << j<< ": ";
        std::vector<bool> vec = *j;

        for (int l = 0; l < vec.size(); l++) {
            std::cout << vec[l] << " ";
        }

        std::cout << "\n";
    }

    return AllPossible_States_assignedNZ;


}


 std::vector<std::pair<int, std::vector<bool>>> L2bound(std::vector<bool> Status_intersect_rc, int rc) {


 
    //In code ordennen is dit een global variable
    std::set<std::vector<bool>> AllStates = States(Processors, 0, { 0,0,0 }, Processors);
    //Dit moet global var worden
    std::vector<std::vector<bool>> Index_and_Status = indexStatus_vs_Status(); //Links a specific state from allstates to its binairy index value. ToDo

    //Hierin wordt alle info voor L2 bound 2 opgeslagen en bijgehouden, moet worden meegegevn in tree.
    std::vector<std::pair<int,std::vector<bool>>>  Partial_Status_rowcols; //initialiseren op alles 0 elke int en elke bool imn vector.
    std::vector<bool> A = { 1,1,0,0,0,1 };
   
    Partial_Status_rowcols = { std::make_pair(1, A) };

   std::cout << Partial_Status_rowcols[rc].first; //This is the old L2_bound for this rc before we do any updating.

   //Determine the binairy index of the status of the intersecting rowcol.
    int index_state = Binair_index(Status_intersect_rc);

    //Now this rc has at least this partial status
    Partial_Status_rowcols[rc].second[index_state] = 1;

    //This is the vector that keeps all the information about the partial statuses of this rc, 
    std::vector<bool> info = Partial_Status_rowcols[rc].second;

    //Below we determine the partial statuses of rowcol "rc".
    //Save the binairy indices of the partial statuses and the partial statuses themselves in vectors.
    std::vector<int> index_partial_Statuses;
    std::vector<std::vector<bool>> Partial_Statuses;

    //First get the index of the partial statussen of the rowcol, "rc".
    for (int i = 0; i < 6  ; i++) { //7Moer dynamisch naar die options 2^p-2 ToDo

        if ( info[i]== 1) {
            index_partial_Statuses.push_back(i);
        }

    }

    int x = index_partial_Statuses.size();

    //Now use the indices of the partial statuses to get the partial statuses themselves and save them in a vector.
    for (int j = 0; j < x; j++) {
        int ind_St = index_partial_Statuses[j];
        Partial_Statuses.push_back(Index_and_Status[ind_St]);

    }
    //

    //Prints all partal statuses of intersect_rowcol.
    std::cout << "All partial states: ";
    for (int h = 0; h < Partial_Statuses.size();h++) {
        std::cout << "\n";
        for (int g = 0; g < 3; g++) {

            std::cout << Partial_Statuses[h][g];
        }
    }
    std::cout << "\n";
    //
    

    
  
    //Use the assigned_States function to find all possible options for assignments based on the partial statuses alone for this rowcol 
    //and find the option with the least cuts i.e. state with the least number of ones.
    std::set<std::vector<bool>> Partial_options_rc=Assigned_States(Partial_Statuses, { 0,0,0 }); //ToDo {0,0,0,} dynamisch is ZeroState

    //Save the state with the least number of ones. 
    //The initial value is the state in which all processors are used.ToDo

    // std::vector<bool> State_least_ones(Processors,1);
     int no_least_ones = Processors; //ToDo kan ook P-1 maar zo zorg je ervoor dat ie iig altijd met waaarde wordt vervangen uit partial_options_rc

    for (auto k = Partial_options_rc.begin(); k != Partial_options_rc.end(); k++) {

        //Determine the number of ones of state *k
        int no_ones = Determine_Set_indices(*k).size();
       
        //If no of ones i.e. no of processors in this state is less than the state with the least no of ones until now
        //Replace the state with least no of ones by this state.
        if (no_ones < no_least_ones) {
            no_least_ones = no_ones;
         //   State_least_ones = *k;
        }


    }

    //Then the new L2_bound for this rowcol is;
    int L2_boun_rc = no_least_ones - 1;

    //Need to save the new L2_bound for this rowcol;
    Partial_Status_rowcols[rc].first = L2_boun_rc;

    //This prints the new L2 bound
    std::cout << no_least_ones<< "and L2 bound thi rc is: " << Partial_Status_rowcols[rc].first;

    return Partial_Status_rowcols;
}



int main()
{
    std::vector < std::vector<bool>> Hoi = indexStatus_vs_Status();

  /*  for (int i = 0; i < Hoi.size(); i++) {
        std::cout << "index " << i << " :";
        for (int j = 0; j < Processors; j++) {

            std::cout << Hoi[i][j]<<" ";
        }
        std::cout << "\n";
    }*/


    std::vector<bool> AB = { 0,1,0 };
    int blegh = 0;

    L2bound(AB ,blegh);
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
