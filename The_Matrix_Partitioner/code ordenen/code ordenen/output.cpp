
#include<vector>
#include"./matrix.h"
#include <iostream>
#include "./Util.h"
#include"./Global.h"
#include"Possible_States.h"
#include < tuple>
#include<fstream>
#include <string> 


//Function used in the sort function to sort a vector of pairs based on the second element
//instead of sorting it based on the first eleemnt.
bool Sort_by_third(const std::tuple<int, int,int>& a, const std::tuple<int, int,int>& b)
{
	return (std::get<2>(a) < std::get<2>(b));
}


void Tikz_picture(std::vector<std::tuple<int, int, int>> Solution, int m, int n) {

	//Make a file to store the latex commands to make a tikz picture of the partitioned matrix.
	std::ofstream TikzPic_matrix;
	//name of file for Tikz picture
	std::string filename_TikzPic = "p=" + std::to_string(Processors) + " " + nameMatrix + ", Tikz.txt";
	

	TikzPic_matrix.open(filename_TikzPic, std::ios::out | std::ios::app);

	
	TikzPic_matrix << "\\begin{tikzpicture}";
	if (Processors == 3) {

		std::vector<std::string> colors = { "red", "blue", "purple", "yellow", "orange", "green", "lightgray" };
		int j, i;
		for (int l = 0; l < Solution.size(); l++) {

			int color_index = std::get <2>(Solution[l]);
			std::string color = colors[color_index];
			j = std::get<1>(Solution[l]);
			i = std::get<0>(Solution[l]);

			
			TikzPic_matrix << "\\\draw[fill = , color = " << color << " , line width = 0] ( " << j * 0.1 << ", " << (m - i) * 0.1 << ") rectangle( " << (j + 1) * 0.1 << ", " << (m - i + 1) * 0.1 << " );" << "\n";
		}
	}

	else if (Processors == 4) {
		std::vector<std::string> colors = { "red", "blue", "purple", "yellow", "orange", "green", "pink", "black","brown","brown","darkgray","brown", "darkgray","darkgray",  "lightgray"};
		int j, i;
		for (int l = 0; l < Solution.size(); l++) {

			int color_index = std::get <2>(Solution[l]);
			std::string color = colors[color_index];
			j = std::get<1>(Solution[l]);
			i = std::get<0>(Solution[l]);

			
			TikzPic_matrix << "\\\draw[fill = , color = " << color << " , line width = 0] ( " << j * 0.1 << ", " << (m - i) * 0.1 << ") rectangle( " << (j + 1) * 0.1 << ", " << (m - i + 1) * 0.1 << " );" << "\n";
			
		}
	}
	else if (Processors == 2) {

		std::vector<std::string> colors = { "red", "blue",  "yellow" };
		int j, i;
		for (int l = 0; l < Solution.size(); l++) {

			int color_index = std::get <2>(Solution[l]);
			std::string color = colors[color_index];
			j = std::get<1>(Solution[l]);
			i = std::get<0>(Solution[l]);


			TikzPic_matrix << "\\\draw[fill = , color = " << color << " , line width = 0] ( " << j * 0.1 << ", " << (m - i) * 0.1 << ") rectangle( " << (j + 1) * 0.1 << ", " << (m - i + 1) * 0.1 << " );" << "\n";
		}

	}
	else {
		
	}


	TikzPic_matrix << "\\\draw [very thin] (0, 0.1) rectangle (" << n * 0.1 << ", " << 0.1 + m * 0.1 << ") ; " << "\n";

	
	TikzPic_matrix << "\\\end{tikzpicture}";

	TikzPic_matrix.close();

}



