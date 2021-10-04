#pragma once
#include<vector>
#include<set>
#include<array>




//This function determiens the Lowerbound1 for an assigned rowcol,
//i.e. it determiens the number of cuts in the assigned rowcol.
int LB1(std::vector<bool>State_rowcol);

//This fucntion determines the L2 bound for a rowcol, that is the number of implicit cuts in the rowcol. 
std::vector<std::pair<int, std::vector<bool>>> L2bound(std::vector<bool> Status_intersect_rc, int rc, std::vector<std::pair<int, std::vector<bool>>> Partial_Status_rowcols);

//Function that makes the info for the most simple version of the l3bound.
//Only look at rowcols that are partialy coloured in one colour.
//This function determines & saves the binary indices of the states with only 1 processor, in the external variable Info_L3.
void Basic_L3_info();

// Vector with binary indices of the states with only 1 processor.
//This vector is used in the function L3bound.
extern std::vector<int> Info_L3;

//For now this function is the most basic of possible local L3 bound, only looks at rowcols
//that are partial one color, consequently it  only takes into account partition sizes of  single processors.
//This function calculates the "local" L3 bound = local packing bound.
int L3bound(std::array<std::vector<std::vector<int>>, 2> Packing_Sets, std::vector<int> Partition_Size);



//A struct that contains all the information necessary for the Local L4 bound.
//It is essntially a bipartite graph, with a matching
class Bi_Graph
{
public:
    //Adjacency list of the vertices
    std::vector<std::vector<int>>  adj;

    //This vector keeps track of which rowcols are in the graph
    //In_graph[i]=1 means rowcol i is in the graph.
    std::vector<bool> In_graph;


    //Match of the vertices, if vertex i is unmatched Match[i]=[i], if i is matched to j (and j to i)
    // then Match[i]=j & Match[j]=i;
    std::vector<int> Match;

    //Size of the matching in the bipartite graph
    int no_Matched;

    int L4_combL3;

    //Max number of vertices possible in the graph (will never happen)
    int Max_V;

    
    //Constructor Bigraph  
    Bi_Graph(int* M, int* N);

    //This function adds the vertices corresponding to rowcol "index_rc" to the graph.
    void add_vertex(int index_rc, std::vector<int> intersect_rc, int* M, int* N, std::vector<int> PartialStatus);

    //This function  removes the vertices corresponding to rowcol "rc_i" to the graph.
    void remove_vertex(int rc_i, int* M, int* N);

    //Determines if there is an augmenting path in the graph starting from vertex v_i. If this is the case the matching will increase by 1.
    void Augment_Path(int v_i);

    //This function updates the graph after assigning rowcol "rc_i" a state.
    void Set_rowcol(int rc_i, std::vector<int> add_rc, std::vector<int> remove_rc, int* M, int* N, matrix* info, std::vector<int> PartialStatus, std::array<std::vector<std::vector<int>>, 2> Packing_Sets, std::vector<int> Partition_Size); //Pointer matrix A megeven voor interscet rowcol kan nu evt. met vector<vector<int

    
};


//Function for Global L4 bond, uses bfs
int BFS_Global_L4( std::vector<int> Partial_Status,  matrix * A, std::array<std::vector<std::vector<int>>, 2> Packing_Sets, std::vector<int> Partition_Size, int lowerbound, std::vector<std::vector<bool>> states);


//Function for Global L4 bond, starts with the solution of the local L4 bound and uses bfs to find remaining matches,
//via paths of length at maximum of length_path.
int BFS2_Global_L4(std::vector<int> Matches_L4, int no_matched, std::vector<int> Partial_Status, int M, int N, matrix* A);

