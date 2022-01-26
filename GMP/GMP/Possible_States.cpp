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
#include<tuple>
#include"./Symmetry.h"
#include"Lower_bounds.h"
#include<utility>
#include<array>
#include<fstream>


//Function that determines the order of the row and columns based on the number of nonzeros in a rowcol.
//No distinction between rows and columns.
//The input needs to be "matrix.perRow_Col".
//The output is the order of the rows and columns.
std::vector<int> Determine_Order_row_columns(std::vector<int> number_nz) {

    //Vector in which the order is stored.
    std::vector<int> Order__row_columns;

    int number_of_rows_columns = number_nz.size();
    for (int i = 0; i < number_of_rows_columns; i++) {

        int Maximal_Value=*std::max_element(number_nz.begin(), number_nz.end());

        //Makes sure that a rowcol with 0 nonzeros is not taken into account.
        if (Maximal_Value < 1) {            
            break;
        }

        int Index_MaxElement = std::max_element(number_nz.begin(), number_nz.end()) - number_nz.begin();

        Order__row_columns.push_back(Index_MaxElement);
        number_nz[Index_MaxElement] = -1;

    }

    //Prints the order of the row and columns, rowcol with the most nz is first.
    /*std::cout <<"\n"<< "Order of the row and columns, using order1: ";
    for (int j = 0; j < Order__row_columns.size(); j++) {
        std::cout << Order__row_columns[j] << "  ";
    }
    std::cout << "\n";
    */
    
    return Order__row_columns;
}

std::vector<int> Determine_Order_row_columns3(std::vector<int> number_nz, matrix A) {

    //Vector in which the order is stored.
    std::vector<int> Order__row_columns;

    int number_of_rows_columns = number_nz.size();


    for (int k = 0; k < number_of_rows_columns; k++) {

        if (number_nz[k] == 0) {
            number_nz[k] = -1;
        }

        else { 
            continue; 
        }
    }

    for (int i = 0; i < number_of_rows_columns; i++) {

        int Maximal_Value = *std::max_element(number_nz.begin(), number_nz.end());

        //Makes sure that a rowcol with 0 nonzeros is not taken into account.
        if (Maximal_Value < 0) {
            break;
        }

        int Index_MaxElement = std::max_element(number_nz.begin(), number_nz.end()) - number_nz.begin();

        Order__row_columns.push_back(Index_MaxElement);
        number_nz[Index_MaxElement] = -1;

    

        std::vector<int> intersecting_rc_maxrc = A.Intersecting_RowCol(Index_MaxElement);
        int no_intersecting = intersecting_rc_maxrc.size();
        for (int j = 0; j < no_intersecting; j++) {

            int rowcol = intersecting_rc_maxrc[j];
            number_nz[rowcol] -= 1;

        }


    }
    //Prints the order of the row and columns, rowcol with the most nz is first.
    /*std::cout << "Order of the row and columns, using order3: ";
    for (int j = 0; j < Order__row_columns.size(); j++) {
        std::cout << Order__row_columns[j] << "  ";
    }
    std::cout << "\n";
    */
    return Order__row_columns;
}


//Function that determines the order of the row and columns based on the number of nonzeros in a row/column
//In the order rows and columns alternate
//The input is " matrix.perRow_Col"
std::vector<int> Determine_Order_row_columns2(std::vector<int> number_nz, int M) {

    //Vector in which order is stored.
    std::vector<int> Order__row_columns;

    //Boolean used to alternate between rows and columns.
    //If we want to look at the rows x=0 , when x=1 we look at the the columns.
    bool x = 0;
    //Boolean to keep track if there is still a row with no of nonzeros >=1.
    bool y = 0;
    //Boolean to keep track if there is still a column with no of nonzeros >=1.
    bool z = 0;

    //First determine if the rowcol with maximum no of nonzeros is a row or column.
   int Index_Maximal_Element = std::max_element(number_nz.begin(), number_nz.end()) - number_nz.begin();
   int Maximal_value;
    if (Index_Maximal_Element<M) {
        x = 0;
    }
    else { 
        x = 1; 
    }

    //Alternate between the rows and the columns, order of rowcols is found when there are no rowcols left with no nonzeros>=1,
    // i.e. when y==1 && z==1.
    while(y==0 || z==0) {

        int Index_MaxElement;

        //Find row/col with maximum number of nonzeros, and we alternate between rows and cols.
        if (x == 0 && y!=1) {

            Maximal_value = *std::max_element(number_nz.begin(), number_nz.begin() + M);
            Index_MaxElement = std::max_element(number_nz.begin(), number_nz.begin() + M) - (number_nz.begin());
            x = 1;

            //Makes sure that rows with 0 nonzeros are not taken into account.
            if (Maximal_value < 1) {           
                y = 1;
                continue;
            }
        }

        else  {
            Maximal_value = *std::max_element(number_nz.begin() + M, number_nz.end());
            Index_MaxElement = std::max_element(number_nz.begin() + M, number_nz.end()) - (number_nz.begin());
            //set bool back to 0 to make sure that next rowcol is a row 
            x = 0;

             //Makes sure that columns with 0 nonzeros are not taken into account.
            if (Maximal_value < 1) {           
                z = 1;
                continue;
            }
        }

       // std::cout << "  " << Maximal_value << " " << Index_MaxElement;
        Order__row_columns.push_back(Index_MaxElement);
        number_nz[Index_MaxElement] = -1;
    }


    //Prints the order of the row and columns, order is on basis of number of nz in row/column.
    //Row/column with the most nz is first.
    /*std::cout << "Order of the row and columns, using order2: ";
    for (int j = 0; j < Order__row_columns.size(); j++) {
        std::cout << Order__row_columns[j] << "  ";
    }
    std::cout << "\n";
    */
    return Order__row_columns;
}


//Determines based on a given state and the number of free nz in a rowcol , which states there are possible for that rowcol.
//i.e. if state=(1,0,0) & 1 free nz, then the possible states are {(1,1,0); (1,0,1)}

