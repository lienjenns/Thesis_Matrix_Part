#ifndef MATRIX_H
#define MATRIX_H



#include <iostream>
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


	//Number of nz per row and column
	std::vector<int> perRow_Col;

	//CRS data structure
	//The column indices of the nonzeros 
	std::vector<int> ColumnEntries;
	//The start vector Start_Row[i]= is the index of the first nonzero in row  i.
	std::vector<int> Start_Row;


	//CCS data structure
	//Row indices of the nozeros
	std::vector<int> RowEntries;
	//The start vector for the columns, Start_Column[j] is the index of the first nonzero in column j.
	std::vector<int> Start_Column;

	//Constructs a "matrix" with the number of rows, columns and the entries of the nonzeros.
	matrix(int x, int y, int z, std::vector<std::pair<int, int >> locations);

	//Function that makes the CRS data structure and fills the vectors: "ColumnEntries"and "Start_Row".
	void CRS(std::vector < std::pair<int, int>> locations, int nnz, int M);
	//Function that makes the CCS data structure and fills the vectors: "RowEntries"and "Start_Column".
	void CCS(std::vector < std::pair<int, int>> Entries, int nnz, int number_of_columns);

	//This functions determines the value of Cmax and returns this value.
	int Determine_Cmax();

	//Given an rowcol, this function determines the indices of the rowcols it intersects with in a nonzero
	std::vector<int> Intersecting_RowCol(int index_ROWCOL);

};
#endif