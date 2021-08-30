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
#include<fstream>
#include <string> 
#include"./output.h"
#include<math.h>


//Initialize some of the exertnal variables of Global.h.

//Give the number of Processsors
int Processors;

//Give the value of Epsilon 3% load imbalance means Epsilon=0.03
double Epsilon;

//Give the name of the matrix you want to partition.
std::string nameMatrix;
std::string filename_Sol_info;
std::string Location_matrix;


//Initial value for some other external variables of Global.h
std::vector<bool> Zero_State;
std::vector<bool> AllProc_State;
bool Stop_Partition = 0;

//Define some of the exetrnal variables of Global.h.
std::set<std::vector<bool>> AllStates;
int Max_Partition_size;
std::vector <std::vector<bool>>Index_and_Status;
int UB=1;
int Lowest_cv_sofar= -1;

//Define the outputstream and file name in order to store all info about the partitioning of this matrix.
std::ofstream Solution_and_info;



//Do we want to use the priority Queue, for p=3 / p=4.
bool PQ = 0;
//Do we want to use symmetry for the 2nd row/column?
bool s2 = 1;
//Do we want to combine L3 and L3 bound, and calculate the L3 after L4 is detemrined?
bool CombL3_L4 = 1;
//Do we want to use the Global L4 bound?
bool GL4_on = 1;
//Give the maximum length of path in BFS of global L4 bound
int length_path = 7;


int combolocal = 0;
int aantalGL4 = 0;
 int L4groter=0;
 int GL4groter=0;
 int L3gr=0;
 int gelijk = 0;
 int GL42 = 0;