//Input; Freeindices= processors that don't own a nonzero in this rowcol, state, number of free nz, set to hold all the possible states.
//Output; Set with all possible states based on a starting state and the numebr of free nonzeros.
std::set<std::vector<bool>> Possible_States_meerFreeNZ(std::vector<int> freeindices, std::vector<bool> State,
    int number_NZ, std::set<std::vector<bool>> Begin) {


    //If there are no free nonzeros in the rowcol or there are no unused processors,
    //Then all possible states are found.
    if (number_NZ == 0 || freeindices.size() == 0) {
        //Prints the set of all possible states
       // std::cout << "size of set: " << Begin.size() << "\n";
     
        //for (auto j = Begin.begin(); j != Begin.end(); j++) {

        //    //std::cout << "State " << j<< ": ";
        //    std::vector<bool> vec = *j;

        //    for (int l = 0; l < vec.size(); l++) {
        //        std::cout << vec[l] << " ";
        //    }

        //    std::cout << "\n";
        //}

        return Begin;
    }

    else {
        //Number of processors not used in the rowcol.
        int nm_free_indices = freeindices.size();

        std::vector<int> New_freeProcessors = freeindices;
        std::set<std::vector<bool>> Result;
        Result = Begin;

        //Traverse all processors that do not own nonzeros in the rowcol.
        for (int i = 0; i < nm_free_indices; i++) {

            std::vector<bool> State_New = State;

          
            int index = freeindices[i];
            //Make a permitted new state.
            State_New[index] = 1;       

            std::set<std::vector<bool>> All_States = Begin;

            //Save the new state.
            All_States.insert(State_New);

            //This processor is now used, erase it from th free processors list.
            New_freeProcessors.erase(New_freeProcessors.begin());


            //Recurse
            //number of free nz decreases with one so number_NZ -= 1.
            std::set<std::vector<bool>> result = Possible_States_meerFreeNZ(New_freeProcessors, State_New, number_NZ - 1, All_States);

            //Save the found possible states in the set that holds all possible states.
            for (auto l = result.begin(); l != result.end(); l++) {

                Result.insert(*l);
            }

        }

        return Result;
    }

}


//Function that determines the possble states for a rowcol given the states of already assigned rowcols, intersecting this rowcol in a nonzero.
//The states of the already assigned intersecting rowcols are given as input; Set_States_nz.
std::set<std::vector<bool>> Assigned_States(std::vector<std::vector<bool>> Set_States_nz, std::vector<bool> Unassigned_State) {

    //This will be our container with all correct states for the row/column, 
    //if we only look at the already assigned nz in a row/column. 
    std::set<std::vector<bool>> AllPossible_States_assignedNZ;


    std::vector<bool> Basic_State;
    Basic_State = Unassigned_State;
    std::vector<bool> B = Unassigned_State;
    int Size_of_Set = Set_States_nz.size();

    //If there are no nonzeros in the rowcol (anymore) that are already assigned to a processor(s);
    if (Size_of_Set == 0) {
        AllPossible_States_assignedNZ.emplace(Basic_State);

    }

    else {

        //Here we will determine the indices of the one's and how many one's there are of the first element in the set.
        std::vector<bool> Statenz = Set_States_nz[0];

        std::vector <int> Indices_set = Determine_Set_indices(Statenz);
        //Determine number of used processor for intersecting row/column.
        int Number_Set = Indices_set.size();

       
        //Adjust the set, Set_States_nz for the recursion step.
        Set_States_nz.erase(Set_States_nz.begin());
        std::vector<std::vector<bool>> NewSet_States_of_nz = Set_States_nz;

        //Loop over all the indices that are set to 1 in Statenz.
        for (int i = 0; i < Number_Set; i++) {


            int A = Indices_set[i];
            Basic_State[A] = 1;
            std::vector<bool> Basic_State2 = Basic_State;
            //For the next iteration in the loop set the value of Basic_State back to B=Unassigned_State
            Basic_State = B;

            //Now recurse
            //In the nezt recurion step we will look at the next element in Set_States_nz, 
            //i.e. the first element in NewSet_States_of_nz.
            std::set<std::vector<bool>> result = Assigned_States(NewSet_States_of_nz, Basic_State2);


            for (auto l = result.begin(); l != result.end(); l++) {

                AllPossible_States_assignedNZ.insert(*l);
            }

        }
  
    }
    // Print the states;
    //std::cout << "\n" << "Hoeveelheid states in verzameling: " << AllPossible_States_assignedNZ.size() << "\n";
    //for (auto j = AllPossible_States_assignedNZ.begin(); j != AllPossible_States_assignedNZ.end(); j++) {

    //    //std::cout << "State " << j<< ": ";
    //    std::vector<bool> vec = *j;

    //    for (int l = 0; l < vec.size(); l++) {
    //        std::cout << vec[l] << " ";
    //    }

    //    std::cout << "\n";
    //}


    return AllPossible_States_assignedNZ;
}

//This function combines the previous two functions; Assigned_States and Possible_States_meerFreeNZ.
//This function determines for a rowcol, all possible states for this rowcol, based on already assigned nonzeros in this rowcol, given by States_NZs
//and based on the free nonzeros in this rowcol, given by No_Free_NZ.
std::set<std::vector<bool>> Possible_States(std::vector<std::vector<bool>> States_NZs, int No_Free_NZ) {

    //The container of all possible states for the chosen row/column
    std::set<std::vector<bool>> Feasible_States;
 
    //Now determine and save alle possible states for this rowcol according to the assigned nonzeros in this rowcol;
    std::set<std::vector<bool>> Outcome_Assigned;
    Outcome_Assigned = Assigned_States(States_NZs, Zero_State);


    //All the states in the outcome set of assigend_States are feasible for this row/column
    //So insert in container
    for (auto l = Outcome_Assigned.begin(); l != Outcome_Assigned.end(); l++) {

        Feasible_States.insert(*l);
    }

    //Now traverse all states in Outcome_Assigned to take into account the free nonzeros in this rowcol.
    for (auto i = Outcome_Assigned.begin(); i != Outcome_Assigned.end(); i++) {

        //This info is needed for the function Possible_States_meerFreeNZ
        std::vector<bool> view_State = *i;
        std::vector<int> ZeroIndices;
        ZeroIndices = Determine_indices_zeros(view_State);
        std::set<std::vector<bool>> StartSet;

        //For this state, view_State, in Outcome_Assigned determine all possible states with regard to the free nonzeros in this rowcol
        // i.e. No_Free_Nz.
        std::set<std::vector<bool>> Outcome_Unass;
        Outcome_Unass = Possible_States_meerFreeNZ(ZeroIndices, view_State, No_Free_NZ, StartSet);

        //Outcomes in set of Possible_States_meerFreenz are other (new) feasible states for this row/column
        //So add them to the container
        for (auto k = Outcome_Unass.begin(); k != Outcome_Unass.end(); k++) {

            Feasible_States.insert(*k);
        }

    }
    // It is possible that Outcome_Assigned={(0, ..., 0)}, so remove the zero_state from the set Feasible_States.
    Feasible_States.erase(Zero_State);

    //std::cout << Feasible_States.size();
    return Feasible_States;
}



