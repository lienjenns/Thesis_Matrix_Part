#include "./MTX.h"
#include "./Input.h"
#include<algorithm>
#include<vector>
#include<tuple>




//Function that sorts by second element.
bool Sort_by_Second_than_first2(const std::tuple<int, int, int>& a, const std::tuple<int, int, int>& b)
{
    int a0 = std::get<0>(a);
    int b0 = std::get<0>(b);
    int a1 = std::get<1>(a);
    int b1 = std::get<1>(b);
    return (a1 < b1) || (a1 == b1 && a0 < b0);
}

matrix::matrix(int x, int y, int z, std::vector<std::pair<int, int >> entries) {
    M = x;
    N = y;
    og_M = x;
    og_N = y; 
    nnz = z;

    og_locations = entries;
    locations = entries;
   
    //Sort both vectors, then every onzero has the same index in both vectors, (is important for the output files later on after ilp procedure.)
    std::sort(og_locations.begin(), og_locations.end());
    std::sort(locations.begin(), locations.end());


    perRow_Col.resize((M+N), 0);
     
    //Fill the vector perRow_Col.
    for (std::pair<int, int> a : locations) {

        int _r = a.first;
        int _c = M + a.second;

        perRow_Col[_r] += 1;
        perRow_Col[_c] += 1;
    }

    //Now remove the rows and columns that contain no nonzeros from the matrix.
    //Also adjust the location entries of the nonzeros accordingly.

    int M_2 = M;
    int no_zero = 0;

    //Adjust M,N, locations.
    for (int i = 0; i < M; i++) {
        
        if (perRow_Col[i] == 0) {
            M_2 -= 1;
            no_zero += 1;
            for (int j = 0; j < nnz; j++) {
                if (locations[j].first > (i - no_zero)) {
                    locations[j].first -= 1;

                }
            }
        }
    }

    int N_2 = N;
    no_zero = 0;
    for (int i = M; i < (M+N); i++) {
       
        int index = i - M;
        if (perRow_Col[i] == 0) {
            N_2 -= 1;
            no_zero += 1;
            for (int j = 0; j < nnz; j++) {
                if (locations[j].second > (index - no_zero) ) {
                    locations[j].second -= 1;

                }
            }
        }
    }

    //Adjust the values of M and N to the compressed matrix
    M = M_2;
    N = N_2;

    CRS(locations, nnz, M);
  
    //Make the vector of locations of the nonzeros wih the number of the nonzero.
    std::sort(locations.begin(), locations.end());
    for (int n0 = 0; n0 < nnz; n0++) {

        std::tuple<int,int,int> new_loc=std::make_tuple(locations[n0].first, locations[n0].second, n0);
        loations_with_nz.push_back(new_loc);
    }

    CCS(loations_with_nz,nnz, N);
}


//This fucntion makes the compressed row storage data structure.
//It makes/fills in the following vectors in the matrix structure;  "Row_nz_entries and  "Start_Row".
//These two vectors are the CRS data structure.
void matrix::CRS(std::vector < std::pair<int, int>> locations, int nnz, int M) {
    sort(locations.begin(), locations.end());

    //Number of rows m plus 1 is vector length of start vector.
    int length = M + 1;
    int nz_number = 0;
    Start_Row.resize(length, 0);
    
    //Traverse all nonzeros.
    for (int i = 0; i < nnz; i++) {

        //Makes the vector with the index of the nonzeros.
        Row_nz_Entries.push_back(nz_number);

        //Determines how many nonzeros per row. 
        int theRow = 1 + locations[i].first;
        Start_Row[theRow] += 1;
        nz_number += 1;
    }
    //Makes the start vector.
    for (int j = 1; j < length; j++) {
        Start_Row[j] = Start_Row[j] + Start_Row[j - 1];

    }
}

//This fucntion makes the compressed column storage data structure.
//It makes/fills in the following vectors in the matrix structure;   "RowEntries" and "Start_Column".
//These two vectors form the CCS data structure.
void matrix::CCS(std::vector<std::tuple<int, int, int>> loations_with_nz, int nnz, int number_of_columns) {

    //sorts the pairs based on the second element of the pair and within every element sorts also based on 1st element.
    //Sorting based on the second element makes sure that in the next for loop the row indices of the nonzeros are stored per column.
    //So first teh row indices of the nz in column 0 are stored in RowEntries, then the row indices of the n in column 2etc..
    sort(loations_with_nz.begin(), loations_with_nz.end(), Sort_by_Second_than_first2);

    //Number of columns n plus 1 is vector length of start vector.
    int length = number_of_columns + 1;
    Start_Column.resize(length, 0);

    //Traverse all nonzeros
    for (int i = 0; i < nnz; i++) {

        //Makes the vector of the indices of nonzeros in the order in which they appear in the columns.
        std::tuple<int, int, int> location = loations_with_nz[i];
        int nz_number = std::get<2>(location);
        Col_nz_Entries.push_back(nz_number);

        //Determines how many nonzero per column.
        int col_number= std::get<1>(location);
        int thecolumn = 1 + col_number;
        Start_Column[thecolumn] += 1;
    }

    //Makes the start vector.
    for (int j = 1; j < length; j++) {
        Start_Column[j] = Start_Column[j] + Start_Column[j - 1];

    }
}



//Given an rowcol, this function determines the indices of the rowcols it intersects with in a nonzero
std::vector<int> matrix::Intersecting_RowCol(int index_ROWCOL) {
    int index = index_ROWCOL;

    //Container contains the indices of the nonzeros in the row/column.
    std::vector<int>  Indices_intersectrowCol;

    //If index is smaller than M it is a row.
    if (index_ROWCOL < M) {

        //is now the start in the nz entries.
        int Start_inColentry = Start_Row[index];
        int NO_NZ = Start_Row[index + 1] - Start_Row[index];

        for (int i = 0; i < NO_NZ; i++) {

            //Indices of the nonzeros in the row.
            int indexCol = Row_nz_Entries[Start_inColentry + i];
            Indices_intersectrowCol.push_back(indexCol);

        }
        return Indices_intersectrowCol;
    }

    //Otherwise the rowcol is a column.
    else {

        //We need to rescale the index because in start_Column column indicies start at 0 and not M.
        index -= M;

        //is now the start in the nz entries.
        int Start_inRowentry = Start_Column[index];
        int NO_NZ = Start_Column[index + 1] - Start_Column[index];

        for (int i = 0; i < NO_NZ; i++) {

            //Indices of the nonzeros in the column.
            int indexRow = Col_nz_Entries[Start_inRowentry + i];
            Indices_intersectrowCol.push_back(indexRow);
        }
        return Indices_intersectrowCol;
    }
}