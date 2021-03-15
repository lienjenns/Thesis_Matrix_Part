#include "./input.h"
#include <fstream>
#include <vector>
#include<sstream>
#include<algorithm>


    matrix Read_From_File(std::string filename) {
        //eerst variabelen opniwu definieren??? waarom h en cpp files???
        int M=0 ;
        int N=0 ;
        int nnz=0 ;
        std::vector<double> datas;
        std::vector<std::pair<int, int >> locations;


        // open file for read
        std::ifstream file(filename);
        
      /*  if (file.is_open())
            std::cout << file.rdbuf();*/

        if (file.fail()) {
            std::cout << "Kan het niet lezen";
        }

       // std::cout << file.rdbuf();
      
        // Ignore headers and comments:
        while (file.peek() == '%') {
         
            file.ignore(2048, '\n');
        }


        // Read defining parameters:
        file >> M >> N >> nnz;

        //std::cout << M << " " << N << " " << nnz<< "\n";

        // reserve data
        locations.reserve(nnz);
        datas.reserve(nnz);

        // Read the data
        for (int l = 0; l < nnz; l++)
        {
            int m, n;
            double data;
            file >> m >> n >> data;
            std::pair<int, int> q(m-1, n-1);
            locations.push_back(q);
            datas.push_back(data);
        }

        file.close(); 
        
       
        ////Prints the matrix in a real matrix form
        ////Extra check in order to see if the information from the file is correctly read.
        //int t = 0;
        //for (int m = 1; m <= M; m++)
        //{
        //    for (int n = 1; n <= N; n++) {
        //        std::pair<int, int> h(m, n);
        //        // try finding h, if h is not found std::find will return location.end()
        //        if (std::find(locations.begin(), locations.end(), h) != locations.end()) {
        //            std::cout << datas[t] << " ";
        //            t++;
        //        }
        //        else {
        //            std::cout << "0        ";
        //        }
        //    }
        //    std::cout << std::endl;
        //}

        return matrix(M, N, nnz, locations);
    }


   