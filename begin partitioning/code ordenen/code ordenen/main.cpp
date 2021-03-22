// code ordenen.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "./input.h"
#include "./matrix.h"
#include "./Possible_States.h"
#include <iostream>
#include<vector>
#include "./Global.h"
#include<numeric>
#include<Windows.h>
#include<time.h>
#include <iomanip>  
#include"./Util.h"
#include<utility>

int Processors=4;
double Epsilon=0.03;
std::set<std::vector<bool>> AllStates;

std::vector<bool> Zero_State(Processors,0);
std::vector<bool> AllProc_State(Processors, 1);

int Max_Partition_size;

bool Stop_Partition = 0;

std::vector <std::vector<bool>>Index_and_Status;

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

int Load_Balance(int nnz) {
    int Max_part_size;
    Max_part_size = (1 + Epsilon) * ceil((float)nnz / (float)Processors); //gaat goed rond t naar beneden af ook als bv 4.8 dan max parititon size 4 moet k nog floor en dan int?

    return Max_part_size;
}

std::vector <std::vector<bool>> indexStatus_vs_Status() {
    std::vector<std::vector<bool>> Index_Status;

    int x = pow(2, Processors) - 2;
    Index_Status.resize(x); //Moet naar options resize dus 2^p-2
   

    for (auto i = AllStates.begin(); i != AllStates.end(); i++) {
        int index = Binair_index(*i);

        Index_Status[index] = *i;

    }

    return Index_Status;

}

int main()
{ 
    clock_t start, end;
    start = clock();

    //Give the number of Processsors
  //Prints the number of processors
    std::cout << "Number of Processors: " << Processors << "\n";

    //Give the value of Epsilon 3% load imbalance means Epsilon=0.03.
   //Prints the chosen value of epsilon.
    std::cout <<"Value epsilon : "<< Epsilon<< "\n";

   //Here the set of all posibble states without the "all processors state =(1,1,1, ...,1)" is made,
    AllStates = States(Processors, 0, Zero_State, Processors);

    //Here the vector of binairy indices with there status is made.
    Index_and_Status = indexStatus_vs_Status();
   
    //Give the name of the mtx of txt file
    matrix A(Read_From_File("GL7d10.mtx"));


    A.Initial_nzstate_vector();//ToDo dit geval dexe fucntie gebruik ik volgens mij niet.
    Max_Partition_size = Load_Balance(A.nnz);
    std::cout << "Max partition size: " << Max_Partition_size << "\n";

    //matrix B();
    //A en B 2 verschillende objecten.
    //Maar als je zegt matrix B=A dan heb je 1 object , A is je object en B is je pointer  

    
   int b=A.Determine_Cmax();
   
 //  std::cout << "\n" <<"The CMax is:"<< b;
   int max_size = Load_Balance(A.nnz);

   //std::cout << " Max partition size: " << max_size;

   std::vector<int> c;
   c=Determine_Order_row_columns(A.perRow_Col);
    
   int d = c.size();
   std::vector<bool > bs(Processors,0);
   std::vector<std::vector<bool>> TheState(A.M+A.N,bs);
  
   //Voro bijhouden kleuren moest dit mee in partition
   int options = pow(2, Processors) - 2;
   std::vector<int> Partition_size(options, 0);



   std::vector<std::vector<int>> color_count;

   //Hier wordt color_count gemaakt ToDo moet netter cpp
       for (int i = 0; i < options; i++) {

           std::vector<int> a(A.M + A.N, 0);
           color_count.push_back(a);

       }

   //Initial container values for L2bound, 
  std::vector<bool> initial_Partstat(options, 0);
  std::vector<std::pair<int, std::vector<bool>>>  Partial_Status_rowcols((A.M + A.N), std::make_pair(0, initial_Partstat));
 

       

   std::vector<std::vector<bool>> Partitie;
   Partitie = Partition(TheState, c, d,A, A.perRow_Col, Partition_size, color_count, 0, Partial_Status_rowcols);
  
   std::cout << "Number of Partitions aborted: " << get_Aantal();

   end = clock();
   std::cout << "\n";

   //Determine execution time and print it
   double time_taken = double(end - start) / double(CLOCKS_PER_SEC);

   std::cout << "\n" << "Time taken: " << std::setprecision(5)<< time_taken << "sec";

   //Prints the best solution that is found so far and the corresponding communicaion volume.
   std::cout <<"\n"<< "The best solution found so far is:  ";
   for (int i = 0; i < Best_solution_sofar.size(); i++) {
       for (int j = 0; j < Processors; j++) {

           std::cout << Best_solution_sofar[i][j];
       }

       std::cout << " ";
   }
   std::cout <<" With Comm. Vol:  "<< Lowest_cv_sofar<<"\n";
}