int main(int argc, char* argv[])
{
    if (argc < 2) {
        nameMatrix = "Hamrle1";
        Processors = 2;
        Epsilon = 0.03;
        Location_matrix = "31-50matrix/" + nameMatrix + ".mtx";
    }
    else {

        nameMatrix = argv[1];
        Processors = std::stoi(argv[2]);
        Epsilon    = std::stod(argv[3]);
        Location_matrix = "matrix/" + nameMatrix + ".mtx";
    }
    filename_Sol_info = "p=" + std::to_string(Processors) + " " + nameMatrix + ".txt";

    std::cout << nameMatrix<<"\n";

    Zero_State = std::vector<bool>(Processors, 0);
    AllProc_State = std::vector<bool>(Processors, 1);

    clock_t start, end;
    start = clock();

    //Prints the number of processors and the chosen value of Epsilon.
    std::cout << "Number of Processors: " << Processors <<"  Value epsilon : "<< Epsilon<< "\n";

    //Give the name of the mtx or txt file ToDo (now enetr name in r.42)
  
    matrix A(Read_From_File(Location_matrix));

    matrix* pointA;
    pointA = &A;
  

    //File to store all information about one matirx
    //It stores all  new upperbounds, corresponding partitions  and new ub value
    Solution_and_info.open(filename_Sol_info, std::ios::out | std::ios::app);
    Solution_and_info << nameMatrix << "\n";
    Solution_and_info<<"\n"<< "Number of Processors: " << Processors << "  Value epsilon : " << Epsilon << "\n";
    

 //Now make/determine some variables for the "Partition" function = The Tree:
   
    //Determine the initial value of the UB.
   // init_UB(pointA);

   //Here the set of all posibble states without the "all processors state =(1,1,1, ...,1)" is made,
    AllStates = States( 0, Zero_State, Processors);

    //Here the vector of binairy indices with there status is made.
    Index_and_Status = indexStatus_vs_Status();
   
    //Call the function that makes the basic info for the L3 bound.
    Basic_L3_info();

    //Here the Maximal partition size is determined.
    Max_Partition_size = Load_Balance(pointA -> nnz);
    std::cout << "Max partition size: " << Max_Partition_size << "\n";
    Solution_and_info << "Max partition size: " << Max_Partition_size << "\n";

   int b=A.Determine_Cmax();
   
   //Below the order of the rows and columns is determined:
   std::vector<int> c;
   c=Determine_Order_row_columns(pointA -> perRow_Col);

   std::vector<int> order2;
   order2 = Determine_Order_row_columns2(pointA -> perRow_Col, pointA -> M);
   
   std::vector<int>order3=Determine_Order_row_columns3(pointA->perRow_Col, A);

   //The length of the vector that contans the order of rowcols is needed for the "Partition"function.
   //Note this length can be smaller than m+n because some rowcols may not contain any nonzeros.
   int d = c.size();
   int size_order2 = order2.size();
   int size_order3 = order3.size();


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
  std::vector<std::pair<int, std::vector<bool>>>  Partial_Status_rowcols((pointA -> M + pointA ->N), std::make_pair(0, initial_Partstat));  


  //Initialize packing sets for L3 bound.
  std::vector<int> Rows(pointA ->M, 0);
  std::vector<int> Cols(pointA -> N, 0);
  std::vector < std::vector<int>>PAck_row(Processors, Rows);
  std::vector < std::vector<int>>PAck_col(Processors, Cols);
  std::array<std::vector<std::vector<int>>, 2> Packing_Sets = { PAck_row, PAck_col };

  //initialize partial status
  std::vector<int>Value_Partial_status(pointA->M + pointA->N, -1);

  //Initialize LB3bound at zero
  int LB3_First = 0;


  //Initialize the bipartite graph necessary for the local L4 bound
  Bi_Graph graph(&A.M, &A.N);

  int count_rondes = 0;
  float UB_factor = 1.25;
  while (Lowest_cv_sofar == -1) {
      std::cout << "Nu in ronde: " << count_rondes << " UB: " << UB<< "\n";
      //Give the "Partition" function = the tree all the information it needs and execute it.
      Partition(TheState, order3, size_order3, &A, A.perRow_Col, Partition_size, color_count, 0, Partial_Status_rowcols, Packing_Sets, LB3_First, Value_Partial_status, graph);


      int new_UB = ceil(UB * UB_factor);
      UB = new_UB;
      count_rondes++;
  }

   //When the partition function is executed the number of partial partitions that where aborted,
   //because of LB>=UB is printed.
   std::cout << "Number of Partitions aborted: " << get_Aantal();
   Solution_and_info << "\n"<<"Number of Partitions aborted: " << get_Aantal();

   end = clock();
   std::cout << "\n";

   //Determine execution time and print it
   double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
   std::cout << "\n" << "Time taken: " << std::setprecision(5)<< time_taken << "sec";
   Solution_and_info << "\n" << "Time taken: " << std::setprecision(5) << time_taken << "sec"<<"\n";

   //Check if partition was stopped before finishing, If so print this in file with all the info about partitioning this matrix.
   if (Stop_Partition == 1) {
       Solution_and_info << " \n" << "STOPPED BEFORE PARTITION WAS FINISHED" << "\n"
           << "SOLUTION MAY NOT BE OPTIMAL"<< "\n";

   }


   //Prints the best solution that is found so far and the corresponding communication volume.
   //(It also prints the solution in the file)
   std::cout <<"\n"<< "The best solution found so far is:  ";
   Solution_and_info << "\n" << "The best solution found so far is:  ";
   for (int i = 0; i < Best_solution_sofar.size(); i++) {
       for (int j = 0; j < Processors; j++) {

           std::cout << Best_solution_sofar[i][j];
           Solution_and_info << Best_solution_sofar[i][j];
       }

       std::cout << " ";
      Solution_and_info << " ";
   }
   std::cout <<" With Comm. Vol:  "<< Lowest_cv_sofar<<"\n";
   Solution_and_info << "\n"<<"With Comm. Vol:  " << Lowest_cv_sofar << "\n";

   //Close the file with all information about the partitioning of this  matrix
   Solution_and_info.close();


   //Make a file to store the optimal solution for every matrix, in a format that makes a table in latex.
   //file name depends on number of processors.
    std::ofstream OptSol;
    //name of file
    std::string filename_Opt = "p=" + std::to_string( Processors) + ",latex_table.txt";

    OptSol.open(filename_Opt, std::ios::out | std::ios::app);
    OptSol << nameMatrix<<" & " << A.M << " & "<< A.N << " & " << A.nnz<< "  &"<< Lowest_cv_sofar << " & " << time_taken << " s"<< " \\\\ \\\hline " <<"\n";
    OptSol.close();

    //Make a file with information about the number of cuts in the first rowcol.
    //Use this to make a priority queue for first rowcol.
    std::ofstream Prior_queue;

    std::string filename_PriorQueue = "p=" + std::to_string(Processors) + ",priorQueue.txt";

    //Determine number of cuts in the first rowcol, in the optimal solution.
    std::vector<bool> State0=Best_solution_sofar[order2[0]];
    int Cut_rc_0= std::accumulate(State0.begin(), State0.end(), 0) - 1;


    Prior_queue.open(filename_PriorQueue, std::ios::out | std::ios::app);
    Prior_queue << nameMatrix <<  " & " << A.nnz << " & " << Max_Partition_size << " & " << A.perRow_Col[order2[0]] <<" & " << Cut_rc_0 << " & "
        << "\n";
    Prior_queue.close();




    output_States_nzs(A);

    std::cout << "GL4: " << aantalGL4 << " , combo : " << combolocal << " ,GL4 groter: " << GL4groter << " ,L4 groter " << L4groter << " , L3 " << L3gr<< " ,gelijk "<< gelijk << " ,GL42 groter dan l4 "<<GL42;

}

