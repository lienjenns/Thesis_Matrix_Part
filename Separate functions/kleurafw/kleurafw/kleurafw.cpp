// kleurafw.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<vector>
#include<numeric>
#include<algorithm>
#include<set>

//Function && operator for two vector<bool>. 
//For instance if a=[0,1,0] and b=[1,1,0], then the function returns a && b =[0,1,0]
std::vector<bool> operator &&(std::vector<bool> State_nz, std::vector<bool> State_rowcol) {

    int p = State_nz.size();
    int lengthState_row = State_rowcol.size();

    if (p == lengthState_row) {

        std::vector<bool> New_state_nz(p, 0);

        for (int i = 0; i < p; i++) {

            //If in both vectors the ith entry is 1, then the ith entry of the New_State_nz wil also be 1.
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

        std::cout << "Error in using && operator when determining New_state of NZ";
        std::vector<bool> Notassigned = { 0, 0, 0, 0 };
        return Notassigned;
    }
}

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


struct Alt_color2 {

    Alt_color2(std::vector<bool> index) { this->index = index; }


    bool operator () (const std::vector<bool>& a, const std::vector<bool>& b) {


        std::vector<bool> A = a && index;
        std::vector<bool> B = b && index;
        int sumA = std::accumulate(A.begin(), A.end(), 0);
        int sumB = std::accumulate(B.begin(), B.end(), 0);


        return(std::accumulate(a.begin(), a.end(), 0) < std::accumulate(b.begin(), b.end(), 0) || ((std::accumulate(a.begin(), a.end(), 0) == std::accumulate(b.begin(), b.end(), 0) )&& sumA > sumB));
    }
    std::vector<bool> index;
};

struct Alt_color3 {

    Alt_color3(std::vector<bool> index) { this->index = index; }


    bool operator () (const std::vector<bool>& a, const std::vector<bool>& b) {


        std::vector<bool> A = a && index;
        std::vector<bool> B = b && index;
        int sum_proc_A = std::accumulate(A.begin(), A.end(), 0);
        int sum_proc_B = std::accumulate(B.begin(), B.end(), 0);
        int sum_a = std::accumulate(a.begin(), a.end(), 0);
        int sum_b = std::accumulate(b.begin(), b.end(), 0);

        return( ( sum_a < sum_b && sum_proc_A !=0 ) || (sum_proc_A != 0 && sum_proc_B==0 && sum_a !=Processors ) || ( (sum_a == sum_b ) && (sum_proc_A > sum_proc_B) ) || sum_b == Processors );
    }
    std::vector<bool> index;
};

//( (sum_a == sum_b ) && (sum_proc_A > sum_proc_B) ) || sum_b == Processors || sum_proc_A ==0 && sum_b==Processors   );
// ( sum_a < sum_b && sum_proc_A !=0 )|| (sum_proc_A != 0 && sum_proc_B==0 ) || ( (sum_a == sum_b ) && (sum_proc_A > sum_proc_B) ) || sum_b == Processors )

int main()
{

    std::vector<bool> A = { 1,1,1 };
    std::vector<bool> D = { 0,1,1 };
    std::vector<bool> C = { 1,0,0 };
    std::vector<bool> L = { 0,0,1 };
    std::vector<bool> E = { 1,0,1 };
    std::vector<bool> F = { 1,1,0 };
    std::vector<bool> H = { 0,1,0 };

    std::vector<std::vector<bool>> B = { A, D,H, C,L, E,F};

   // std::sort(B.begin(), B.end(), Gedoe);
    std::vector<bool> sorting = { 1,0,1 };
    std::sort(B.begin(), B.end(), Alt_color3(sorting));

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
