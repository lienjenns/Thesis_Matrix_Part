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

//Gives the state of an unassigned nz , state hasnumber of zeros  equal to number of processors. 
 //So it depends on the number of processors.
std::vector<bool> Determine_Unassigned_State(int p) {
    std::vector<bool> Unassigned_State(p, 0);

    return Unassigned_State;
}



//Function that determines the order of the row and columns based on the number of nonzeros in a row/column

//moet als input krijgen uit matrix.cpp" perRow_Col"
std::vector<int> Determine_Order_row_columns(std::vector<int> number_nz) {

    //Vector in which order is stored.
    std::vector<int> Order__row_columns;

    int number_of_rows_columns = number_nz.size();
    for (int i = 0; i < number_of_rows_columns; i++) {

        int Maximal_Value=*std::max_element(number_nz.begin(), number_nz.end());

        
        if (Maximal_Value < 1) {            //Makes sure that rows/col with 0 nonzeros are not taken into account.
            break;
        }

        int Index_MaxElement = std::max_element(number_nz.begin(), number_nz.end()) - number_nz.begin();

        Order__row_columns.push_back(Index_MaxElement);
        number_nz[Index_MaxElement] = -1;


    }
    //Prints the order of the row and columns, order is on basis of number of nz in row/column.
    //Row/column with the most nz is first.
    std::cout << "Volgorde rijen & kolommen";
    for (int j = 0; j < Order__row_columns.size(); j++) {
        std::cout << Order__row_columns[j] << "  ";
    }
    return Order__row_columns;
}






std::set<std::vector<bool>> Possible_States_meerFreeNZ(std::vector<int> freeindices, std::vector<bool> State, int number_NZ, std::set<std::vector<bool>> Begin) {
    //maak verzameling aan waar je alle possible states in op gaat slaan
    //std::set<std::vector<bool>> All_States;
   // All_States = Begin;
    //Is dit nodig?
    int a = number_NZ;

    if (a == 0 || freeindices.size() == 0) {
       // std::cout << "size of set: " << Begin.size() << "\n";
        //Dit print de set van possible states; Dubbele loop nodig en auto ipv int j: j ++
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
        //aantal processoren die nog niet gebruikt
        int nm_free_indices = freeindices.size();

        std::vector<int> New_freeProcessors = freeindices;
        std::set<std::vector<bool>> Result;
        Result = Begin;
        for (int i = 0; i < nm_free_indices; i++) {

            std::vector<bool> State_New = State;


            //Poss state maken verander een 0 in een 1
            int index = freeindices[i];
            State_New[index] = 1;        //maakt de mogelijke state, kan ipv 1 ook true gebruiken
            std::set<std::vector<bool>> All_States = Begin;
            All_States.insert(State_New); //slaat mogelijke state op in set

            //Aanpassen van variabelen voor recursie
            //a -= 1;
            New_freeProcessors.erase(New_freeProcessors.begin());



            //recursie
            std::set<std::vector<bool>> result = Possible_States_meerFreeNZ(New_freeProcessors, State_New, a - 1, All_States);
            for (auto l = result.begin(); l != result.end(); l++) {

                Result.insert(*l);
            }

        }

        return Result;
    }

}


//std::vector<bool> State1, std::vector<bool> State2
std::set<std::vector<bool>> Assigned_States(std::vector<std::vector<bool>> Set_States_nz, std::vector<bool> Unassigned_State, int Size_of_Set) {

    //This will be our container with all correct states for row/column, 
    //if we only look at the already assigned nz in a row/column. 
    std::set<std::vector<bool>> AllPossible_States_assignedNZ;


    //std::set<std::vector<bool>> AllPossibilities; 

    std::vector<bool> Basic_State;
    Basic_State = Unassigned_State;
    std::vector<bool> B = Unassigned_State;
    int b = Size_of_Set;

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

            // std::cout <<"laag in tree is waarde b"<< b<<" Zover zijn we in for loop; "<< i<< "\n ";

            int A = Indices_set[i];
            Basic_State[A] = 1;
            std::vector<bool> Basic_State2 = Basic_State;
            Basic_State = B;

            std::set<std::vector<bool>> result = Assigned_States(NewSet_States_of_nz, Basic_State2, b - 1);


            for (auto l = result.begin(); l != result.end(); l++) {

                AllPossible_States_assignedNZ.insert(*l);
            }



        }
        // std::cout << "tot hier einde else";
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

