
#include "./Possible_States.h"
#include<algorithm>
#include<vector>
#include <math.h>
#include "./matrix.h"
#include "./Global.h"
#include<set>
#include "./Util.h"
#include<numeric>
#include<Windows.h>
#include <tuple>
#include"./Symmetry.h"



std::vector<std::pair<int, std::vector<bool>>> L2bound(std::vector<bool> Status_intersect_rc, int rc, std::vector<std::pair<int, std::vector<bool>>> Partial_Status_rowcols) {


 


    //std::cout << Partial_Status_rowcols[rc].first; //This is the old L2_bound for this rc before we do any updating.

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

    int options = pow(2, Processors) - 2;
    //First get the index of the partial statussen of the rowcol, "rc".
    for (int i = 0; i < options; i++) { //nu dynamisch nog 1 x check met options ToDo

        if (info[i] == 1) {
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
   /* std::cout << "All partial states: ";
    for (int h = 0; h < Partial_Statuses.size(); h++) {
        std::cout << "\n";
        for (int g = 0; g < 3; g++) {

            std::cout << Partial_Statuses[h][g];
        }
    }
    std::cout << "\n";*/
    //




    //Use the assigned_States function to find all possible options for assignments based on the partial statuses alone for this rowcol 
    //and find the option with the least cuts i.e. state with the least number of ones.
    std::set<std::vector<bool>> Partial_options_rc = Assigned_States(Partial_Statuses, Zero_State); 

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
//    std::cout << no_least_ones << "and L2 bound thi rc is: " << Partial_Status_rowcols[rc].first;

    return Partial_Status_rowcols;
}
