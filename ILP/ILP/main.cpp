

#include <iostream>

#include <ilcplex/ilocplex.h>
#include<fstream>
#include<vector>
#include<time.h>
#include "./MTX.h"
#include "./Input.h"
#include <algorithm>

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
		nameMatrix = "rel3";
		P = 4;
		Epsilon = 0.03;
		Location_matrix = "matrix/" + nameMatrix + ".mtx";
	}

	else {

		nameMatrix = argv[1];
		Location_matrix = argv[2];
		P = std::stoi(argv[3]);
		Epsilon = std::stod(argv[4]);
	}



	//Read the information from the mtx file
	matrix A(Read_From_File(Location_matrix));

	std::cout << nameMatrix << " " << P << " " << Epsilon << "\n";

	//Determine the maximum alowed size of a part.
	int Max_size = floor ( (1 + Epsilon) * ceil( (float)A.nnz / (float)P) ); 


	////Now make the ILP model in CPlex

	//Declare the environment "env" and the model "model" in the environment "env"
	IloEnv env; //Define environment object
	IloModel model(env); //Define a model in environment "env" named "model"


	//Get information from matrix struct "A"
	int nnz = A.nnz;
	int m = A.M;
	int n = A.N;
	int no_nets = m + n; // no. nets that contain at least one nonzero.
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
	//So P constraints.
	for (int i = 0; i < P; i++) {

		IloExpr loadbal(env);

		for (int j = 0; j < nnz; j++) {
			int var = j + i * nnz;
			loadbal += nonzeros[var];
		}
		model.add(loadbal <= Max_size);
		loadbal.end();
	}


	//Symmetry in assignment of the nonzeros: 
	//Every partition has p! equivalent ones including itself, to remove some symmetry we fix the first nonzero.
	//nonzero 0 is assigned to proc 0.
	//Now every partition has (p-1)! equivalent ones including itself
	model.add(nonzeros[0] == 1);


	//Add the hyperedge constraints; x_ij<=y_kj, if nz "i" is in net "k".
	//So y_kj=1 if it contains a nonzero that is assigend to processor j.
	// nz(A) *2*p constraits.
	for (int i = 0; i < no_nets; i++) {

		std::vector<int> RC_nz = A.Intersecting_RowCol(i);
		int n0_nz_rc = RC_nz.size();

		//Traverse all nonzeros that are in rowcol i.
		for (int j = 0; j < n0_nz_rc; j++) {

			int no_nz = RC_nz[j];

			for (int k = 0; k < P; k++) {


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



	//Make sure we only use one thread. Set threads to one in order to make ilp sequential
	solver.setParam(IloCplex::Param::Threads, 1);   


	//Solve the model "model".
	solver.solve();

	

	end = clock();


	//Get the optimal value of the objective function found by the solver.
	//We round the optimal value, although the objective value should be an integer, 
	//due to round off errors  e.g. by the computer a integer value can deviate from its integer value by very small amounts e.g.  with 2*e-16.
	//Other option to solve this problem is  by setting the parameter EPINT to 0 (default value is 1e-5). The parameter Epint specifies the amount by which an computed solution value 
	//for an integer variable can violate its integrality requirement. But setting Epint to 0 can possibly result in performance issues. Also it may be impossible to achieve a tolerance of 0 do to finite precision floating point computattions.
	//	solver.setParam(IloCplex::Param::MIP::Tolerances::Integrality, 0);  << settig Epint to 0.

	int OPt_val = round( solver.getObjValue() );

	std::cout << "opt value, CV: " << OPt_val;

	std::cout << " Status: " << solver.getStatus()<< "\n"; //Status signals if solution is optimal



	double time_taken = double(end - start) / double(CLOCKS_PER_SEC);


	//Check if found solution is indeed optimal.
	if (solver.getStatus() != IloAlgorithm::Optimal) {
		std::cerr << "\n"<< "ERROR: Objective value found is not optimal!";
	}


	//File to store the partitioned matrix in matrix market format
	std::ofstream output_NZ;
	//name of file
	std::string filename_output =  nameMatrix + ".mtx-I" + std::to_string(P);

	output_NZ.open(filename_output, std::ios::out | std::ios::app);
	
	output_NZ << "%%MatrixMarket matrix coordinate integer general" << "\n";
	output_NZ << "% Matrix partitioned into p=" << P << " parts." << "\n";
	output_NZ << "% Max. allowed partition size was: " << Max_size << "." << "\n";
	output_NZ << "% Optimal communication volume is: " << OPt_val << " with status "<< solver.getStatus() << "." << "\n";
	

	////Declare some variables in order to check the feasibility of the solution.

	//Container to store the partition sizes
	std::vector<int> part_sizes(P,0);

	//Container to store the states of the rowcols
	std::vector<bool> state(P, 0);
	std::vector<std::vector<bool>> rowcol_values(A.og_M+A.og_N, state);


	//Determine how many nonzeros are owned by each processor in the final partitioning.
	for (int i = 0; i < nnz; i++) {
		int ind_nz;

		for (int j = 0; j < P; j++) {

			ind_nz = i + nnz * j;
			IloBool antwrd = solver.getIntValue(nonzeros[ind_nz]);  //get INT value: the Int is important otherwise variable can be e.g. 2*e-16 due to round off errors.

			if (antwrd) {
				part_sizes[j] ++;

			}
		}
	}

	output_NZ << "%Size of the different parts: ";
	for (int j = 0; j < P; j++) {
		output_NZ<< part_sizes[j] << " ";
	}
	output_NZ << "."<< "\n";
	output_NZ << A.og_M << " " << A.og_N << " " << A.nnz << "\n";

	//Traverse all nonzeros to make the martrix market file for the partitioned matrix.
	//And at the same time we run some checks.
	for (int i = 0; i < nnz; i++) {
		int ind_nz;

		std::pair<int, int> entries = A.og_locations[i];
		output_NZ << entries.first +1 << " " << entries.second + 1 << " ";
		
		//Var to check if every nonzero is assigned to one and only one proc.
		int assigned = 0;

		for (int j = 0; j < P; j++) {
			
			ind_nz = i + nnz * j;
			IloBool antwrd = solver.getIntValue(nonzeros[ind_nz]);  //get INT value: the Int is important otherwise variable can be e.g. 2 e-16 due to round off errors.

			if (antwrd) {
				assigned++;

				output_NZ << j+1 << "\n";

				rowcol_values[entries.first][j] = 1;
				rowcol_values[(entries.second + A.og_M)][j] = 1;

			
			}
		}
		
		if (assigned != 1) {
			std::cerr << "ERROR: a nonzero is assigned to multiple or no processors." << "\n";
		}
	}

	
	//Check if part sizes aren't to large for the optimal solution (this shouldn't be the case).
	for (int i = 0; i < P; i++) {
		if ( part_sizes[i] > Max_size) {
			std::cerr << "ERROR: Partition size of proc: " << i << " is too large"<< "\n";
		}
	}

	//Check the objective value.
	int expected_opt_value = LowerBound1(rowcol_values);
	if (expected_opt_value != OPt_val) {
		
		std::cerr << "ERROR: Optimal value is possibly not optimal."<< "\n";
	}

	output_NZ.close();

	//Close the model, free the allocated memory
	//solver.clear();
	env.end();
}