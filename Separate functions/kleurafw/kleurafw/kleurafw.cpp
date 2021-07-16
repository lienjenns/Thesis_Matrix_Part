// kleurafw.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<vector>
#include<numeric>
#include<algorithm>
#include<set>



int Processors = 3;
//int i = 1;

struct Ellende {

    Ellende(int index) { this -> index = index; }


  bool operator () (const std::vector<bool> & a,const std::vector<bool>& b) {


    return( (a[index] > b[index] ) && std::accumulate(a.begin(),a.end(), 0) != Processors || (a[index]==b[index] && std::accumulate(a.begin(), a.end(), 0) < std::accumulate(b.begin(), b.end(), 0)) ||   
        std::accumulate(b.begin(), b.end(), 0) == Processors);
  }
  int index;
};




struct Ellende2 {

    Ellende2(int index) { this->index = index; }


    bool operator () (const std::vector<bool>& a, const std::vector<bool>& b) {


        return(std::accumulate(a.begin(), a.end(), 0) < std::accumulate(b.begin(), b.end(), 0) || std::accumulate(a.begin(), a.end(), 0) == std::accumulate(b.begin(), b.end(), 0) && (a[index] > b[index]));
    }
    int index;
};


int main()
{
    std::vector<bool> A = { 0,1,1 };
    std::vector<bool> D = { 0,1,1 };
    std::vector<bool> C = { 1,0,0 };
    std::vector<bool> L = { 1,0,0 };
    std::vector<bool> E = { 1,1,1 };
    std::vector<bool> F = { 1,0,1 };
    std::vector<bool> H = { 0,1,0 };

    std::vector<std::vector<bool>> B = { A, D,H, C, E,F ,L};

   // std::sort(B.begin(), B.end(), Gedoe);

    std::sort(B.begin(), B.end(), Ellende(2));

    for (int j = 0; j < B.size(); j++) {

        for (int k = 0; k < 3; k++) {

            std::cout << B[j][k];

        }

        std::cout << "\n";
    }


  /*  std::set<int> s = { 1,2,3,4,5,1,6 };

    std::vector<int> v(s.begin(), s.end());

    for (int l = 0; l < v.size(); l++) {

        std::cout << v[l];
    }*/

   // std::cout << log2(8);

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
