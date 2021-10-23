// order.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>


#pragma once
#include<vector>
#include"./matrix.h"
#include <queue> 


class order {

public:

	std::queue<int> full_implicit_cut;
	std::vector<int> no_nz_rc;

	order(std::vector<int> X);
	order(int a);

	int Determine_next_rc(matrix* A);


};


#include<vector>
#include<algorithm>
#include"./matrix.h"
#include"./order.h"
#include <queue> 
order::order(std::vector<int> X) {

    full_implicit_cut;
    no_nz_rc = X;



}

order::order(int a) {
    full_implicit_cut;
    no_nz_rc.resize(a);


}

int order::Determine_next_rc(matrix* A) {

    int next_rc = 0;

    if (!full_implicit_cut.empty()) {


        next_rc = full_implicit_cut.front();
        full_implicit_cut.pop();

    }

    else {
        next_rc = std::max_element(no_nz_rc.begin(), no_nz_rc.end()) - no_nz_rc.begin();

    }

    no_nz_rc[next_rc] = -1;



    std::vector<int> intersecting_rc_maxrc = A->Intersecting_RowCol(next_rc);
    int no_intersecting = intersecting_rc_maxrc.size();
    for (int j = 0; j < no_intersecting; j++) {

        int rowcol = intersecting_rc_maxrc[j];
        no_nz_rc[rowcol] -= 1;

    }




    return next_rc;

}

int main()
{
    std::cout << "Hello World!\n";
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
