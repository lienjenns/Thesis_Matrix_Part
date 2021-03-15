#ifndef MATRIX_H
#define MATRIX_H



#include <iostream>// is deze echt nodig??
#include<utility>// is deze echt nodig?
#include<vector>

struct matrix {
public:
	//The number of rows in the matrix.
	int M;
	//The numebr of columns in the matrix.
	int N;
	//The number of nonzeros in the matrix.
	int nnz;
	//The maximum number of nonzeros in a single row or column.
	int Cmax;

	
	// The locations/entries (i,j) of the nonzeros in the matrix.
	std::vector<std::pair<int, int >> locations;

	//Vector of vectors; gives per row and column the columns and rows of the nonzeros.
	std::vector<std::vector<int>> adj;

	//Number of nz per row and column
	std::vector<int> perRow_Col;
//
	std::vector<int> ColumnEntries;
	std::vector<int> Start_Row;
	//
	std::vector<int> RowEntries;
	std::vector<int> Start_Column;

	//Constructs a "matrix" with the number of rows, columns and the entries of the nonzeros.
	matrix(int x, int y, int z, std::vector<std::pair<int, int >> locations);


	void CRS(std::vector < std::pair<int, int>> locations, int nnz, int M);
	void CCS(std::vector < std::pair<int, int>> Entries, int nnz, int number_of_columns);
	int Determine_Cmax();
	std::vector<int> Intersecting_RowCol(int index_ROWCOL);

	void Initial_nzstate_vector();
};
#endif