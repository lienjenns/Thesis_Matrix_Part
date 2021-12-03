#pragma once



#include <iostream>
#include<utility>
#include<vector>
#include<tuple>

struct matrix {
public:
	//The number of rows in the compressed matrix.
	int M;
	//The numebr of columns in the compressed matrix.
	int N;
	//The number of the rows and columns in the original matrix.
	int og_M;
	int og_N;
	//The number of nonzeros in the matrix.
	int nnz;
	

	// The locations/entries (i,j) of the nonzeros in the compressed matrix.
	std::vector<std::pair<int, int >> locations;
	// The locations/entries (i,j) of the nonzeros in the original matrix.
	std::vector<std::pair<int, int >> og_locations;

	// The locations/entries (i,j) of the nonzeros in the compressed matrix with the corresponding nonzero index.
	std::vector<std::tuple<int, int, int>> loations_with_nz;
	//Number of nz per row and column
	std::vector<int> perRow_Col;

	//CRS data structure
	//Indices of the nonzeros in the rows
	std::vector<int> Row_nz_Entries;
	//The start vector Start_Row[i]= is the index of the first nonzero in row  i.
	std::vector<int> Start_Row;


	//CCS data structure
	//Indices of the nonzeros in the columns.
	std::vector<int> Col_nz_Entries;
	//The start vector for the columns, Start_Column[j] is the index of the first nonzero in column j.
	std::vector<int> Start_Column;

	//Constructs a "matrix" with the number of rows, columns and the entries of the nonzeros.
	matrix(int x, int y, int z, std::vector<std::pair<int, int >> locations);

	//Function that makes the CRS data structure and fills the vectors: "Row_nz_entries"and "Start_Row".
	void CRS(std::vector < std::pair<int, int>> locations, int nnz, int M);
	//Function that makes the CCS data structure and fills the vectors: "Col_nz_entries" and "Start_Column".
	void CCS(std::vector<std::tuple<int, int, int>> loations_with_nz ,int nnz, int number_of_columns);

	//Given an rowcol, this function determines the indices of the  nonzeros in that rowcol.
	std::vector<int> Intersecting_RowCol(int index_ROWCOL);

};
