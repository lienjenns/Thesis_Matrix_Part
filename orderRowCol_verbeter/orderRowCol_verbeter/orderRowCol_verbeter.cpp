// orderRowCol_verbeter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<vector>
#include<algorithm> //nodig voor max_element




//Function that determines the order of the row and columns based on the number of nonzeros in a row/column

//moet als input krijgen uit matrix.cpp" perRow_Col"
std::vector<int> Determine_Order_row_columns2(std::vector<int> number_nz, int M) {

    //Vector in which order is stored.
    std::vector<int> Order__row_columns;
    bool x = 0;
    bool y = 0;
    bool z = 0;

    int number_of_rows_columns = number_nz.size();
    for (int i = 0; i < number_of_rows_columns; i++) {

        int Maximal_value;
        int Index_MaxElement;
        //Find row/col with maximum number of nonzeros, and we alternate between rows and cols.
        if (x == 0) {

             Maximal_value= *std::max_element(number_nz.begin(), number_nz.begin()+M);
             Index_MaxElement = std::max_element(number_nz.begin(), number_nz.begin() + M) - (number_nz.begin());
            x = 1;

         if (Maximal_value < 1) {            //Makes sure that rows/col with 0 nonzeros are not taken into account.
             y = 1;
             continue;
         }


        }
        else {
            Maximal_value= *std::max_element(number_nz.begin()+M, number_nz.end());
            Index_MaxElement = std::max_element(number_nz.begin()+M, number_nz.end() ) - (number_nz.begin());
            //set bool back to 0 i order that next rowcol is a row 
            x = 0;


            if (Maximal_value < 1) {            //Makes sure that rows/col with 0 nonzeros are not taken into account.
                z = 1;
                continue;
            }
        }

        //If there is no row and no col with more than 0 nz than we can stop
        if (y && z) {
            break;
        }

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


int main()
{
    std::vector<int> A = {3,1,4,5,2,3,4};
   /*int Maximal_value = *std::max_element(A.begin() + 3, A.end());
    int Index_MaxElement = std::max_element(A.begin() + 3, A.end()) - (A.begin() );*/


  /*int  Maximal_value = *std::max_element(A.begin(), A.begin() + 3);
    int Index_MaxElement = std::max_element(A.begin(),A.begin() + 3) - (A.begin());

    std::cout << "Max element: " << Maximal_value << " index max el: " << Index_MaxElement;*/

    std::vector<int> B = { 3,1,0,0,4,5,2,3,4 ,0};
    Determine_Order_row_columns2(B, 5);

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
