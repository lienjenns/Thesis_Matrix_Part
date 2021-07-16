// plaatje.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include<sstream>
#include<string>

int main()
{
   
    int m, n;
    int i, j;

    std::vector<int> Proc;


        // open file to read it
        std::ifstream file(filename);

        

        if (file.fail()) {
            std::cerr << "Not able to read the file.";
            exit(3);
        }

        std::cout << "\\begin{tikzpicture}";

        std::string line;

        for (int k = 0; k < Proc[1]; k++) {

            std::getline(file, line);
            std::istringstream iss(line);
            int m, n;
            double data;
            iss >> i >> j;
            i--;
            j--;


            std::cout << "\\\draw[fill = blue, color = blue, line width = 0] ( " << j * 0.1 << ", " << (m - i) * 0.1 << ") rectangle( " << (j + 1) * 0.1 << ", " << (m - i + 1) * 0.1 << " );" << "\n";
        }

    std::cout << "\\\draw [very thin] (0, 0.1) rectangle (" << n*0.1 << ", "<< 0.1 +m *0.1<< ") ; "<< "\n";

    std::cout << "\\\end{tikzpicture}";
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