//Given a (partial) partition of a matrix, this fucntion determines the (explicit) communication volume of the (partial) partition.
int LowerBound1(std::vector < std::vector<bool>>The_Partition) {

    int size = The_Partition.size();
    int Number_of_cuts = 0;

    //Traverse all the rowcols.
    for (int i = 0; i < size; i++) {

        std::vector<bool> State_i;
        State_i = The_Partition[i];
        int Ones_State_i = std::count(State_i.begin(), State_i.end(), true);

        //Number of cuts in the rowcol i is number of ones minus one.
        int Cuts_row_column_i = Ones_State_i - 1;

        //A rowcol in a partial partition or a rowcol with 0 nonzeros can have status (0, ..., 0),
        //So only count the cuts of rowcol i when Cuts_row_column_i>=0.
        if (Cuts_row_column_i >= 0) {
            Number_of_cuts += Cuts_row_column_i;

        }
    }
    //std::cout << "\n" << "Lower bound 1 is;" << Number_of_cuts;
    return Number_of_cuts;
}

//Function , a check , used in Check_Load_Bal.
//Given a new status S ("Poss_Status_rc"), a status C ("color") s.t. S&&C=S, we want to know how many NEW nonzeros will be added to in the partition size of C,
//if rowcol gets status Poss_Status_rc.

//no_New_C is # nz's in rowcol (starting value of no_New_C that is returned by the function).
int Check1(std::vector<bool> Poss_Status_rc, int rowcol, std::vector<bool> color, std::vector<std::vector<int>> color_count, int no_New_C) {

    //All nonzeros in rowcol will fall  in partition of C, when it gets status "Poss_Status_rc".
    //Need tod determine how many new nz will fall in partition of C.
    for (auto k = AllStates.begin(); k != AllStates.end(); k++) {

        //The nonzeros in rowcol with status *k s.t. Proc(*k) subset of Proc(color), are already counted in the partition_size of C
        //So these nz are not New nz belonging to partition C, so substract them from no_New_c.
        if ((*k && color) == *k) {

            no_New_C -= color_count[Binair_index(*k)][rowcol]; 

        }
        else {
            continue;
        }

    }

    return no_New_C;
}

//Function , a check , used in Check_Load_Bal.
//Given a new status S ("Possible_Status_rc"), a status C ("C") s.t.
//Proc(C) n Proc(Possible_Status_rc) is not empty AND Proc(Possible_Status_rc) not subset of Proc(C).
//We want to know how many NEW nonzeros will be added to the partition size of C, if this rowcol gets status Possible_Status_rc.
//no_New_C has starting value 0.
int Check2(std::vector<bool> Possible_Status_rc, int rowcol, std::vector<bool> C, std::vector<std::vector<int>> color_count, int no_New_C) {

    
    for (auto k = AllStates.begin(); k != AllStates.end(); k++) {

        //So the NEW nz belonging to partition C are;
        //Nonzeros in rowcol with status *k  that are not already counted in partiton of C, 
        //(that is Proc(*k) not subset of Proc(C), i.e. *k && C != *k.)
        //And the new status of these nonzeros *k && Possible_Status_rc is completely "contained" in C,
        //(i.e. Proc(*k && Possible_Status_rc) subset of Proc(C).)
        if ((((*k && Possible_Status_rc) || C) == C) && ((C && *k) != *k)) {
            no_New_C += color_count[Binair_index(*k)][rowcol];  

        }
        else {
            continue;
        }
    }

    return no_New_C;
}


//This fucntion determines if a potential state, "Possible_Status_rc"  for a rowcol, "rowcol", is feasible given the load balance constraints.
//So it checks if the potential state is possible given the partition_sizes, the number of nonzeros added to certain partition sizes and the maximum allowed partition size.
//It returns a bool,if it is 1 then the potential state is feasible.
bool Check_Load_Bal(std::vector<bool> Possible_Status_rc, int rowcol, std::vector<int> Partition_size, std::vector<std::vector<int>> color_count, matrix B) {

    //Start the return bool at 1.
    bool The_Check = 1;

    //Determine how many processors are used in the potential state for this rowcol.
    int no_Proc = Determine_Set_indices(Possible_Status_rc).size();

  
    //If 1 processor is used in  the potential state:
    if (no_Proc == 1) {

       //Number of nonzeros added to partition_size of "Possible_Status_rc".
       int no_new_S = B.perRow_Col[rowcol] - color_count[Binair_index(Possible_Status_rc)][rowcol];

        
        //Check if we can still add this number of nonzeros to the partition size of the "Possible_status_rc" for the rowcol.
        if (Partition_size[Binair_index(Possible_Status_rc)] + no_new_S <= Max_Partition_size) {

            The_Check = 1;
        }

        //If # new nonzeros +Partition_size > maximum allowed partiiton size, then "Possible_status_rc" is not feasible for this rowcol.
        else {
            The_Check = 0;
            return The_Check;
        }

       
        //We also need to check the partiton sizes of the other states that also use this 1 processor. 
        //Here we will check the partition sizes of the states C s.t C&&S=S and C!=S.
        //So C will have more # ones than S if C&&S=S.
        for (auto l = AllStates.begin(); l != AllStates.end(); l++) {

            //We look at partition size of C.
            std::vector<bool> C = *l;

            if (((C && Possible_Status_rc) == Possible_Status_rc) && (C !=Possible_Status_rc)) { 

                //# nonzeros in the rowcol.
                int no_New_C = B.perRow_Col[rowcol];

                //Determine # new nonzeros that will belong to partition C
                no_New_C = Check1(Possible_Status_rc, rowcol, C, color_count, no_New_C);

                //How many processors are used in state "C".
                int x = Determine_Set_indices(C).size();

                //Determine the new value of "The_Check"; 
                //can we still add no_New_C to the partition_size of C without exceeding the max allowed partition size?
                The_Check = The_Check && (Partition_size[Binair_index(C)] + no_New_C <= x * Max_Partition_size);


                //If "The_Check"=0 we know that the potential state is not feasible and will never be feasible, so we can return 0.
                if (The_Check == 0) {
                    return 0;
                }
            }

            else {
                continue;
            }

        }

        return The_Check;

    }

    //If # processors used in the potential state == Processors, then we know that the potential satte is (1, ..,1)
    //This state is always feasible w.r.t. load balance.
   //So we don't need to check anything and we can return 1.
    else if (no_Proc == Processors) {

        return The_Check;
    }

    //If # processors used > 1 and < p.
    else {
     
        for (auto l = AllStates.begin(); l != AllStates.end(); l++) {

            //We look at partition size of state C.
            std::vector<bool> C = *l;

            //There are 4 cases of states C to distnguish:

            //1.
            //If state Possible_Status_rc is completely contained in state C.
            //So Proc(Possible_Status_rc) subset of Proc(C).
            if ((C && Possible_Status_rc) == Possible_Status_rc) {

                //# nonzeros in the rowcol.
                int no_New_C = B.perRow_Col[rowcol];

                //Determine # new nonzeros that will belong to partition C
                no_New_C = Check1(Possible_Status_rc, rowcol, C,  color_count, no_New_C);

                //How many processors are used in state "C".
                int x = Determine_Set_indices(C).size();

                //&& dus The_Check=1 iff The check was 1 and 
                // if we can still add no_New_C to the partition_size of C without exceeding the max allowed partition size.
                The_Check = The_Check && (Partition_size[Binair_index(C)] + no_New_C <= x * Max_Partition_size);

            }

            //2.
            //If state C is completely contained in Possible_Status_rc
            ////So Proc(C) subset of  Proc(Possible_Status_rc).
            else if ((C && Possible_Status_rc) == C) {

                int no_newC = 0;

                //Determine # new nonzeros that will belong to partition C
                no_newC = Check2(Possible_Status_rc, rowcol, C,  color_count, no_newC);

                //How many processors are used in state "C".
                int x = Determine_Set_indices(C).size();

                //&& dus The_Check=1 iff The check was 1 and 
                // if we can still add no_New_C to the partition_size of C without exceeding the max allowed partition size of C.
                The_Check = The_Check && (Partition_size[Binair_index(C)] + no_newC <= x * Max_Partition_size);
            }

            //3.
            //If state C and Possible_Status_rc have no processors in common.
            //Nothing needs to be checked, no nonzeros can be in state C in this rowcol if this rowcol gets state Possible_Status_rc.
            else if ((C && Possible_Status_rc) == Zero_State) {
                continue;
            }

            //4.
            //If state C and Possible_Sttaus_rc have some processors in common, but both states have a processor that the other one doesn't have.
            else {

                int no_newC = 0;

                //Determine # new nonzeros that will belong to partition C
                no_newC = Check2(Possible_Status_rc, rowcol, C, color_count, no_newC);

                //How many processors are used in state "C".
                int x = Determine_Set_indices(C).size();

                //&& dus The_Check=1 iff The check was 1 and 
               // if we can still add no_New_C to the partition_size of C without exceeding the max allowed partition size of C.
                The_Check = The_Check && (Partition_size[Binair_index(C)] + no_newC <= x * Max_Partition_size);
            }

            if (The_Check == 0) {
                return The_Check;
            }

        }

        return The_Check;
    }

}


 //This function updates; the Partition_size, the color_count for each rowcol, 
