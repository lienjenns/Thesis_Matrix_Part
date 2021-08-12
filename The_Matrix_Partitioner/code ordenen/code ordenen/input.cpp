#include "./input.h"
#include <fstream>
#include <vector>
#include<sstream>
#include<algorithm>
#include<string>
#include <iterator>


// Split a string on whitespace.
std::vector<std::string> tokenize(const std::string& line) {
    std::istringstream iss(line);
    return std::vector<std::string>{
        std::istream_iterator<std::string>{iss},
            std::istream_iterator<std::string>{}};
}

    matrix Read_From_File(std::string filename) {
       
        int M=0 ;
        int N=0 ;
        int nnz=0 ;
        std::vector<std::pair<int, int >> locations;
        bool symmetric = 0;


        // open file to read it
        std::ifstream file(filename);
        
      /*  if (file.is_open())
            std::cout << file.rdbuf();*/

        if (file.fail()) {
            std::cerr << "Not able to read the file."<<"\n";
            exit(3);
        }

       //The first line is the header line it contains information about the matrix
        //We are interested if the matrix is presented in symmetric form.

        std::string headerline;
        std::getline(file, headerline);

        const auto& tokens = tokenize(headerline);

        if (tokens.size() != 5)
            std::cerr<<"first line is not a valid typecode.";
        if (tokens[0] != "%%MatrixMarket")
            std::cerr << "first line did not start with a typecode.";
        if (tokens[1] != "matrix")
            std::cerr << "file does not describe a matrix.";
  

        if (tokens[4] == "symmetric" || tokens[4] == "hermitian" ||
            tokens[4] == "skew-symmetric") {
            symmetric = true;
        }
        // Ignore the rest of the headers and comments:
        while (file.peek() == '%') {
         
            file.ignore(2048, '\n');
        }


        // Read defining parameters:
        file >> M >> N >> nnz;

        //std::cout << M << " " << N << " " << nnz<< "\n";

        // Make sure the vector has enough capacity to store all information.
        locations.reserve(nnz);

        //This removes the first, now empty, row.
        std::string line;
        std::getline(file, line);
       

        // Read the data
        for (int l = 0; l < nnz; l++)
        {
            std::getline(file, line);
            std::istringstream iss(line);
            int m, n;
            double data;
            iss >> m >> n ;
          
            //The entries of the nonzeros -1, in order to start at 0.
            std::pair<int, int> q(m-1, n-1);
            locations.push_back(q);

            if (symmetric && m != n) {
                std::pair<int, int> q2(n - 1, m - 1);
                locations.push_back(q2);
            }
        }

        file.close(); 
        
        std::cout << "size locations " << locations.size()<<"\n";
        ////Prints the matrix in a real matrix form
        ////Extra check in order to see if the information from the file is correctly read.
        //int t = 0;
        //for (int m = 0; m <= M; m++)
        //{
        //    for (int n = 0; n <= N; n++) {
        //        std::pair<int, int> h(m, n);
        //        // try finding h, if h is not found std::find will return location.end()
        //        if (std::find(locations.begin(), locations.end(), h) != locations.end()) {
        //            std::cout << "X ";
        //            t++;
        //        }
        //        else {
        //            std::cout << "  ";
        //        }
        //    }
        //    std::cout << std::endl;
        //}

        return matrix(M, N, nnz, locations);
    }


   