void output_States_nzs(matrix Info_matrix) {
//Make a file to store the solution of the partitioned matrix per coordinate, ie. nonzero. 
//The state of every nonzero is stored.
  //file name depends on number of processors.
std::ofstream output_NZ;
//name of file
std::string filename_output = "p=" + std::to_string(Processors) +" " + nameMatrix + ", OUT.txt";


output_NZ.open(filename_output, std::ios::out | std::ios::app);
	std::vector<std::tuple<int, int, int>> Solution;
	//Print some information about the matrix, number of processors and the sollution.

	output_NZ<< "%% nnz, p, Max, CV" << "\n" << "%%Number of nonzeros per processor(s)" << "\n";
	output_NZ <<  Info_matrix.nnz << " " << Processors << " " << Max_Partition_size << " " << Lowest_cv_sofar << "\n";


	std::vector<int> color_count_solution;
	int options = pow(2, Processors) - 1;
	color_count_solution.resize(options);

	std::vector<std::vector<bool>> Solution_nz;

	//Traverse all nonzeros, and determine the state (color) of each nonzero.
	for (auto i = Info_matrix.locations.begin(); i != Info_matrix.locations.end(); i++){

		std::pair<int, int > entries = *i;
		int row_entry = entries.first;
		int column_entry = entries.second + Info_matrix.M; //+M want i antwrd staat state van column j op entry j+M.



		std::vector<bool>State_nz = Best_solution_sofar[row_entry] && Best_solution_sofar[column_entry];

		//Check if every nonzero is assigned to a processor(s).
		if (State_nz == Zero_State) {
			std::cerr << "The partitioning failed, a nonzero has not been assigned to a processor." << "\n";
			exit(EXIT_FAILURE);
		}

		Solution_nz.push_back(State_nz);

		int index_statenz = Binair_index(State_nz);
		color_count_solution[index_statenz] += 1;

		Solution.push_back(std::make_tuple(row_entry, entries.second, index_statenz));

	}

	//Check if for the optimal solution the max partition sizes is not exceeded.
	for (int k = 0; k < options-1; k++) {
		std::vector<bool> StateofIndex = Index_and_Status[k];

		if ( std::accumulate(StateofIndex.begin(), StateofIndex.end(), 0) == 1) {

			if (color_count_solution[k] > Max_Partition_size) {

				std::cerr << "The partitioning failed, size of part " << k << " larger than max partition size allows."<< color_count_solution[k]<< "\n";
			}
		}
		else {
			int no_nz_inPart = 0;
			for (int i = 0; i < options - 1; i++) {
				std::vector<bool> State_l = Index_and_Status[i];

				if ((State_l && StateofIndex) == State_l) {
					no_nz_inPart += color_count_solution[i];
					
				}
			}

			int no_used_proc = std::accumulate(StateofIndex.begin(), StateofIndex.end(), 0);

			if (no_nz_inPart > (no_used_proc * Max_Partition_size)) {

				std::cerr<< "The partitioning failed, size of part " << k << " larger than max partition size allows.";
			}
		}
	}

	//Voor elek partie van r , rb, tot en met rbg print hoeveel nz in elke partitie.
	for (int l = 0; l < options; l++) {

		
		output_NZ << color_count_solution[l] << " ";
	}


	output_NZ <<"\n";

	//Print the entries and the state (color) of the nonzeros.
	//for (int j = 0; j < Solution_nz.size(); j++) {
	//	
	//	std::cout << Info_matrix.locations[j].first+ 1<<" "<< Info_matrix.locations[j].second + 1<< " ";

	//	std::vector<bool> State = Solution_nz[j];

	//	for (int k = 0; k < Processors; k++) {

	//		std::cout << State[k];

	//	}

	//	std::cout << "\n";
	//}
	
	//Sort the solutionvector by the state of the nonzeros.
	std::sort(Solution.begin(), Solution.end());

	//Print the solution vector, i.e. for every nonzeros print; row entry, column entry, binairy index of the state of the nonzero.
	for (int j = 0; j < Solution.size(); j++) {

		std::tuple<int, int, int> Sol_rowcol = Solution[j];

		
		output_NZ << std::get<0>(Sol_rowcol) + 1 << " " << std::get<1>(Sol_rowcol) + 1 << " " << std::get<2>(Sol_rowcol) << " "<<"\n";

		
	}

	output_NZ.close();

	Tikz_picture(Solution, Info_matrix.M, Info_matrix.N);
}

