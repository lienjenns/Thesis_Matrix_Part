#include "./matrix.h"
#include "./input.h"
#include<algorithm>
#include<vector>
#include "./Global.h"



bool Sort_by_Second_than_first(const std::pair<int, int>& a, const std::pair<int, int>& b)
{
    return (a.second < b.second) || (a.second == b.second && a.first < b.first);
}


matrix::matrix(int x, int y, int z, std::vector<std::pair<int, int >> entries) {
	M = x;
	N = y;
	nnz = z;
	locations = entries;
	Cmax = 0;

    CRS( locations, nnz,  M);
    CCS(locations, nnz, N);

    for (int i = 0; i < z; i++) {

        std::cout << locations[i].first;
        std::cout << locations[i].second;
        std::cout << "\n";
    }
    //check wordt het goed ingevuld vanuit read me functie
    //std::cout<< "aantal rijen"<<M<< '\n';


    adj.resize(M + N);
   

  //vector of vectors at the first M places of adj. [0,M-1] are the vectors with column entries of nnz in row i, 
    //Places [M,M+N-1] are the vectors with row entries of nnz for column j.
    for (std::pair<int, int> nz : locations) {
        // Identifiers [0, R) are reserved for rows, [R, R+C) for the columns.
        int _r = nz.first, _c = M + nz.second;
        // int _ri = (int)adj[_r].size();
        // int _ci = (int)adj[_c].size();

        adj[_r].emplace_back(_c);
        adj[_c].emplace_back(_r);


    }
  
   //This prints the "matrix", the vector of vectors adj.
   /* int b = 0;
    b = adj.size();
    std::cout <<"Adj size, should be equal to M+N: "<< b;
    int q = 0;
    std::vector<int> H;
    for (int k = 0; k < M + N; k++) {
        int q = adj[k].size();
        H = adj[k];
        std::cout << "\n"<< "adj indices: "<< k<< "  ";
        for (int n = 0; n <= q-1; n++)
            std::cout<< H[n]<<"  ";
    }*/
}




void matrix::CRS(std::vector < std::pair<int, int>> locations, int nnz, int M) {

    //sorts the pairs based on the first element of the pair and within every element sorts also based on 2nd element.
    sort(locations.begin(), locations.end());

    //Number of rows m plus 1 is vector length of start vector.
    int length = M + 1;

    
     Start_Row.resize(length, 0);

    for (int i = 0; i < nnz; i++) {

        //Makes the vector 1 x nnz j[k]
        //int entrycolom = M + locations[i].second;
        ColumnEntries.push_back(locations[i].second);

        //Determines how many nonzeros per row. 
        int theRow = 1+ locations[i].first;
        Start_Row[theRow] += 1;

    }
    //Makes the start vector.
    for (int j = 1; j < length; j++) {
        Start_Row[j] = Start_Row[j] + Start_Row[j - 1];

    }
    //Prints the vector j[k]
    std::cout << "j[k] row;";
    for (int k = 0; k < nnz; k++) {


        std::cout << ColumnEntries[k] << " ";


    }

    std::cout << "\n";

    //Prints the start vector for the row
    std::cout << "Start_Row;";
    for (int l = 0; l < length; l++) {


        std::cout << Start_Row[l] << " ";


    }

    std::cout << "\n";
}


void matrix::CCS(std::vector < std::pair<int, int>> Entries, int nnz, int number_of_columns) {

    //sorts the pairs based on the second element of the pair and within every element sorts also based on 1st element.
    sort(Entries.begin(), Entries.end(), Sort_by_Second_than_first);

    //Printen ordered vector of pairs.
   /* for (int j = 0; j < 7; j++) {

        std::cout << Entries[j].first << " ";
        std::cout << Entries[j].second << "\n";
    }
    std::cout << "\n";*/

    //Number of columns n plus 1 is vector length of start vector.
    int length = number_of_columns + 1;

     RowEntries;
     Start_Column.resize(length, 0);

    for (int i = 0; i < nnz; i++) {

     //Makes the vector 1 x nnz j[k]
        RowEntries.push_back(Entries[i].first);

     //Determines how many nonzero per column.
        int thecolumn = 1+ Entries[i].second;
        Start_Column[thecolumn] += 1;

    }
    //Makes the start vector.
    for (int j = 1; j < length; j++) {
        Start_Column[j] = Start_Column[j] + Start_Column[j - 1];

    }

    //Prints the vector j[k]
    std::cout << "j[k] Column;";
    for (int k = 0; k < nnz; k++) {


        std::cout << RowEntries[k] << " ";

    }

    std::cout << "\n";

    //Prints the start vector for the column.
    std::cout<<"Start_Column;";
    for (int l = 0; l < length; l++) {


        std::cout << Start_Column[l] << " ";


    }
    std::cout << "\n";
}
	
//This functions determines the value of Cmax and returns this value.
     int matrix::Determine_Cmax() {
         int x = M + N ;
         

        //This is the vector with number of nonzeros per row/column, on entries [0, M-1] the number of nonzeros per row,
        // [M, N+M-1] the number of nonzeros per column.
         perRow_Col.resize(x, 0);


         //Vector perRow_Col is made;
         for (std::pair<int, int> a : locations) {
             
             int _r = a.first;
             int _c = M + a.second;
             
             perRow_Col[_r] += 1;
             perRow_Col[_c] += 1;

         }

         //Prints the vector perRow_Col
         int  t = 0;
         std::cout << "\n"<< "The numbers of nz per row/column; ";
         for (int j = 1; j <= M + N ; j++) {
             std::cout << perRow_Col[t] << " ";
             t++;
         }

         //Determines Cmax.
         int Cmax = *max_element(perRow_Col.begin(), perRow_Col.end());

         //Prints the value of Cmax.
         std::cout << "\n "<< "CMax value: " << Cmax;
         std::cout << "\n";

         return(Cmax);

     }


     void matrix::Initial_nzstate_vector() {
      std::cout <<"AAntal proceesoors"<< Processors;

         std::vector < std::pair<int, std::vector<bool>>> Vector_NZs;
         Vector_NZs.resize(nnz);
         std::vector<bool> Unassigned(Processors, 0);

         for (int i = 0; i < nnz;  i++) {

             Vector_NZs[i].first = ColumnEntries[i];
             Vector_NZs[i].second = Unassigned;
         }

        

     }


     std::vector<int> matrix::Intersecting_RowCol(int index_ROWCOL) {
         int index = index_ROWCOL;

         
         if (index_ROWCOL < M) {
         std::vector<int> Indices_NZ;
         std::vector<int>  Indices_intersectCol;

            int Start_inColentry= Start_Row[index];
             int NO_NZ=Start_Row[index + 1] - Start_Row[index];

             for (int i = 0; i < NO_NZ; i++) {
                 Indices_NZ.push_back(Start_inColentry + i); //Index of nz's  in this row

                 int indexCol = ColumnEntries[Start_inColentry+ i] +M;
                 Indices_intersectCol.push_back(indexCol); //Indices of the columns intersecting the row in a nonzero.

             }

             return Indices_intersectCol;
         }


         else {
              index -= M;
             std::vector<int>  Indices_intersectingRow;

            int Start_inRowentry= Start_Column[index];
             int NO_NZ = Start_Column[index +1] - Start_Column[index];


             for (int i = 0; i < NO_NZ; i++) {

                 int indexRow = RowEntries[Start_inRowentry + i];
                     Indices_intersectingRow.push_back(indexRow); //Indices of the rows intersecting the column in a nonzero.

             }

             return Indices_intersectingRow;
         }

     }