// the lowerbound for the partial partition given by "The_States", and the information for Lowerbound_2.
std::tuple<std::vector<int>, std::vector<int>, std::vector<std::vector<int>>, int, std::vector<std::pair<int, std::vector<bool>>> , std::array<std::vector<std::vector<int>>, 2> , int , 
    std::vector<int>, Bi_Graph > Update(int rowcol, std::vector<bool> Status_rowcol, matrix * A, std::vector<std::vector<bool>> The_States, std::vector<int> Vector_Freenz,
    std::vector<int> Partition_size, std::vector<std::vector<int>> color_count,int LB, std::vector<std::pair<int, std::vector<bool>>>  Partial_Status_rowcols,  
        std::array<std::vector<std::vector<int>>, 2> Packing_Sets2, int max_L3_L4, std::vector<int>Value_Partial_status , Bi_Graph bigraph) {

    LB -= max_L3_L4;
    //
  

    //Define two containers to keep track of the rowcols that need to be reoved or added to the graph in lowerbound 4.
    std::vector<int> add_vertices;
    std::vector<int> remove_vertices;

    //First update w.r.t the just assigned rowcol, "rowcol"; free nonzeros, packingsets, lowerbound.

    //There are no free nonzeros anymore in "rowcol".
    Vector_Freenz[rowcol] = 0;

  

    //Adjust the lowerbound "LB" remove for the assigned rowcol the L2  bound and add the L1 bound to Lowerbound "LB".

    //Remove the L2 bound from LB for assigned rowcol.
    LB -= Partial_Status_rowcols[rowcol].first;
    //Set L2 bound for "rowcol" to 0
    Partial_Status_rowcols[rowcol].first = 0;

    //Determine the L1 bound for "rowcol" and add it to the lowerbound of the partial partitioning.
    int increase_LB1 = LB1(Status_rowcol);
    LB += increase_LB1;

  

    if (LB >= UB) {
        return std::make_tuple(Vector_Freenz, Partition_size, color_count, LB, Partial_Status_rowcols,Packing_Sets2, max_L3_L4, Value_Partial_status, bigraph);

    }



    int old_value_partstat = Value_Partial_status[rowcol];
    int index_old_partstate = pow(2, old_value_partstat) - 1;
    if (old_value_partstat >= 0) {
        
        if (rowcol < A->M) {
           int index_no_nz = A->perRow_Col[rowcol] - color_count[index_old_partstate][rowcol];
           Packing_Sets2[0][old_value_partstat][index_no_nz] -= 1;
            
        }
         else {
            int index_no_nz = A->perRow_Col[rowcol] - color_count[index_old_partstate][rowcol];
            Packing_Sets2[1][old_value_partstat][index_no_nz] -= 1;
         }
    }
    

    //Update value partial status of the "rowcol" we set it to-3 meaning that this rowcol is assigned
    Value_Partial_status[rowcol] = -3;
   

   //Now update w.r.t. the intersecting rowcols;
   //Update; 

    //std::vector<std::pair<int, std::vector<bool>>> New_Partial_Status_rowcols = Partial_Status_rowcols;
 
    //Determine the rowcols that intersect "rowcol" in a nonzero.
    std::vector<int> Intersecting_rowcol = A -> Intersecting_RowCol(rowcol);

    //Traverse ovr all intersecting rowcols.
    for (auto i = Intersecting_rowcol.begin(); i != Intersecting_rowcol.end(); i++) {

        int index_itersect_rc = *i;

        //State of the intersecting rowcol.
        std::vector<bool> State_intersect_rc = The_States[index_itersect_rc];

        //Je hoeft niks te updten als nieuwe states allproc state kan dit meer aanbegin dat neit for lop in?? ToDo
      

        //Make the new state of the nonzero at the intersection of "rowcol" and index_itersect_rowcol".
        std::vector<bool> New_State_intersect_NZ;

        //If the intersecting rowcol is already assigned.
        if (State_intersect_rc != Zero_State) {
            New_State_intersect_NZ = Status_rowcol && State_intersect_rc;

            if (Status_rowcol == AllProc_State) {
            continue;
            }

           
            //If the state of the nonzero at the intersetion changes:
           

                int index_of_newStatus = Binair_index(New_State_intersect_NZ); //ToDo dit is al hierboven bepaald dit weg en 1 locake var maken voor deze fucntie?

                //Both rowcol and intersect rowcol get +1 nonzero with status " New_State_intersect_NZ".
                color_count[index_of_newStatus][index_itersect_rc] += 1;
                color_count[index_of_newStatus][rowcol] += 1;

                //If the previous nonzero was already assigned, i.e. State_intersect_rc !=(0,..0), and was not assigned the all Processor state,
                //i.e. State_intersect_rc !=(1,..1), then we need to remove one count of State_interscet_rc for both rowcol and intersect rowcol.
                if ((State_intersect_rc != Zero_State) && (State_intersect_rc != AllProc_State)) {
                    color_count[Binair_index(State_intersect_rc)][index_itersect_rc] -= 1;
                    color_count[Binair_index(State_intersect_rc)][rowcol] -= 1;
                }

        }
        //Intersect rowcol is not assigned
        else {
            New_State_intersect_NZ = Status_rowcol;
          
            //Number of free nonzeros in intersecting rowcol decreases by 1.    
            Vector_Freenz[index_itersect_rc] -= 1;

            if (Status_rowcol == AllProc_State) {
                continue;
            }

            int index_of_newStatus = Binair_index(New_State_intersect_NZ); //ToDo dit is al hierboven bepaald dit weg en 1 locake var maken voor deze fucntie?

            //Both rowcol and intersect rowcol get +1 nonzero with status " New_State_intersect_NZ".
            color_count[index_of_newStatus][index_itersect_rc] += 1;
            color_count[index_of_newStatus][rowcol] += 1;


          //Update the L2 bound and the partial_status for this intersecting rowcol.

          int index_newst= Binair_index(New_State_intersect_NZ);

          //Old L2 bound for this intersecting rowcol
          int old_L2_intersectrc = Partial_Status_rowcols[index_itersect_rc].first;

          //Had the intersecting rowcol already a nonzero with status " New_State_intersect_NZ",
          //i.e. is there already another rowcol with status " New_State_intersect_NZ"  that intersects this "intersect rowcol".
          bool Partial_status_newST=  Partial_Status_rowcols[index_itersect_rc].second[index_newst];

          //If old L2 bound is maximal or the intersect rowcol already has status " New_State_intersect_NZ", nothing needs to be done for the L2 bound.
          if ((old_L2_intersectrc == (Processors - 1)) || Partial_status_newST == 1) {

          }

          else {
              //Remove the old L2 bound for the intersecting rowcol from "LB".
              LB -= old_L2_intersectrc;
             
              //Determine the new L2 bound and update Partial_status for this intersecting rowcol.
              Partial_Status_rowcols=L2bound(Status_rowcol, index_itersect_rc, Partial_Status_rowcols);

             //Add the new L2 bound.
             LB += Partial_Status_rowcols[index_itersect_rc].first;
          /*   if (Partial_Status_rowcols[index_itersect_rc].first == (Processors - 1)) {
                 std::cout << " L2 ";
             }*/
          

          }
       
          if (LB >= UB) {
              return std::make_tuple(Vector_Freenz, Partition_size, color_count, LB, Partial_Status_rowcols,Packing_Sets2, max_L3_L4, Value_Partial_status, bigraph);
          }
          //Now Update the packig sets for the local L3 bound, for this intersect rowcol.

           //Get the partial statuses for this intersect rowcol.
           std:: vector<bool> partial_statuses_intersectRC = Partial_Status_rowcols[index_itersect_rc].second;
           //Get the L2 bound for this rowcol
           int L2_bound_intersectrc = Partial_Status_rowcols[index_itersect_rc].first;

           int sum_1Proc_Partial_status = 0;
           int index_Partial_status;
           int index_packingset;
           std::vector<int> all_1procs;
           int sum_partial_statuses = std::accumulate(partial_statuses_intersectRC.begin(), partial_statuses_intersectRC.end(), 0);
           int val;
           if (sum_partial_statuses == 1) {

                val = std::max_element(partial_statuses_intersectRC.begin(), partial_statuses_intersectRC.end()) - partial_statuses_intersectRC.begin();
           }

           //Check  how many 1proc states this intersecting rowccolumn has 
          for (int i = 0; i < Processors; i++) {
              int view_index=Info_L3[i];

              sum_1Proc_Partial_status += partial_statuses_intersectRC[view_index];

              if (partial_statuses_intersectRC[view_index] == 1) {
                  index_Partial_status = view_index;
                  index_packingset = i;
                  all_1procs.push_back(i);
              }
          }

       

           //If this intersect rowcol has partial sum 1 than it has partial one state  if this one state exist of one processor .
          //add the free nonzeros in the correct packing set.
          if (L2_bound_intersectrc== 0 && sum_1Proc_Partial_status == 1) {

              //This rowcol needs to be added as a vretex in the graph of lowerbound 4.
              add_vertices.push_back(index_itersect_rc);
              //Adjust/confirm/save the value of the partial status of the intersecting rowcol
              

              int old_partial_value = Value_Partial_status[index_itersect_rc];
              Value_Partial_status[index_itersect_rc] = index_packingset;
              int sum_state = std::accumulate(Status_rowcol.begin(), Status_rowcol.end(), 0);

              if (old_partial_value == index_packingset) {
                 
                  if (sum_state==1) {
                        
                      if (index_itersect_rc < A->M) {

                          int index_no_nz = A->perRow_Col[index_itersect_rc] - color_count[index_Partial_status][index_itersect_rc];
                          int previous_no_nz = index_no_nz + 1;
                          Packing_Sets2[0][index_packingset][previous_no_nz] -= 1;
                          Packing_Sets2[0][index_packingset][index_no_nz] += 1;
                      }

                      else {
                          int index_no_nz = A->perRow_Col[index_itersect_rc] - color_count[index_Partial_status][index_itersect_rc];
                          int previous_no_nz = index_no_nz + 1;
                          Packing_Sets2[1][index_packingset][previous_no_nz] -= 1;
                          Packing_Sets2[1][index_packingset][index_no_nz] += 1;

                      }
                  }
                  else {

                  }
              }
              else if (old_partial_value < 0) {
                 
                  if (index_itersect_rc < A->M) {

                      int index_no_nz = A->perRow_Col[index_itersect_rc] - color_count[index_Partial_status][index_itersect_rc];
                      Packing_Sets2[0][index_packingset][index_no_nz] += 1;
                  }

                  else {
                      int index_no_nz = A->perRow_Col[index_itersect_rc] - color_count[index_Partial_status][index_itersect_rc];
                      Packing_Sets2[1][index_packingset][index_no_nz] += 1;

                  }
              }
          }

          ////Sum larger than 1 or smaller than, the values in packing sets for this rowcol must be zero.
         ////, because this rowcol is not partially coloured in one colour.    
          else {

              //This rowcol needs to be removed from the graph of lowerbound 4.
              remove_vertices.push_back(index_itersect_rc);

              int old_partial_value = Value_Partial_status[index_itersect_rc];

              //Remove the partial status of this intersecting row/column, set it to -7 meaning rowcol is partially assigned to more than 1 or 2 processors.
              Value_Partial_status[index_itersect_rc] = -7;

              if (old_partial_value >= 0) {
                  
                  int index_old_partvalue = pow(2, old_partial_value) - 1;
                int index_no_nz = A->perRow_Col[index_itersect_rc] - color_count[index_old_partvalue][index_itersect_rc];
                if (index_itersect_rc < A->M) {

                    Packing_Sets2[0][old_partial_value][index_no_nz] -= 1;
                }
                else {
                    Packing_Sets2[1][old_partial_value][index_no_nz] -= 1;
                }
              }
          }

          //If there are >2 processors, then an unassigned rowcol can also be partially assigned to 2 processors.
          //Adjust the values of the partial states of these rowcols.
          if (Processors > 2) {
              if (L2_bound_intersectrc == 1 && sum_1Proc_Partial_status == 2) {
                  std::vector<bool> v(Processors, 0);
                  for (auto l = all_1procs.begin(); l != all_1procs.end(); l++) {
                      v[*l] = 1;

                  }
                  int partial_value = (-1 * Binair_index(v));


                  Value_Partial_status[index_itersect_rc] = partial_value;
              }
              if (L2_bound_intersectrc == 0 && sum_partial_statuses == 1 && (Indices_2proc_states.find(val) != Indices_2proc_states.end())) {
                  Value_Partial_status[index_itersect_rc] = (-1 * val);

              }
          }
        }
        
        //Now update Partition sizes.

        //If the state of the nz where the rowcol and the intersceting rowcol intersect doesn't change
        //colour_count for both the rowcol and intersect rowcol stay the same.
        //Also Partition_size stays the smae.
        if (New_State_intersect_NZ == State_intersect_rc) {
            continue;
        }
        //If the state of the nonzero at the intersetion changes:
        else {

            int index_of_newStatus = Binair_index(New_State_intersect_NZ); //ToDo dit is al hierboven bepaald dit weg en 1 locake var maken voor deze fucntie?

          //Update the Partition sizes

            for (auto j = AllStates.begin(); j != AllStates.end(); j++) { 

                //If new_state is completely contained in *j, i.e. all processor used in new_state are also used in *j,
                //and the nonzero was not already counted in partition_size of *j, because the previous state of the nonzero, state intersect,
                //was completely contained in *j.
                //Than add count to Partition_size of *j.
                if (((New_State_intersect_NZ && *j) == New_State_intersect_NZ) && ((State_intersect_rc == Zero_State) || ((*j && State_intersect_rc) != State_intersect_rc)) ) { 

                    int index_St = Binair_index(*j);

                    Partition_size[index_St] += 1;

                }

            }


        }

       

    }

    int L3a = 0;
 //Deterine the new L3 bound for this partial partitioning
     L3a = L3bound_improved(Packing_Sets2, Partition_size, A);
    

 
 
 if ( (LB  + L3a)  >= UB) {
     LB += L3a;
      return std::make_tuple(Vector_Freenz, Partition_size, color_count, LB, Partial_Status_rowcols, Packing_Sets2, max_L3_L4, Value_Partial_status, bigraph);
  }

 
  
  //Determine the new local L4 bound
  bigraph.Set_rowcol(rowcol, add_vertices, remove_vertices ,A, Value_Partial_status,  Partition_size, Packing_Sets2, color_count);

  //The value of the L4 bound
  int new_L4 = bigraph.no_Matched; 
  
  if (CombL3_L4) {
      new_L4 += bigraph.L4_combL3;  
  }

  if ((LB  + new_L4) >= UB) {
      LB += new_L4;
      return std::make_tuple(Vector_Freenz, Partition_size, color_count, LB, Partial_Status_rowcols,Packing_Sets2, max_L3_L4, Value_Partial_status, bigraph);
  }

  int lowerbound = LB ;
  int GL4 = 0;
  if (GL4_on) {
      GL4 = BFS_Global_L4(Value_Partial_status,  A,  Partition_size, lowerbound, The_States, Packing_Sets2, color_count);
  }

  //Determine max of L3 , L5, and sum(GL4+ max(GL3, L3))=GL5 bound
  if(L3a >= new_L4 && L3a >= GL4) {
      max_L3_L4 = L3a;
  }
  else if (GL4 >= L3a && GL4 >= new_L4) {
      max_L3_L4 = GL4;
  }
  else { 
  max_L3_L4 = new_L4;

  }

  LB += max_L3_L4;
 
    return std::make_tuple(Vector_Freenz, Partition_size, color_count, LB, Partial_Status_rowcols, Packing_Sets2, max_L3_L4, Value_Partial_status, bigraph);
}