std::set<std::vector<bool>> Possible_States(std::vector<std::vector<bool>> States_NZs, int No_Free_NZ) {


    //The container of all possible states for the chosen row/column
    std::set<std::vector<bool>> Feasible_States;

    //Hier nog functie om (0,...,0) vectoren uit States_Nzs te verwidjeren.

    int No_Assigned_NZ = States_NZs.size();
    std::vector<bool> Unassigned_State(Processors, 0); //Makes the Unassigned State (0, ..., 0)
    std::set<std::vector<bool>> Outcome_Assigned;
    Outcome_Assigned = Assigned_States(States_NZs, Unassigned_State, No_Assigned_NZ);


    //All the states in the outcome set of assigend_States are feasible for this row/column
    //So insert in container
    for (auto l = Outcome_Assigned.begin(); l != Outcome_Assigned.end(); l++) {

        Feasible_States.insert(*l);
    }


    for (auto i = Outcome_Assigned.begin(); i != Outcome_Assigned.end(); i++) {

        //This info is needed for the function Possible_States_meerFreeNZ
        std::vector<bool> view_State = *i;
        std::vector<int> ZeroIndices;
        ZeroIndices = Determine_indices_zeros(view_State);

        std::set<std::vector<bool>> StartSet;

        std::set<std::vector<bool>> Outcome_Unass;
        Outcome_Unass = Possible_States_meerFreeNZ(ZeroIndices, view_State, No_Free_NZ, StartSet);

        //Outcomesin set of Possible_States_meerFreenz are other n(new) feasible states for this row/column
        //So add them to the container
        for (auto k = Outcome_Unass.begin(); k != Outcome_Unass.end(); k++) {

            Feasible_States.insert(*k);
        }

    }
    std::vector<bool> NoProcessor(Processors, 0);
    Feasible_States.erase(NoProcessor);
    //std::cout << Feasible_States.size();

  
    

    return Feasible_States;
}


//DE eerste lower bound bepaald in de partitite die we tot nu toe hebben heoveel cuts er in totaal zijn
//Hoeveel comm vol er nu al is.
//Deze functie gaat nu toch elke keer door hele vector dus rij/kolom die nog niet toegedeeld zijn moete op 000 worden gezet.
//Uiteindelijk mooier om t getal te onthouden en elke keer als rij/kolom wordt ingedeeld dan alleen kijken naar die rij/kolom en lower bound daar (evt) mee te verhogen.
//

//
int LowerBound1(std::vector < std::vector<bool>>The_Partition) {

    int size = The_Partition.size();
    int Number_of_cuts = 0;

    for (int i = 0; i < size; i++) {

        std::vector<bool> State_i;
        State_i = The_Partition[i];
        int Ones_State_i = std::count(State_i.begin(), State_i.end(), true);
        int Cuts_row_column_i = Ones_State_i - 1;


        //omadat je ook 000 kan hebben als nog niet teogedeeld krijg je  ook -1 cuts per rij voor niet gepartitioneerde rijen/kolommen
        //dus vandaar alleen tellen als de cuts in je rij/kolom >=0
        if (Cuts_row_column_i >= 0) {
            Number_of_cuts += Cuts_row_column_i;

        }
    }
    //std::cout << "\n" << "Lower bound 1 is;" << Number_of_cuts;
    return Number_of_cuts;




}

