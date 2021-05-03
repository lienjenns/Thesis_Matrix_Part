// loadBal.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include<algorithm>
#include<vector>
#include <math.h>
#include<set>
#include<numeric>


int Processors = 3;

//This function determines the index of the state by using binary arithmetric .
//So index of state (0,1,1) is 2^1+2^2 -1=4. The minus one is in order to start at index 0.
int Binair_index(std::vector<bool> State) {
    int index=0;

    for (int i = 0; i < Processors; i++) {

        index += pow(2, i)*State[i];

    }
    index -= 1;

    return index;
}


//Function && operator for two vector<bool>. 
//For instance if a=[0,1,0] and b=[1,1,0], then the function returns a && b =[0,1,0]
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
//Deze functie maakt alle states die mogelijk zijn voor p processoren 
//int x wil je weg maar moet je ook doorgebn in recursie zou je a=p kunnen zeggen end an gewoon a-1 door gven?

//int g = 0;

std::set<std::vector<bool>> States(int Number_of_processors, int start, std::vector<bool> Different_States, int x) {

    std::set<std::vector<bool>> AllStates;

    int a = x;
    int p = Number_of_processors;
    int entry = start;

   
    if (a == 0) {
        AllStates.emplace(Different_States);
    //Pints the states when they are added to AllStates:
        /*std::cout << g << ":   " << "\n";
        for (int j = 0; j < p; j++) {
            std::cout << Different_States[j] << " ";
        }
        std::cout << "\n";
        g++;*/
    }
    else {


        for (int k = 0; k < 2; k++){

            Different_States[entry] = k;


            int b = a - 1;
            int entry=start+1;

           std::set<std::vector<bool>> New_States= States(p, entry, Different_States, b);


           for (auto l =New_States.begin(); l != New_States.end(); l++) {

               AllStates.emplace(*l);
           }

        }


    }

    std::vector<bool> NoProcessor(Processors, 0);
    AllStates.erase(NoProcessor);

    return AllStates;
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
    for (int j = 0; j < Set_Indices.size(); j++) {
        // std::cout << Set_Indices[j];
    }

    return Set_Indices;

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

int Check1(std::vector<bool> Poss_Status_rc, int rowcol,std::vector<bool> color, std::set<std::vector<bool>>allstates, std::vector<std::vector<int>> color_count, int no_New_C){

    for (auto k = allstates.begin(); k != allstates.end(); k++) {


        if ((*k && color) == *k) {

            no_New_C -= color_count[Binair_index(*k)][rowcol]; //Ging hier mis bij (1,1,1) heeft namelijk binair index 7 maar color_count bestaat maar uit 6 vectoren, voor (1,1,1) hoef je ook niks te checken

        }
        else {
            continue;
        }

    }

    return no_New_C;

}

int Check2(std::vector<bool> Possible_Status_rc, int rowcol, std::vector<bool> C, std::set<std::vector<bool>>allStates, std::vector<std::vector<int>> color_count, int no_New_C) {

    //
    for (auto k = allStates.begin(); k != allStates.end(); k++) {

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



bool Check_Load_Bal(std::vector<bool> Possible_Status_rc, int rowcol) {

    bool The_Check = 1;
    std::vector<bool> zero_state(Processors, 0);
    std::set<std::vector<bool>> allStates = States(Processors, 0, zero_state, Processors); //Hier niet aangemaat uit global file gehaald
    std::vector<bool> allProc(Processors, 1);
    allStates.erase(allProc);
    std::cout << "äll state size "<< allStates.size()<< "\n";
 

    //Dit moet in code allemaal in andere plek worden gemaakt en goed toegankelijk zijn
    int M = 5;
    int N = 5;
    int options = pow(2, Processors) - 2;
    std::vector<std::vector<int>> color_count;
    for (int i = 0; i < options; i++) {

        std::vector<int> a(M + N, 0);
        color_count.push_back(a);

    }

    int no_Proc = Determine_Set_indices(Possible_Status_rc).size();

   
    std::vector<int> Partition_size = { 1,3,4,0,4,6 }; ///Container van partitiesize moet (bijna) overal toegangkelijk
    std::vector<int> PEr_rowcol = { 2,3,2  }; //aantal NZ per rijcol is per_rowcol in code. Dus op dit moment even 3 rijen
    int Max_Partitionsize = 3;
 
    if (no_Proc == 1) {

        std::cout << "check for one proc";
        int no_new_S = PEr_rowcol[rowcol] - color_count[Binair_index(Possible_Status_rc)][rowcol];

        //Mag deze state?
        //Check the partition size of the possible_state S for the rowcol.
        if (Partition_size[Binair_index(Possible_Status_rc)] + no_new_S <= Max_Partitionsize) {


            The_Check = 1;
            std::cout << "HEt mag";

        }

        else {
            The_Check = 0;
            std::cout << "Nope";
        }

        //Here we will check the partition sizes of the states C s.t C&&S=S and C!=S.
        //So C will have more ones than S if C&&S=S.

        for (auto l = allStates.begin(); l != allStates.end(); l++) {
        
            std::vector<bool> C = *l;
            if ((C && Possible_Status_rc) == Possible_Status_rc) { //Hier nog zorgen dat ie niet ook weeer S afgaat

                int no_New_C = PEr_rowcol[rowcol];
            
                no_New_C = Check1(Possible_Status_rc, rowcol, C, allStates, color_count, no_New_C);

                int x = Determine_Set_indices(C).size();

                //&& dus The_Check alleen 1 als beide 1 zijn 
                //Je wil loop nog stoppen als the check tussendoor 0 wordt of start check met 1 en dan 0 maken als het niet mag ToDo.
               The_Check= The_Check && (Partition_size[Binair_index(C)] + no_New_C <= x*Max_Partitionsize);

            }

            else {
                continue;
            }
        
        }

        return The_Check;

    }

    else if (no_Proc == Processors) {

        std::cout << "Nothing to check";
        return The_Check;
    }
    else {
        std::cout << "two checks needed";
        for (auto l = allStates.begin(); l != allStates.end(); l++) {
            std::vector<bool> C = *l;

            if ((C && Possible_Status_rc) == Possible_Status_rc) { //Hier nog zorgen dat ie niet ook weeer S afgaat

                int no_New_C = PEr_rowcol[rowcol];
                //
                no_New_C = Check1(Possible_Status_rc, rowcol, C, allStates, color_count, no_New_C);
                //
                int x = Determine_Set_indices(C).size();

                //&& dus The_Check alleen 1 als beide 1 zijn 
                //Je wil loop nog stoppen als the check tussendoor 0 wordt of start check met 1 en dan 0 maken als het niet mag ToDo.
                The_Check = The_Check && (Partition_size[Binair_index(C)] + no_New_C <= x*Max_Partitionsize);

            }

            else if ((C && Possible_Status_rc) == C) {

                int no_newC = 0;

                no_newC = Check2(Possible_Status_rc, rowcol, C, allStates, color_count, no_newC);


                int x = Determine_Set_indices(C).size();
                The_Check=The_Check && (Partition_size[Binair_index(C)] + no_newC <= x* Max_Partitionsize);
            }


            else if ((C && Possible_Status_rc) == zero_state) {
                continue;
            }


            else {
                int no_newC = 0;
                
                no_newC = Check2(Possible_Status_rc, rowcol, C, allStates, color_count, no_newC);

                int x = Determine_Set_indices(C).size();
                The_Check = The_Check && (Partition_size[Binair_index(C)] + no_newC <= x * Max_Partitionsize);
            }
        }
      

        return The_Check;

    }

}



int main()
{

    //std::set<std::vector<bool>> AB=States(3, 0, {0,0,0}, 3);
    //std::cout << "size" << AB.size();

    //for (auto i = AB.begin(); i != AB.end();i++) {
    //    std::vector<bool> X = *i;

    //    for (int j = 0; j < X.size(); j++) {

    //        std::cout<<X[j];
    //    }
    //    std::cout << "\n";
    //}
    //
    //int M = 5;
    //int N = 5;
    //int options = pow(2,Processors)-2;
    
    //std::vector<int> color_count[6]; // ToDo moet nog dynamisch gemaakt worden 2^p-2 vectors moeten aangemaakt worden.

    //std::vector<std::vector<int>> color_count;

    //c+++ schuld dat array niet wil.
    /*std::array<std::vector<int>, x> v;
    std::vector<int> a[x];*/



   /* for (int i = 0; i < options; i++) {

        std::vector<int> a(M + N, 0);
        color_count.push_back(a);
        
    }
    std::cout << color_count.size();*/




   bool a= Check_Load_Bal({ 1,1,0 }, 2);



   std::cout << "uitkomst " << a;

}