//Sort function used to alternate between processors
struct Alt_color1 {

    Alt_color1(int index) { this->index = index; }


    bool operator () (const std::vector<bool>& a, const std::vector<bool>& b) {


       

        return((a[index] > b[index]) && std::accumulate(a.begin(), a.end(), 0) != Processors || (a[index] == b[index] && std::accumulate(a.begin(), a.end(), 0) < std::accumulate(b.begin(), b.end(), 0)) ||
            std::accumulate(b.begin(), b.end(), 0) == Processors);
    }
    int index;
};


//Sort fucntion used to alternate between processors
//If we have  states that can be assigned to the rowcol, these states are sorted first of all based on the number of processors used in the state.
//and second on the number of processors a state contains that have part size == the minimum part size at the moment.
struct Alt_color2 {

    Alt_color2(std::vector<bool> index) { this->index = index; }


    bool operator () (const std::vector<bool>& a, const std::vector<bool>& b) {


        std::vector<bool> A = a && index;
        std::vector<bool> B = b && index;
        int sumA = std::accumulate(A.begin(), A.end(), 0);
        int sumB = std::accumulate(B.begin(), B.end(), 0);


        return(std::accumulate(a.begin(), a.end(), 0) < std::accumulate(b.begin(), b.end(), 0) ||( (std::accumulate(a.begin(), a.end(), 0) == std::accumulate(b.begin(), b.end(), 0) ) && sumA > sumB));
    }
    std::vector<bool> index;
};