int LB1( std::vector<bool>State_rowcol) {

    int Cuts_rowcol = 0;

    //Determines the number of processors tow which the rowcol is assigned, == number of ones in state_rowcol.
  int Ones_State_rowcol = std::count(State_rowcol.begin(), State_rowcol.end(), true);

  //Determine the number of cuts in the state that us assigned to the rowcol.
  Cuts_rowcol = Ones_State_rowcol - 1;

  //Because we look at a state that is assigned to a rowcol, the state will contain at least one 1 so always cuts_rowcol>=0.
  //This in contrast to cuts_row_column_i in the LowerBound1 fucntion, in that function cuts_row_column_i  can be -1.
  //So in this case we can just return Cuts_rowcol.
     

    return Cuts_rowcol;

}



int Check1(std::vector<bool> Poss_Status_rc, int rowcol, std::vector<bool> color, std::vector<std::vector<int>> color_count, int no_New_C) {

    for (auto k = AllStates.begin(); k != AllStates.end(); k++) {


        if ((*k && color) == *k) {

            no_New_C -= color_count[Binair_index(*k)][rowcol]; //Ging hier mis bij (1,1,1) heeft namelijk binair index 7 maar color_count bestaat maar uit 6 vectoren, voor (1,1,1) hoef je ook niks te checken

        }
        else {
            continue;
        }

    }

    return no_New_C;

}

int Check2(std::vector<bool> Possible_Status_rc, int rowcol, std::vector<bool> C, std::vector<std::vector<int>> color_count, int no_New_C) {

    //
    for (auto k = AllStates.begin(); k != AllStates.end(); k++) {

        if ((((*k && Possible_Status_rc) || C) == C) && ((C && *k) != *k)) {///Nu neem je alleen nog wel C zelf ook mee en dat moet juist niet
            no_New_C += color_count[Binair_index(*k)][rowcol];  //Gaat goed voor p=3 maar niet voor p=4  of eigenlojk gaat niet goed voor C met 2 eneen en deze situatie komt alleen voor bij p=4

        }


        else {
            continue;
        }
    }
    //

    return no_New_C;

}



