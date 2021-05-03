// sanity.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<vector>
#include<set>
#include <algorithm> 
#include <stdexcept>
#include<string>



//code stopt
//En error printen of omgekeerd

std::vector<int> MaxValue_PartSize_vector = { 1,4,2 };

void Sanity_Check_PartitionSize(std::vector<int> Current_PArtition_sizes) {

    int options = Current_PArtition_sizes.size();

    for (int i = 0; i < options; i++) {

        if (Current_PArtition_sizes[i] > MaxValue_PartSize_vector[i]) {

           // std::cerr << "Errorrrr";
            /*try{*/
            std::string errosmessage = " Error in determining the Partition size of state: " + std::to_string(i);
           // errosmessage += std::to_string(i);
                throw errosmessage;
        /*}*/
            

           /* catch (const char* appel) {
                std::cerr << appel;

            }*/
        }
        else {

            throw " alloah";
            std::cout << "ok";

        }
    }

}

int main()
{
    try {
        std::vector<int>A = { 3,5,2 };
        Sanity_Check_PartitionSize(A);

    }
    catch (std::string appel) {
        std::cerr << appel;
    }

    catch (const char* a) {

        std::cout <<"Dit is char error:"<< a;
    }
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