struct Alt_color0 {

    Alt_color0(int index) { this->index = index; }
    bool operator () (const std::vector<bool>& a, const std::vector<bool>& b) {




        return((std::accumulate(a.begin(), a.end(), 0) < std::accumulate(b.begin(), b.end(), 0)) || ((std::accumulate(a.begin(), a.end(), 0) == std::accumulate(b.begin(), b.end(), 0)) && (a[index] > b[index])));
    }


    int index;
};

struct Alt_color3 {

    Alt_color3(std::vector<bool> index) { this->index = index; }


    bool operator () (const std::vector<bool>& a, const std::vector<bool>& b) {


        std::vector<bool> A = a && index;
        std::vector<bool> B = b && index;
        int sum_proc_A = std::accumulate(A.begin(), A.end(), 0);
        int sum_proc_B = std::accumulate(B.begin(), B.end(), 0);
        int sum_a = std::accumulate(a.begin(), a.end(), 0);
        int sum_b = std::accumulate(b.begin(), b.end(), 0);

        return((sum_a < sum_b&& sum_proc_A != 0) || (sum_proc_A != 0 && sum_proc_B == 0 && sum_a != Processors) || ((sum_a == sum_b) && (sum_proc_A > sum_proc_B)) || sum_b == Processors);
    }
    std::vector<bool> index;
};

