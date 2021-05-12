#include "./matrix.h"
#include "./input.h"
#include<algorithm>
#include<vector>
#include "./Global.h"


//Function used in the sort function to sort a vector of pairs based on the second element
//instead of sorting it based on the first eleemnt.
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

}


//This fucntion makes the compressed row storage data structure.
//It makes/fills in the following vectors in the matrix structure;  "ColumnEntries and  "Start_Row".
//These two vectors are the CRS data structure.
void matrix::CRS(std::vector < std::pair<int, int>> locations, int nnz, int M) {

    //sorts the pairs based on the first element of the pair and within every element sorts also based on 2nd element.
   //The sorting based on the first element makes sure that in the next loop the column indices of the nonzeros in the rows are stored consecutively
    //So first are the column indices of the nonzeros in the first row, then the column indices of the nz in the second row etc..
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
    //Prints the vector j[k], the vcetor with the column indices of the nonzeros.
    /*std::cout << "j[k] row;";
    for (int k = 0; k < nnz; k++) {


        std::cout << ColumnEntries[k] << " ";


    }

    std::cout << "\n";*/

    //Prints the start vector for the row
    //std::cout << "Start_Row;";
    //for (int l = 0; l < length; l++) {


    //    std::cout << Start_Row[l] << " ";


    //}

    //std::cout << "\n";
}

//This fucntion makes the compressed column storage data structure.
//It makes/fills in the following vectors in the matrix structure;   "RowEntries" and "Start_Column".
//These two vectors form the CCS data structure.
void matrix::CCS(std::vector < std::pair<int, int>> Entries, int nnz, int number_of_columns) {

    //sorts the pairs based on the second element of the pair and within every element sorts also based on 1st element.
    //Sorting based on the second element makes sure that in the next for loop the row indices of the nonzeros are stored per column.
    //So first teh row indices of the nz in column 0 are stored in RowEntries, then the row indices of the n in column 2etc..
    sort(Entries.begin(), Entries.end(), Sort_by_Second_than_first);

    //Printen ordered vector of pairs.
   /* for (int j = 0; j < 7; j++) {

        std::cout << Entries[j].first << " ";
        std::cout << Entries[j].second << "\n";
    }
    std::cout << "\n";*/

    //Number of columns n plus 1 is vector length of start vector.
    int length = number_of_columns + 1;
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
 /*   std::cout << "j[k] Column;";
    for (int k = 0; k < nnz; k++) {


        std::cout << RowEntries[k] << " ";

    }

    std::cout << "\n";*/

    //Prints the start vector for the column.
   /* std::cout<<"Start_Column;";
    for (int l = 0; l < length; l++) {


        std::cout << Start_Column[l] << " ";


    }
    std::cout << "\n";*/
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


     //Given an rowcol, this function determines the indices of the rowcols it intersects with in a nonzero
     std::vector<int> matrix::Intersecting_RowCol(int index_ROWCOL) {
         int index = index_ROWCOL;
         std::vector<int>  Indices_intersectrowCol;

         //If index is smaller than M it is a row.
         if (index_ROWCOL < M) {
         std::vector<int> Indices_NZ;
         

            int Start_inColentry= Start_Row[index];
             int NO_NZ=Start_Row[index + 1] - Start_Row[index];

             for (int i = 0; i < NO_NZ; i++) {

                 //At this moment we are not usig this; the indices of the nz in the row.
                 Indices_NZ.push_back(Start_inColentry + i); 

                  //Indices of the columns intersecting the row in a nonzero.
                 //+M is needed to get the correct index for the columns.
                 int indexCol = ColumnEntries[Start_inColentry+ i] +M;
                 Indices_intersectrowCol.push_back(indexCol); 

             }

             return Indices_intersectrowCol;
         }

         //Otherwise the rowcol is a column.
         else {

             //We need to rescale the index because in start_Column column indicies start at 0 and not M.
              index -= M;
            

            int Start_inRowentry= Start_Column[index];
             int NO_NZ = Start_Column[index +1] - Start_Column[index];


             for (int i = 0; i < NO_NZ; i++) {

                //Indices of the rows intersecting the column in a nonzero.
                 int indexRow = RowEntries[Start_inRowentry + i];
                     Indices_intersectrowCol.push_back(indexRow); 

             }

             return Indices_intersectrowCol;
         }

     }