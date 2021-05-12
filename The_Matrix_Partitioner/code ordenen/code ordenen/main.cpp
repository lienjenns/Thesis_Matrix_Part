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
#include"./Lower_bounds.h"
#include<array>


//Initialize some of the exertnal variables of Global.h.

//Give the number of Processsors
int Processors = 3;

//Give the value of Epsilon 3% load imbalance means Epsilon=0.03
double Epsilon=0.03;

//Initial value for some other external variables of Global.h
std::vector<bool> Zero_State(Processors, 0);
std::vector<bool> AllProc_State(Processors, 1);
bool Stop_Partition = 0;

//Define some of the exetrnal variables of Global.h.
std::set<std::vector<bool>> AllStates;
int Max_Partition_size;
std::vector <std::vector<bool>>Index_and_Status;



int main()
{ 
    clock_t start, end;
    start = clock();

     //Prints the number of processors and the chosen value of Epsilon.
    std::cout << "Number of Processors: " << Processors <<"  Value epsilon : "<< Epsilon<< "\n";

    //Give the name of the mtx or txt file
    matrix A(Read_From_File("0-30matrix/cage4.mtx"));



 //Now make/determine some variables for the "Partition" function = The Tree:
   

   //Here the set of all posibble states without the "all processors state =(1,1,1, ...,1)" is made,
    AllStates = States( 0, Zero_State, Processors);

    //Here the vector of binairy indices with there status is made.
    Index_and_Status = indexStatus_vs_Status();
   
    //Call the function that makes the basic info for the L3 bound.
    Basic_L3_info();

    //Here the Maximal partition size is determined.
    Max_Partition_size = Load_Balance(A.nnz);
    std::cout << "Max partition size: " << Max_Partition_size << "\n";

   int b=A.Determine_Cmax();
   
   //Below the order of the rows and columns is determined:
   std::vector<int> c;
   c=Determine_Order_row_columns(A.perRow_Col);

   std::vector<int> order2;
   order2 = Determine_Order_row_columns2(A.perRow_Col, A.M);
   

   //The length of the vector that contans the order of rowcols is needed for the "Partition"function.
   //Note this length can be smaller than m+n because some rowcols may not contain any nonzeros.
   int d = c.size();
   int size_order2 = order2.size();


   //The starting "Partition" is made.
   //Every rowcol is not yet assigned a status so the state of every rowcol is (0,..0,).
   std::vector<bool > bs(Processors,0);
   std::vector<std::vector<bool>> TheState(A.M+A.N,bs);
  
   //Make the container that keeps track of the partition sizes.
   int options = pow(2, Processors) - 2;
   std::vector<int> Partition_size(options, 0);

   //Make the container that keeps track of the "colours" per rowcol
   std::vector<int> a(A.M + A.N, 0);
   std::vector<std::vector<int>> color_count(options,a);

  //Initial container values for L2bound, 
  std::vector<bool> initial_Partstat(options, 0);
  std::vector<std::pair<int, std::vector<bool>>>  Partial_Status_rowcols((A.M + A.N), std::make_pair(0, initial_Partstat));  


  //Initialize packing sets for L3 bound.
  std::vector<int> Rows(A.M, 0);
  std::vector<int> Cols(A.N, 0);
  std::vector < std::vector<int>>PAck_row(Processors, Rows);
  std::vector < std::vector<int>>PAck_col(Processors, Cols);
  std::array<std::vector<std::vector<int>>, 2> Packing_Sets = { PAck_row, PAck_col };

  //Initialize LB3bound at zero
  int LB3_First = 0;

  //Give the "Partition" function = the tree all the information it needs and execute it.
   Partition(TheState, order2, size_order2,A, A.perRow_Col, Partition_size, color_count, 0, Partial_Status_rowcols, Packing_Sets, LB3_First);

   //When the partition function is executed the number of partial partitions that where aborted,
   //because of LB>=UB is printed.
   std::cout << "Number of Partitions aborted: " << get_Aantal();

   end = clock();
   std::cout << "\n";

   //Determine execution time and print it
   double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
   std::cout << "\n" << "Time taken: " << std::setprecision(5)<< time_taken << "sec";

   //Prints the best solution that is found so far and the corresponding communication volume.
   std::cout <<"\n"<< "The best solution found so far is:  ";
   for (int i = 0; i < Best_solution_sofar.size(); i++) {
       for (int j = 0; j < Processors; j++) {

           std::cout << Best_solution_sofar[i][j];
       }

       std::cout << " ";
   }
   std::cout <<" With Comm. Vol:  "<< Lowest_cv_sofar<<"\n";
}

