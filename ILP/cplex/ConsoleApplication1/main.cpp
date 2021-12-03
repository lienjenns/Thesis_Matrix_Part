

#include <iostream>

#include <ilcplex/ilocplex.h>
#include<fstream>
#include<vector>
#include<time.h>
#include "./MTX.h"
#include "./Input.h"

ILOSTLBEGIN

//Declare variables, for processors and epsilon.
int P;
double Epsilon;

//Declare variables for the name of the matrix and its location.
std::string nameMatrix;
std::string Location_matrix;

//Given a  partition of a matrix, this fucntion determines the (explicit) communication volume of the  partition.
int LowerBound1(std::vector < std::vector<bool>>The_Partition) {

	int size = The_Partition.size();
	int Number_of_cuts = 0;

	//Traverse all the rowcols.
	for (int i = 0; i < size; i++) {

		std::vector<bool> State_i;
		State_i = The_Partition[i];
		int Ones_State_i = std::count(State_i.begin(), State_i.end(), true);

		//Number of cuts in the rowcol i is number of ones minus one.
		int Cuts_row_column_i = Ones_State_i - 1;

		//A rowcol in a partial partition or a rowcol with 0 nonzeros can have status (0, ..., 0),
		//So only count the cuts of rowcol i when Cuts_row_column_i>=0.
		if (Cuts_row_column_i >= 0) {
			Number_of_cuts += Cuts_row_column_i;
		}
	}
	return Number_of_cuts;
}


