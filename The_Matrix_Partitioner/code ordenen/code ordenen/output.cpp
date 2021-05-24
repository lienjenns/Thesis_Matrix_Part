
#include<vector>
#include"./matrix.h"
#include <iostream>
#include "./Util.h"
#include"./Global.h"
#include"Possible_States.h"


void output_States_nzs(matrix Info_matrix) {

	//Print some information about the matrix, number of processors and the sollution.
	std::cout << Info_matrix.M << " " << Info_matrix.N << " "<< Info_matrix.nnz << " " << Processors<< " "<< Max_Partition_size<< " "<< Lowest_cv_sofar<< "\n";

	std::vector<int> color_count_solution;
	int options = pow(2, Processors) - 1;
	color_count_solution.resize(options);

	std::vector<std::vector<bool>> Solution_nz;

	//Traverse all nonzeros, and determien the state (color) of each nonzero.
	for (auto i = Info_matrix.locations.begin(); i != Info_matrix.locations.end(); i++){

		std::pair<int, int > entries = *i;
		int row_entry = entries.first;
		int column_entry = entries.second + Info_matrix.M; //+M want i antwrd staat state van column j op entry j+M.

		std::vector<bool>State_nz = Best_solution_sofar[row_entry] && Best_solution_sofar[column_entry];

		Solution_nz.push_back(State_nz);

		int index_statenz = Binair_index(State_nz);
		color_count_solution[index_statenz] += 1;

	}


	//Voor elek partie van r , rb, tot en met rbg print hoeveel nz in elke partitie.
	for (int l = 0; l < options; l++) {

		std::cout << color_count_solution[l] << " ";

	}

	std::cout << " \n";

	//Print the entries and the state (color) of the nonzeros.
	for (int j = 0; j < Solution_nz.size(); j++) {
		
		std::cout << Info_matrix.locations[j].first+ 1<<" "<< Info_matrix.locations[j].second + 1<< " ";

		std::vector<bool> State = Solution_nz[j];

		for (int k = 0; k < Processors; k++) {

			std::cout << State[k];

		}

		std::cout << "\n";
	}
	


}