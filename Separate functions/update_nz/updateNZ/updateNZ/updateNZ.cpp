// updateNZ.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<algorithm>
#include<vector>
#include <math.h>
#include<set>
#include<numeric>

int Processors = 4;
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

std::vector<int> Gedoe(std::vector<int> UpdateNZ, std::vector<int> index_over, std::vector<std::vector<bool>> Stat_restNZ) {
   // int Processors = 3;                  //ToDo p moet naar Processors
    std::vector<int> No_owned(Processors, 0);

    for (int i = 0; i < UpdateNZ.size(); i++) {

        int Nz_no = UpdateNZ[i];
        std::vector<bool> State_Nz = Stat_restNZ[Nz_no];

        for (int j = 0; j < index_over.size(); j++) {

            int Proc_no = index_over[j];

            if (State_Nz[Proc_no] != 0) {

                No_owned[Proc_no] += 1;
            }


        }



    }

    return No_owned;

}


std::vector<std::vector<bool>> Ellende(std::vector<int> no_ownedproc, std::vector<int> update_nz, std::vector<int> Index_over, std::vector<std::vector<bool>> Status_restNZ) {
    //int p = 3; //ToDo moet naar processors
    std::vector<std::vector<bool>> Status_otherNZ;
    bool control = 0;
    std::vector<bool> zero(Processors, 0); //ToDo p moet naar processors
    std::vector<int> New_no_ownedproc;
    for (int i = 0; i < Processors; i++) {  // i is number of processor your looking at

        if (no_ownedproc[i] == 1) {

            control = 1;

            for (int j = 0; j < update_nz.size(); j++) {

                int no_regard_nz = update_nz[j];             //no_regard_nz is the number of the nonzero your looking at.

                std::vector<bool> state_regard_nz = Status_restNZ[no_regard_nz];

                if (state_regard_nz[i] == 1) {

                    std::vector<bool> newstate = zero;
                    newstate[i] = 1;

                    Status_restNZ[no_regard_nz] = newstate;
                    update_nz.erase(update_nz.begin() + j);

                    break;
                }


            }



            Index_over.erase(std::remove(Index_over.begin(), Index_over.end(), i), Index_over.end());
        }

        else {
            continue;
        }




    }
    if (control == 1) {

        New_no_ownedproc = Gedoe(update_nz, Index_over, Status_restNZ);
        
        Status_restNZ = Ellende(New_no_ownedproc, update_nz, Index_over, Status_restNZ);

    }

    else {
    }

    return Status_restNZ;

}


//std::vector<bool> Status_rijKolom
//Nog niks gedaan als antal enen even groot maar op adnere plekken dan ook issue met status nz