bool Check_Load_Bal(std::vector<bool> Possible_Status_rc, int rowcol, std::vector<int> Partition_size, std::vector<std::vector<int>> color_count, matrix B) {

    bool The_Check = 1;
    std::vector<bool> zero_state(Processors, 0);
   
  


    //Dit moet in code allemaal in andere plek worden gemaakt en goed toegankelijk zijn
   
  
   

    int no_Proc = Determine_Set_indices(Possible_Status_rc).size();

  

    if (no_Proc == 1) {

       // std::cout << "check for one proc";
        int no_new_S = B.perRow_Col[rowcol] - color_count[Binair_index(Possible_Status_rc)][rowcol];

        //Mag deze state?
        //Check the partition size of the possible_state S for the rowcol.
        if (Partition_size[Binair_index(Possible_Status_rc)] + no_new_S <= Max_Partition_size) {


            The_Check = 1;
           // std::cout << "HEt mag";

        }

        else {
            The_Check = 0;
            return The_Check;
           // std::cout << "Nope";
        }

        //Here we will check the partition sizes of the states C s.t C&&S=S and C!=S.
        //So C will have more ones than S if C&&S=S.

        for (auto l = AllStates.begin(); l != AllStates.end(); l++) {

            std::vector<bool> C = *l;
            if ((C && Possible_Status_rc) == Possible_Status_rc) { //Hier nog zorgen dat ie niet ook weeer S afgaat

                int no_New_C = B.perRow_Col[rowcol];

                no_New_C = Check1(Possible_Status_rc, rowcol, C, color_count, no_New_C);

                int x = Determine_Set_indices(C).size();

                //&& dus The_Check alleen 1 als beide 1 zijn 
                //Je wil loop nog stoppen als the check tussendoor 0 wordt of start check met 1 en dan 0 maken als het niet mag ToDo.
                The_Check = The_Check && (Partition_size[Binair_index(C)] + no_New_C <= x * Max_Partition_size);
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

    else if (no_Proc == Processors) {

       // std::cout << "Nothing to check";
        return The_Check;
    }
    else {
      //  std::cout << "two checks needed";
        for (auto l = AllStates.begin(); l != AllStates.end(); l++) {
            std::vector<bool> C = *l;

            if ((C && Possible_Status_rc) == Possible_Status_rc) { //Hier nog zorgen dat ie niet ook weeer S afgaat

                int no_New_C = B.perRow_Col[rowcol];
                //
                no_New_C = Check1(Possible_Status_rc, rowcol, C,  color_count, no_New_C);
                //
                int x = Determine_Set_indices(C).size();

                //&& dus The_Check alleen 1 als beide 1 zijn 
                //Je wil loop nog stoppen als the check tussendoor 0 wordt of start check met 1 en dan 0 maken als het niet mag ToDo.
                The_Check = The_Check && (Partition_size[Binair_index(C)] + no_New_C <= x * Max_Partition_size);

            }

            else if ((C && Possible_Status_rc) == C) {

                int no_newC = 0;

                no_newC = Check2(Possible_Status_rc, rowcol, C,  color_count, no_newC);


                int x = Determine_Set_indices(C).size();
                The_Check = The_Check && (Partition_size[Binair_index(C)] + no_newC <= x * Max_Partition_size);
            }


            else if ((C && Possible_Status_rc) == zero_state) {
                continue;
            }


            else {
                int no_newC = 0;

                no_newC = Check2(Possible_Status_rc, rowcol, C, color_count, no_newC);

                int x = Determine_Set_indices(C).size();
                The_Check = The_Check && (Partition_size[Binair_index(C)] + no_newC <= x * Max_Partition_size);
            }

            if (The_Check == 0) {
                return The_Check;
            }
        }


        return The_Check;

    }

}

 //This fucntion updates the Partition_size, the color_count for each rowcol, and the lowerbound.
std::tuple<std::vector<int>, std::vector<std::vector<int>>, int> Update(int rowcol, std::vector<bool> Status_rowcol, matrix A, std::vector<std::vector<bool>> The_States, std::vector<int> Partition_size, std::vector<std::vector<int>> color_count,
    int LB) {

    int increase_LB1 = LB1(Status_rowcol);
    LB += increase_LB1;
   // std::cout << " LB1: " << LB;
    std::vector<int> Intersecting_rowcol = A.Intersecting_RowCol(rowcol);

    for (auto i = Intersecting_rowcol.begin(); i != Intersecting_rowcol.end(); i++) {

        int index_itersect_rc = *i;

        std::vector<bool> State_intersect_rc = The_States[index_itersect_rc];

        //Je hoeft niks te updten als nieuwe states allproc state kan dit meer aanbegin dat neit for lop in?? ToDo
        if (Status_rowcol == AllProc_State) {
            continue;
        }

        //MAke the new_state of the intersecting NZ
        std::vector<bool> New_State_intersect_NZ;
        if (State_intersect_rc != Zero_State) {
            New_State_intersect_NZ = Status_rowcol && State_intersect_rc;

        }
        else {
            New_State_intersect_NZ = Status_rowcol;
            //Hier update free nz??
        }

        //If the state of the nz where the row and col intersect doesn't change nothing needs to be updated
        if (New_State_intersect_NZ == State_intersect_rc) {
            continue;
        }

        else {

            int index_of_newStatus = Binair_index(New_State_intersect_NZ);

            color_count[index_of_newStatus][index_itersect_rc] += 1;
            color_count[index_of_newStatus][rowcol] += 1;
            if ((State_intersect_rc != Zero_State) && (State_intersect_rc != AllProc_State)) {
                color_count[Binair_index(State_intersect_rc)][index_itersect_rc] -= 1;
                color_count[Binair_index(State_intersect_rc)][rowcol] -= 1;
            }

            // Partition_size[index_of_newStatus] += 1;

            for (auto j = AllStates.begin(); j != AllStates.end(); j++) { //Moet tweede conditie wl in if statement doe je er al wat mee maar dat voorkomy hier niet dat je m toch niet afgaat in allstates denk ik ToDo

                if (((New_State_intersect_NZ && *j) == New_State_intersect_NZ) && (*j != State_intersect_rc)) {

                    int index_St = Binair_index(*j);

                    Partition_size[index_St] += 1;

                }

            }


        }

    }


    return std::make_tuple(Partition_size, color_count, LB);


}



//Other UB this is a very large UB better option? ToDo
//int UB = (Processors-1)*std::min(ab.N, ab.M)-(Processors-1);

//ToDo the first ub now fixed must be flexible.
int UB = 100;

//TREE

//De tree gaat eerst helemaal een zijtak af
int AAnt = 0;
std::vector<std::vector<bool>> Partition(std::vector<std::vector<bool>> The_States, std::vector<int> Order_rows_columns, int a, matrix Info, std::vector<int> Vrije_NZ, std::vector<int> Partition_size, std::vector<std::vector<int>> color_count,
    int lowerbound) {

    //If it takes too long push escape button, then this funtion ends and the best solution so far is printed
    //27 is the escape toets
    if (GetKeyState(27) & 0x8000) {

        Stop_Partition = 1;
    }
    if (Stop_Partition) {
        return The_States;
    }


    if (lowerbound >= UB) {

        return The_States;
    }

    std::vector<int> Vector_No_Freenz = Vrije_NZ;
    

    int Layers_Tree =The_States.size();
    
    if (a == 0) {
      
        
        int Comm_vol = LowerBound1(The_States);
       // New_Upperbound(Comm_vol);
        if (Comm_vol <= UB) {
            Save_best_solution_sofar(The_States, LowerBound1(The_States));
            
            //printen van uiteindelijke partitie
            for (int j = 0; j < Layers_Tree; j++) {
                std::vector<bool> Stateof_j = The_States[j];

                /* std::vector<bool> unassign(Processors, 0);
                 if (Stateof_j = unassign) {
                     std::cout << " x ";
                 }*/
                std::cout << "  ";
                for (int l = 0; l < Processors; l++) {

                    std::cout << Stateof_j[l];
                }

            }
            // Determine & print communication volume of the partition
            std::cout << " CV: " << LowerBound1(The_States) << " ";
            std::cout << ": " << AAnt << "   ";   
            std::cout << "\n";
            UB = Comm_vol;
        }

        //Print partition size
       /* std::cout << "The part sizes: ";
        for (int i = 0; i < Partition_size.size(); i++) {
            std::cout << Partition_size[i] << " ";


        }
        std::cout << "tot nz dmv part_size ";

        std::cout << std::accumulate(Partition_size.begin(), Partition_size.end(), 0) - 2 * (Partition_size[0] + Partition_size[1] + Partition_size[3]); //Dit is de formule voor p=3 moet nog dynamisch*/
     
        AAnt++;
        if (AAnt % 100000 == 0) {
            std::cout << AAnt<<"\n";
        }
    }
    else {
        
        int y = Order_rows_columns.size();


        int i = Order_rows_columns[y-a];

          std::vector<int> IntersectingRowCol= Info.Intersecting_RowCol(i); //Determine the indices of the intersecting rows/columns with the column/row you are now going to assigm.
        
        //Prints the indices of the intersecting rows/columns
       /*   std::cout << "Huidige indeex: " << i << " intersecting rij/kolom :" << "\n";
          for (int k = 0; k < IntersectingRowCol.size(); k++) {
              std::cout << IntersectingRowCol[k] << " ";

          }
          std::cout << "\n";*/

        int no_Free_nzi = Vector_No_Freenz[i]; //Dit moet worden geupdate
        //   std::cout << "aantal vrije nz: " << no_Free_nzi;
        std::vector<int> New_vector_FreeNZ = Update_NO_Freenz(Vector_No_Freenz, IntersectingRowCol);


        /// Dit geeft de correcte states vn intersecing rows/columns, maakt er een vectir van
        /// //om mee te geven aan Possible_States
        std::vector<std::vector<bool>> Empty;
        for (int h = 0; h < IntersectingRowCol.size(); h++) {
            int index_Intersect_RowCol = IntersectingRowCol[h];
            std::vector<bool> State_Intersect_ROWCOl = The_States[index_Intersect_RowCol];

            //Only take into account the state of the intersecting rows/columns that are asssigned.
            if (accumulate(State_Intersect_ROWCOl.begin(), State_Intersect_ROWCOl.end(), 0) != 0) {

                Empty.emplace_back(The_States[index_Intersect_RowCol]);

            }

        }

        std::set<std::vector<bool>> FeasibleStates;
        FeasibleStates = Possible_States(Empty, no_Free_nzi);
        int No_FeasibleStates = FeasibleStates.size();

        // std::cout<< "Aantal Feas states" << No_FeasibleStates;
        // std::cout << "\n";

        std::set<std::vector<bool>> FeasibleStates_Symm;

        //Include the first symmetry step
        if (a == y) {
            FeasibleStates=First_Symmetry_Check(First_Symmetry_Set(), FeasibleStates);

        }

        std::set<std::vector<bool>> FeasibleStates_Final;

        for (auto l = FeasibleStates.begin(); l != FeasibleStates.end(); l++) {

            std::vector<bool> State_to_Check = *l;

           bool Checkvalue= Check_Load_Bal(State_to_Check, i, Partition_size, color_count, Info);

           if (Checkvalue == 1) {

               FeasibleStates_Final.insert(State_to_Check);
           }

           else { 
               continue; }

        }



       // Order_rows_columns.erase(Order_rows_columns.begin());
       //ToDo if Feasible_Final==0 meteen returen als ie stuk zou gaan als  for stuk gaat als feasible_Final stuk leeg is .

        for (auto k = FeasibleStates_Final.begin(); k != FeasibleStates_Final.end(); k++) {


            std::vector<bool> Chosen_State = *k;

           
            std::tuple<std::vector<int>, std::vector<std::vector<int>>, int> updatecolor = Update(i, *k, Info, The_States, Partition_size, color_count,  lowerbound);
            The_States[i] = Chosen_State;
        
            //variables for the tie() fucntion used to unapck the tuple, tie() or get() wat is beter ToDo ??
            std::vector<int> new_Partsize;
            std::vector<std::vector<int>> new_colorcount;
            int new_lb;

            tie(new_Partsize, new_colorcount, new_lb) = updatecolor;

            int b = a - 1;
            Partition(The_States, Order_rows_columns, b, Info, New_vector_FreeNZ, new_Partsize, new_colorcount,  new_lb);
        }



    }
  
    return The_States;
}


//Geeft totaal aantal partities.
int get_Aantal() {
    return AAnt;
}

//Initialize the external variables here in cpp
std::vector<std::vector<bool>> Best_solution_sofar;
int Lowest_cv_sofar;

//This function saves the best solution found sofar and the corresponding communication volume of this solution.
void Save_best_solution_sofar(std::vector<std::vector<bool>> The_States, int commvol ) {

    if (commvol < UB) {
         Best_solution_sofar = The_States;
         Lowest_cv_sofar = commvol;
    }
}



std::vector<int> Update_NO_Freenz(std::vector<int> No_Freenz_perRowCol, std::vector<int> Indices_intersectRowCol) {

    std::vector<int> New_No_Freenz = No_Freenz_perRowCol;

    int No_intersecting = Indices_intersectRowCol.size();

    for (int i = 0; i < No_intersecting; i++) {

        int  Index_RowCol = Indices_intersectRowCol[i];
        New_No_Freenz[Index_RowCol] -= 1;


    }

    return New_No_Freenz;
}