int main(int argc, char* argv[])
{
	if (argc < 2) {
		nameMatrix = "bibd_9_3";
		P = 4;
		Epsilon = 0.03;
		Location_matrix = "matrix/" + nameMatrix + ".mtx";
	}

	else {

		nameMatrix = argv[1];
		P = std::stoi(argv[2]);
		Epsilon = std::stod(argv[3]);
		Location_matrix = "matrix/" + nameMatrix + ".mtx";
	}

	//Read the information from the mtx file
	matrix A(Read_From_File(Location_matrix));

	std::cout << nameMatrix << " " << P << " " << Epsilon << "\n";

	//Determine the maximum alowed size of a part.
	int Max_size = (1 + Epsilon) * ceil((float)A.nnz / (float)P); //gaat goed rond t naar beneden af ook als bv 4.8 dan max parititon size 4 moet k nog floor en dan int? ToDo

	////Now make the ILP model in CPlex

	//Declare the environment "env" and the model "model" in the environment "env"
	IloEnv env; //Define environment object
	IloModel model(env); //Define a model in environment "env" named "model"


	//Get information from matrix struct "A"
	int nnz = A.nnz;
	int m = A.M;
	int n = A.N;
	int no_nets = m + n; // no nets (that contain only at least one nonzero)
	int no_net_var = no_nets * P; //Number of net variables
	int no_nnz_var = P * nnz;   //Number of nonzeros variables

	//Declare the variables in the environment
	//Declare the nonzero variables and the net variables
	IloBoolVarArray nonzeros(env, no_nnz_var);
	IloBoolVarArray nets(env, no_net_var);

	//Declare the expression of the objective function
	IloExpr Objective(env);

	//Make the objective function
	for (int i = 0; i < no_net_var; i++) {

		Objective += nets[i];
	}

	Objective -= no_nets;

	//objective is minimized
	IloObjective obj = IloMinimize(env, Objective);
	model.add(obj);      //Add objective to our model "model".

	Objective.end(); //Clear memory so end exspression after they are added to the model "model".


	//Make the nonzero constraints, and add them to the model "model".
	//Every nonzero must be assigned to one and only one processor, so nz(A) constraints. 
	for (int i = 0; i < nnz; i++) {
		IloExpr nz_con(env);

		for (int j = 0; j < P; j++) {

			int var = i + nnz * j;
			nz_con += nonzeros[var];
		}
		model.add(nz_con == 1);
		nz_con.end();
	}
	
	//Add the Load balance constraints, every part may contain at maximum "Max_size" nonzeros.
	//So p constraints.
	for (int i = 0; i < P; i++) {

		IloExpr loadbal(env);

		for (int j = 0; j < nnz; j++) {
			int var = j + i * nnz;
			loadbal += nonzeros[var];
		}
		model.add(loadbal <= Max_size);
		loadbal.end();
	}


	//Symmetry assignment nonzeros. 
	//Every partition has p! equivalent ones including itself, to remove some symmetry we fix the first nonzero.
	//nonzero 0 is assigned to proc 0.
	//Now every partition has (p-1)! equivalent ones including itself
	model.add(nonzeros[0] == 1);


	//Add the hyperedge constraints; x_ij<=y_kj, if nz "i" is in net "k".
	//So y_kj=1 if it contains a nonzero that is assigend to processor j.
	// nz(A) *2*p constraits.

	int count = 0; //Used to check if orrect number of constraints is added.
	for (int i = 0; i < no_nets; i++) {

		std::vector<int> RC_nz = A.Intersecting_RowCol(i);
		int n0_nz_rc = RC_nz.size();

		//Traverse all nonzeros that are in rowcol i.
		for (int j = 0; j < n0_nz_rc; j++) {

			int no_nz = RC_nz[j];

			for (int k = 0; k < P; k++) {

				count += 1;

				int var_nz = no_nz + nnz * k;
				int var_net = i + no_nets * k;

				model.add(nonzeros[var_nz] <= nets[var_net]);
			}
		}
	}

	
	clock_t start, end;
	start = clock();


	//Construct cplex object "solver" and extract the model to CPLEX
	IloCplex solver(model);


	//This is a aprameter that can be used to adjust the algorithm that cplex uses.
	//Mip uses dynmaic search or branch and cut, 0; cplex choosed, 1; branch and cut, 2; dynamic search
   // solver.setParam(IloCplex::Param::MIP::Strategy::Search, 0);

	//Make sure we only use one thread and the final optimal solutions are not allowed to have a "gap" between its value and the real optimal value.
	solver.setParam(IloCplex::Param::Threads, 1);    //Set threads to one in order to make ilp sequential
	solver.setParam(IloCplex::Param::MIP::Tolerances::AbsMIPGap, 0.0000000); //
	solver.setParam(IloCplex::Param::MIP::Tolerances::MIPGap, 0.0);

	//Solve the model "model".
	solver.solve();

	//Get the optimal value of the objective function found by the solver.
	std::cout << "opt value, CV: " << solver.getObjValue();

	end = clock();

	std::cout << " status: " << solver.getStatus(); //Status signals if solution is optimal

	//For more informatiom about the status of the solution and the cuts made during the optimilization;
	//std::cout << "cstat" << solver.getCplexStatus(); //cplexstatus also signals if solution is optimal
	//std::cout << "0cuts " << solver.getNcuts(solver.CutZeroHalf); //Can be used to determine how many of a specific type cut are used.

	double time_taken = double(end - start) / double(CLOCKS_PER_SEC);


	//Make a file to store the optimal value and used time for every matrix, in a format that makes a table in latex.
	std::ofstream OptSol;
	//file name depends on number of processors.
	std::string filename_Opt = "p=" + std::to_string(P) + ",latex_table.txt";

	OptSol.open(filename_Opt, std::ios::out | std::ios::app);
	OptSol << nameMatrix << " & " << A.og_M << " & " << A.og_N << " & " << A.nnz << "  &" << solver.getObjValue() << " & " << time_taken << " s" << " \\\\ \\\hline " << "\n";

	//Check if found solution is indeed optimal.
	if (solver.getStatus() != IloAlgorithm::Optimal) {
		OptSol << "NOT OPTIMAL";
		std::cerr << "Objective value found is not optimal!";
	}

	////Now generate some output files and do som checks to see if the solution is feasible.

	//Make a file to store the latex commands to make a tikz picture of the partitioned matrix.
	std::ofstream TikzPic_matrix;
	//name of file for Tikz picture
	std::string filename_TikzPic = "p=" + std::to_string(P) + " " + nameMatrix + ", Tikz.txt";

	//File to store the partitioned matrix in matrix market format
	std::ofstream output_NZ;
	//name of file
	std::string filename_output = "p=" + std::to_string(P) + " " + nameMatrix + ", OUT.txt";

	TikzPic_matrix.open(filename_TikzPic, std::ios::out | std::ios::app);
	output_NZ.open(filename_output, std::ios::out | std::ios::app);
	TikzPic_matrix << "\\begin{tikzpicture}";
	output_NZ << "%%MatrixMarket matrix coordinate general integer" << "\n";
	output_NZ << A.og_M << " " << A.og_N << " " << A.nnz << "\n";

	////Declare some variables in order to check the feasibility of the solution.

	//Container to store the partition sizes
	std::vector<int> part_sizes(P,0);

	//Container to store the states of the rowcols
	std::vector<bool> state(P, 0);
	std::vector<std::vector<bool>> rowcol_values(A.og_M+A.og_N, state);

	//Traverse all nonzeros to make the martrix market file for the partitioned matrix and file for the tikz pic.
	//And at the same time we run some checks.
	for (int i = 0; i < nnz; i++) {
		int ind_nz;

		std::pair<int, int> entries = A.og_locations[i];
		output_NZ << entries.first << " " << entries.second << " ";
		
		//Var to check if every noznero is assigned to one and only one proc.
		int assigned = 0;

		for (int j = 0; j < P; j++) {
			
			ind_nz = i + nnz * j;
			IloBool antwrd = solver.getIntValue(nonzeros[ind_nz]);  //get INT value: the Int is important otherwise variable can be e.g. 2 e-16.

			if (antwrd) {
				assigned++;
				part_sizes[j] ++;

				output_NZ << j << "\n";

				rowcol_values[entries.first][j] = 1;
				rowcol_values[(entries.second + A.og_M)][j] = 1;

				//Code to make tikz pic.
				if (P < 5) {

					int m = A.og_M;
					int n = A.og_N;

					std::vector<std::string> colors = { "red", "blue",  "yellow",  "black" };
					int color_index = j;
					std::string color = colors[color_index];
					int ri = entries.first;
					int cj = entries.second;

					TikzPic_matrix << "\\\draw[fill = , color = " << color << " , line width = 0] ( " << cj * 0.1 << ", " << (m - ri) * 0.1 << ") rectangle( " <<
						(cj + 1) * 0.1 << ", " << (m - ri + 1) * 0.1 << " );" << "\n";
				}
			}
		}
		
		if (assigned != 1) {
			OptSol << "ERROR X 0";
		}
	}

	TikzPic_matrix << "\\\draw [very thin] (0, 0.1) rectangle (" << A.og_N * 0.1 << ", " << 0.1 + A.og_M * 0.1 << ") ; " << "\n";
	TikzPic_matrix << "\\\end{tikzpicture}";
	
	//Check if part sizes aren't to large for the optimal solution (this shouldn't be the case).
	for (int i = 0; i < P; i++) {
		if ( part_sizes[i] > Max_size) {
			std::cerr << "Partition size of proc: " << i << " is too large";
			OptSol << "ERROR Part_Size";
		}
	}

	//Check the objective value.
	int expected_opt_value = LowerBound1(rowcol_values);
	if (expected_opt_value != solver.getObjValue()) {
		
		std::cerr << "Optimal value is possibly not optimal.";
		OptSol << "ERROR: optval X & Y";
	}

	OptSol.close();
	output_NZ.close();
	TikzPic_matrix.close();

	//Close the model, free the allocated memory
	//solver.clear();
	env.end();
}