void Update(std::vector<bool> Old_NZStatus, std::vector<bool>New_NZStatus, std::vector<std::vector<bool>> Status_restNZ) {
  //  int p = 3;                  //ToDo p moet processor worden
    int M=0;
    std::vector<std::vector<bool>> New_Stat_othernz;

    std::vector<int> Set_New_Stat = Determine_Set_indices(New_NZStatus);
    std::vector<int> Set_Old_Stat = Determine_Set_indices(Old_NZStatus);

    int no_set_old = Set_Old_Stat.size();
    int no_set_new = Set_New_Stat.size();

    if (no_set_old == no_set_new) {

        std::cout << "Geen update Nodig";
    }

    //else if (no_set_old < no_set_new) { //Niet altjd zo als nog niet ingedeeld kan aantal enen wel omhoog gaan.

    //    std::cout << "Something went wrong with assigning a status to this NZ.";
    //}
    else {
        std::cout << "updaten!";

        std::vector<int> update_nz;

        for (int i = 0; i < no_set_new; i++) {

            int no_Processor = Set_New_Stat[i];
            // remove(Set_Old_Stat.begin(), Set_Old_Stat.end(), no_Processor);

            Set_Old_Stat.erase(std::remove(Set_Old_Stat.begin(), Set_Old_Stat.end(), no_Processor), Set_Old_Stat.end());

        }


        //Make the state S(Set_old_Stat), so if Set_Old_Stat={0,2}  S(Set_Old_Stat)=(1,0,1)^T.
        std::vector<bool> Check_Status(Processors, 0);                                   //ToDo p moet processor worden

        int no_Proc_to_check = Set_Old_Stat.size();
        for (int j = 0; j < no_Proc_to_check; j++) {

            int Processor_no = Set_Old_Stat[j];
            Check_Status[Processor_no] = 1;
        }

        //Prints state S(over)
        /*std::cout << "Status S(over) : ";
        for (int h = 0; h < p; h++) {
            std::cout << Check_Status[h];
        }

        std::cout << "\n";
        std::cout << "The intersect states S(over) && S(other NZ)";*/

        //Determines the intersect state = S(over) && S(other NZ).
        //And how many interscet_State != (0,0,0), this is indicated by the value of M.
        std::vector<bool> Intersect_State;

        for (int k = 0; k < Status_restNZ.size(); k++) {
            std::vector<bool> Stat_OtherNZ = Status_restNZ[k];


            Intersect_State = Check_Status && Stat_OtherNZ;

            //Prints intersect state.
           /* for (int l = 0; l < Intersect_State.size(); l++) {

                std::cout << Intersect_State[l];

            }*/
            // std::cout << "\n";


            int sum_intersect_St = std::accumulate(Intersect_State.begin(), Intersect_State.end(), 0);

            if (sum_intersect_St != 0) {

                update_nz.push_back(k);
                M++;
            }

        }

        //Prints the numbers of the nonzeros that need to be updated.
        for (int i = 0; i < update_nz.size(); i++) {
            std::cout << update_nz[i];
        }

        //Step 3 a; if number of processors to check is equal to the number of nonzeros that is owned by one of these processors
        //Then we need to update the status of these nonzeros.
        if (M == no_Proc_to_check) {

            std::cout << "\n" << "AAnpassen!";


            for (int i = 0; i < M; i++) {
                int k = update_nz[i];
                std::vector<bool>NewStat = Check_Status && Status_restNZ[k];
                Status_restNZ[k] = NewStat;
            }
        }


        //Stap 3 b

       std::vector<int> no_ownedproc;
      no_ownedproc= Gedoe(update_nz, Set_Old_Stat, Status_restNZ);

      std::cout <<"\n"<< "No of nz owned by a processor: ";
      for (int g = 0; g < no_ownedproc.size(); g++) {
          std::cout << no_ownedproc[g]<<" ";
      }
     
      std::cout << "\n";


      //void Ellende(std::vector<int> no_ownedproc, std::vector<int> update_nz, std::vector<int> Index_over, std::vector<std::vector<bool>> Status_restNZ) 
     
      New_Stat_othernz=Ellende(no_ownedproc,update_nz, Set_Old_Stat,Status_restNZ);

       //bool control = 0;
       //std::vector<bool> zero(Processors, 0); //ToDo p moet naar processors
       //for (int i = 0; i < Processors; i++) {  // i is number of processor your looking at

       //    if (no_ownedproc[i] == 1) {

       //        control = 1;

       //        for (int j = 0; j < update_nz.size(); j++) {

       //            int no_regard_nz = update_nz[j];             //no_regard_nz is the number of the nonzero your looking at.

       //            std::vector<bool> state_regard_nz = Status_restNZ[no_regard_nz]; //hier gaat iets mis

       //            if (state_regard_nz[i] == 1) {
       //                
       //                std::vector<bool> newstate = zero;
       //                newstate[i] = 1;

       //                Status_restNZ[no_regard_nz] = newstate;
       //                update_nz.erase(update_nz.begin() + j);

       //                break;
       //            }


       //        }


       //        //Set_Old_Stat.erase(Set_Old_Stat.begin() + i-1); // bevat alleen nog indexen die geckecked moeten worden dus als Set_old_State={} en p=3 zit er in gaat erasen mis
       //        Set_Old_Stat.erase(std::remove(Set_Old_Stat.begin(), Set_Old_Stat.end(), i), Set_Old_Stat.end());

       //    }

       //    else {
       //        continue;
       //    }

       //    


       //}
       //     if (control == 1) {
       //        Gedoe(update_nz, Set_Old_Stat, Status_restNZ);


       //    }
           

    }




    //Prints the status of the other nonzeros.
        for (int h = 0; h < New_Stat_othernz.size(); h++) {

            std::vector<bool> X = New_Stat_othernz[h];

            for (int y = 0; y < Processors; y++) {

                std::cout << X[y];
            }
            std::cout << "\n";
        }

        //Prints information
       /* std::cout << "Value M: " << M << "\n";
        std::cout << "\n";
        std::cout << "Size old" << Set_Old_Stat.size();
        for (int j = 0; j < Set_Old_Stat.size(); j++) {

            std::cout << Set_Old_Stat[j]<<" ";
        }*/

    }



    //void Ellende(std::vector<int> no_ownedproc, std::vector<int> update_nz,std::vector<int> Index_over, std::vector<std::vector<bool>> Status_restNZ) {
    //    //int p = 3; //ToDo moet naar processors
    //    bool control = 0;
    //    std::vector<bool> zero(Processors, 0); //ToDo p moet naar processors
    //    std::vector<int> New_no_ownedproc;
    //    for (int i = 0; i < Processors; i++) {  // i is number of processor your looking at

    //        if (no_ownedproc[i] == 1) {

    //            control = 1;

    //            for (int j = 0; j < update_nz.size(); j++) {

    //                int no_regard_nz = update_nz[j];             //no_regard_nz is the number of the nonzero your looking at.

    //                std::vector<bool> state_regard_nz = Status_restNZ[no_regard_nz];

    //                if (state_regard_nz[i] = 1) {

    //                    std::vector<bool> newstate = zero;
    //                    newstate[i] = 1;

    //                    Status_restNZ[no_regard_nz] = newstate;
    //                    update_nz.erase(update_nz.begin() + j);

    //                    break;
    //                }


    //            }

    //          

    //            Index_over.erase(std::remove(Index_over.begin(), Index_over.end(), i), Index_over.end());
    //        }

    //        else {
    //            continue;
    //        }




    //    }
    //    if (control == 1) {
    //       
    //        New_no_ownedproc= Gedoe(update_nz, Index_over, Status_restNZ);


    //    }

    //    Ellende( New_no_ownedproc ,  update_nz, Index_over, Status_restNZ);

    //}


int main()
{


    std::vector<bool> Old = { 1,1,1 ,1};
    std::vector<bool> New = { 0,1,0,0 };


    std::vector<bool> NZ1 = { 1,0,1,1 };
    std::vector<bool> NZ2 = { 1,0,1,0 };
    std::vector<bool> NZ3 = { 0,1,1,1 };
    std::vector<std::vector<bool>> Waarde_Rest_nz = { NZ1,NZ2,NZ3 };


   

    Update(Old, New, Waarde_Rest_nz);


  ////  for (int i = 0; i < 3; i++) {

  //  std::vector<bool > H= Waarde_Rest_nz[0];
  //  std::cout << H[0]<<"\n";
  //  for (int j = 0; j < 3; j++) {

  //          std::cout << H[j];
  //      }
  ////  }

    
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