//A short to store the state of the previous value of GetAsyncKeystate (used in the function Partition).
short previous_State = 0;

//Two integer to store informaion about number of "better" partitions found and number of partitions that were aborted because LB>=UB.
//Used in  the B&B tree.
int AAnt = 0;
int Aant_aborted=0;




//The B&B TREE
//The tree first traverses one branch until a leaf. (DFS)
void Partition(std::vector<std::vector<bool>> The_States,  std::vector<int> &Order_rows_columns,
    int a, matrix * Info, std::vector<int> Vrije_NZ, std::vector<int> Partition_size, std::vector<std::vector<int>> color_count,
    int lowerbound, std::vector<std::pair<int, std::vector<bool>>>  Partial_Status_rowcols,  std::array<std::vector<std::vector<int>>, 2> Packing_Sets2,
    int maxval_pm, std::vector<int> value_partialstatus, Bi_Graph bigraph, Symmetry_processors Symm) {


    //Open the file n which we store all new ub and corresponding partitions for the matrix
    std::ofstream Solution_and_info;;
    Solution_and_info.open(filename_Sol_info, std::ios::out | std::ios::app);

    //Code below is to check in which subtree Partition fuction currently is.
    //When pussing the "ALT" button (in windows) the state of the first rowcol and the second rowcol is printed. 
    short current_State=GetAsyncKeyState(18);

    if ((previous_State != current_State) &&  previous_State==0) {
        std::cout << "Current Subtree:";
        for (int i = 0; i < Processors; i++) {
            std::cout << The_States[Order_rows_columns[0]][i];
        }
        std::cout << " , ";
        for (int j = 0; j < Processors; j++) {
            std::cout << The_States[Order_rows_columns[1]][j];
        }
        std::cout << "\n";
    }
    previous_State = current_State;
  

    if (Lowest_cv_sofar == Overall_LB) {
        return;
    }
   
    //If Lb>=UB for the partial partitionend matrix we can stop exploring this branch and prune the tree.
    if (lowerbound >= UB) {
        Aant_aborted++;

        //If number of aorted partial partitions is a multiple of one million, print Aant_aborted.
        if (Aant_aborted % 10000000 == 0) {
            std::cout << Aant_aborted << "\n";
        }

        return;
    }

    
    

    int Layers_Tree =The_States.size();
    
    //If a==0, then all rowcols with more than 0 nonzeros are asssigned a state.
    //So the whole matrix is partitioned.
    if (a == 0) {
       /* std::cout << "check0";*/
      
        //Detemine the communication voume of this new partitioning
        int Comm_vol = LowerBound1(The_States);

        //Extra check if Comm_vol<=UB, this should be the case (actually comm_vol<UB), because of previous if statement(LB>=UB.)
        if (Comm_vol <= UB) {

            //Save the nnew better partition and UB.
            Save_best_solution_sofar(The_States, LowerBound1(The_States));
            
            //How many partitions where aborted before this partition?
            //(Also in file)
            std::cout << Aant_aborted << "\n";
            Solution_and_info << Aant_aborted << "\n";

            //Prints the new and better solution.
            //(Also in file)
            for (int j = 0; j < Layers_Tree; j++) {
                std::vector<bool> Stateof_j = The_States[j];

                /* ToDo Dit beter om aan te geven dat rijcol niet iegdeed is aangezien 0 nz in rijcol???
                 if (Stateof_j = Zero_State) {
                     std::cout << " x ";
                 }*/

                std::cout << "  ";
                Solution_and_info << "  ";
                for (int l = 0; l < Processors; l++) {

                    std::cout << Stateof_j[l];
                    Solution_and_info << Stateof_j[l];
                }

            }
            //Print communication volume of the partition
            //(Also in file)
            std::cout << " CV: " << Comm_vol << " ";
            std::cout << ": " << AAnt << "   ";   
            std::cout << "\n";

            Solution_and_info << " CV: " << Comm_vol << " ";
            Solution_and_info << ": " << AAnt << "   ";
            Solution_and_info << "\n";

            //Update the upperbound, the new UB is equal to the communication volume of this new partition.
            UB = Comm_vol;

          
        }
      
        AAnt++;
    }

  

    //If we didn't stop the tree manually, LB<UB and a>0, then we will go deeper in the tree and partition the next rowcol
    else {
        
       // int y = Order_rows_columns.size();

     

        //Determine the index of the next rowcol 
        int i = Order_rows_columns[no_tobeAssigned-a];
     
        
        //Determine the indices of the  rows/columns intersecting with this rowocl in a nonzero.
        std::vector<int> IntersectingRowCol= Info -> Intersecting_RowCol(i); 

        //Determine the no of free nz in the rowcol that you're about to assign a status.
        int no_Free_nzi = Vrije_NZ[i]; 

        //Print no free nz in rowcol that you're  about to assign a status.
        //   std::cout << "aantal vrije nz: " << no_Free_nzi;
 

        /// This gives the states of the intersecting rowcol, and saves them in a vector
        /// This vector is an argument for the Possible_States function.
        std::vector<std::vector<bool>> Empty;
        int no_intersecting_rowcol = IntersectingRowCol.size();

        for (int h = 0; h < no_intersecting_rowcol; h++) {
            int index_Intersect_RowCol = IntersectingRowCol[h];
            std::vector<bool> State_Intersect_ROWCOl = The_States[index_Intersect_RowCol];

            //Only take into account the state of the intersecting rowcol if the rowcol is assigend.
            if (accumulate(State_Intersect_ROWCOl.begin(), State_Intersect_ROWCOl.end(), 0) != 0) {

                Empty.emplace_back(State_Intersect_ROWCOl);
            }
        }

        //Now determine the posibbel states for this rowcol, given the intersecting states and the no of free nz in this rowcol.
        std::set<std::vector<bool>> FeasibleStates;
        FeasibleStates = Possible_States(Empty, no_Free_nzi);



        //Do the loadbalance check for every state in FeasibleStates
        std::set<std::vector<bool>> FeasibleStates_Final;

        for (auto l = FeasibleStates.begin(); l != FeasibleStates.end(); l++) {

            std::vector<bool> State_to_Check = *l;

           bool Checkvalue= Check_Load_Bal(State_to_Check, i, Partition_size, color_count, *Info);

           //Only keep the state if it is allowed acoording to the load balance rule.
           if (Checkvalue == 1) {

               FeasibleStates_Final.insert(State_to_Check);
           }
           else { 
               continue; }

        }

         //Include the first symmetry step, that is remove the symmetry that occurs in assigning the first rowcol.
        if (a == no_tobeAssigned) {
            FeasibleStates_Final=First_Symmetry_Check(First_Symmetry_Set(), FeasibleStates_Final);
        }

       
        if (a < no_tobeAssigned) {

            if (Symm.No_symmetry == 0) {
                Symm.adjust_processor_sets(The_States[Order_rows_columns[(no_tobeAssigned - a - 1)]]);

                if (Symm.No_symmetry == 0) {

                    std::set < std::vector<bool>> initial_set = { Zero_State };
                    std::set<std::vector<bool>> Symmset = Symm.make_symmetry_set(0,initial_set);
                    FeasibleStates_Final = Second_Symmetry_Check(Symmset, FeasibleStates_Final);
                }
            }
        }
      

        std::vector<std::vector<bool>> FeasibleStates_FinalOrder(FeasibleStates_Final.begin(), FeasibleStates_Final.end());

        //Reverses the order of the subtrees of the first row/column, if Rev is "activated" , i.e. Rev=1;
        if ((a == no_tobeAssigned) && (Rev==1) ) {
            std::reverse(FeasibleStates_FinalOrder.begin(), FeasibleStates_FinalOrder.end());
        }
       
        
        //Alternating the processors, Determine the processor that owns the least nonzeros
        //Traverse first the states that contain this processor for the current rowcol.
        //To do this change the order of the vector.
        if (a != no_tobeAssigned) {

            //Information needed to use sorting functien Altcolor1 or Altcolor0
          /*  int index_lowest_partition = std::min_element(Partition_size.begin(), Partition_size.end()) - Partition_size.begin();
            int Proc_lowest_partition = log2(index_lowest_partition + 1);
           std::sort(FeasibleStates_FinalOrder.begin(), FeasibleStates_FinalOrder.end(), Alt_color1(Proc_lowest_partition));
         */

            //Information needed to use sorting function Altcolor2
            std::vector<int> Oneprocsize;
            std::vector<bool> info_minparts(Processors,0);
            Oneprocsize.resize(Processors);

            for (int j = 0; j < Processors; j++) {

                int  index = pow(2, j) - 1;
                Oneprocsize[j]=Partition_size[index];
            }

            int min_Part = *std::min_element(Oneprocsize.begin(), Oneprocsize.end() );
            for (int k = 0; k < Processors; k++) {

                if (Oneprocsize[k] == min_Part) {
                    info_minparts[k] = 1;
                }
            }

           std::sort(FeasibleStates_FinalOrder.begin(), FeasibleStates_FinalOrder.end(), Alt_color2(info_minparts) );
        }

        //Now traverse all Feasible states for this rowcol.
        for (auto k = FeasibleStates_FinalOrder.begin(); k != FeasibleStates_FinalOrder.end(); k++) {
           

            
            std::vector<bool> Chosen_State = *k;
          
           //Update all information regarding partition_sizes and Lowerbounds.
            std::tuple<std::vector<int>, std::vector<int>, std::vector<std::vector<int>>, int, std::vector<std::pair<int, std::vector<bool>>>, 
                std::array<std::vector<std::vector<int>>, 2> ,int , std::vector<int>, Bi_Graph> updatecolor = Update(i,
                    *k, Info, The_States,Vrije_NZ, Partition_size, color_count,  lowerbound, Partial_Status_rowcols,Packing_Sets2, maxval_pm, value_partialstatus ,  bigraph);

            //This rowcol=rowcol i, is now assigned state Chosen_State.
            The_States[i] = Chosen_State;

        
            //variables for the tie() fucntion used to unapck the tuple returned by the Update function
            //tie() or get() wat is beter ToDo ??
            std::vector<int> new_Free_NZ;
            std::vector<int> new_Partsize;
            std::vector<std::vector<int>> new_colorcount;
            int new_lb;
            std::vector<std::pair<int, std::vector<bool>>> new_Partial_status;
           
            int new_maxval_pm;
            std::vector<int> new_val_partstatus;
            Bi_Graph New_bigrph(&(Info->M), &(Info->N));
            std::array<std::vector<std::vector<int>>, 2> new_Packing_sets2;
            
           

            tie(new_Free_NZ, new_Partsize, new_colorcount, new_lb, new_Partial_status,new_Packing_sets2, new_maxval_pm, new_val_partstatus, New_bigrph) = updatecolor;

            //Number of rowcols that still need to be assigne is now a-1.
            int b = a - 1;

            //Recurse, go to the next layer in the tree.
            Partition(The_States, Order_rows_columns, b, Info, new_Free_NZ, new_Partsize, new_colorcount,  new_lb, new_Partial_status,new_Packing_sets2, new_maxval_pm, new_val_partstatus, New_bigrph, Symm);
        }

    }
  
    return;
}


//Gives the number of (partial) partitions that were aborted, because LB>=UB.
int get_Aantal() {
    return Aant_aborted;
}

//This are 2 external variables that hold the best found  solution 
//and the corresponding value of the communciation volume until now.
std::vector<std::vector<bool>> Best_solution_sofar;
//int Lowest_cv_sofar;

//This function saves the best solution found sofar and the corresponding communication volume of this solution.
void Save_best_solution_sofar(std::vector<std::vector<bool>> The_States, int commvol ) {

    if (commvol < UB) {
         Best_solution_sofar = The_States;
         Lowest_cv_sofar = commvol;
    }
}
