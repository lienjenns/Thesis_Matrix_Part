
#include "./Possible_States.h"
#include<algorithm>
#include<vector>
#include <math.h>
#include "./matrix.h"
#include "./Global.h"
#include<set>
#include "./Util.h"
#include<numeric>
#include<array>
#include"./Lower_bounds.h"




//Given the state of an assigned rowcol the function determines the LB1 for this rowcol.
int LB1(std::vector<bool>State_rowcol) {

    int Cuts_rowcol = 0;

    //Determines the number of processors to which the rowcol is assigned, == number of ones in state_rowcol.
    int Ones_State_rowcol = std::count(State_rowcol.begin(), State_rowcol.end(), true);

    //Determine the number of cuts in the state that is assigned to the rowcol.
    Cuts_rowcol = Ones_State_rowcol - 1;

    //Because we look at a state that is assigned to a rowcol, the state will contain at least one 1 so always cuts_rowcol>=0.
    //This in contrast to cuts_row_column_i in the LowerBound1 fucntion, in that function cuts_row_column_i  can be -1.
    //So in this case we can just return Cuts_rowcol.

    return Cuts_rowcol;
}



//This function determines the L2 bound for a rowcol.
//So it determines the implicit number of cuts for this rowcol, that is the number of implicit cuts for rowcol "rc".
std::vector<std::pair<int, std::vector<bool>>> L2bound(std::vector<bool> Status_intersect_rc, int rc,
            std::vector<std::pair<int, std::vector<bool>>> Partial_Status_rowcols) {

    //Determine the binairy index of the status of the intersecting rowcol.
    int index_state = Binair_index(Status_intersect_rc);

    //Now this rc has at least this partial status
    Partial_Status_rowcols[rc].second[index_state] = 1;

    //This is the vector that keeps all the information about the partial statuses of this rc, 
    std::vector<bool> info = Partial_Status_rowcols[rc].second;

    //Below we determine the partial statuses of rowcol "rc".
    //Save the binairy indices of the partial statuses and the partial statuses themselves in vectors.
    std::vector<int> index_partial_Statuses;
    std::vector<std::vector<bool>> Partial_Statuses;

    int options = pow(2, Processors) - 2;

    //First get the index of the partial statussen of the rowcol, "rc".
    for (int i = 0; i < options; i++) {

        if (info[i] == 1) {
            index_partial_Statuses.push_back(i);
        }
    }

    int x = index_partial_Statuses.size();

    //Now use the indices of the partial statuses to get the partial statuses themselves and save them in a vector.
    for (int j = 0; j < x; j++) {
        int ind_St = index_partial_Statuses[j];
        Partial_Statuses.push_back(Index_and_Status[ind_St]);
    }

    //
    //Prints all partal statuses of intersect_rowcol.
   /* std::cout << "All partial states: ";
    for (int h = 0; h < Partial_Statuses.size(); h++) {
        std::cout << "\n";
        for (int g = 0; g < Processors; g++) {

            std::cout << Partial_Statuses[h][g];
        }
    }
    std::cout << "\n";*/
    //


    //Use the assigned_States function to find all possible options for assignments based on the partial statuses alone for this rowcol 
    //and find the option with the least cuts i.e. state with the least number of ones.
    std::set<std::vector<bool>> Partial_options_rc = Assigned_States(Partial_Statuses, Zero_State); 

    //Save the state with the least number of ones. 

    //ToDo maybe we need to make a set in order to check L2 bound faster ToDo
    //The initial value of state with least number of ones is equal to number of processors p.
    int no_least_ones = Processors; //ToDo kan ook P-1 maar zo zorg je ervoor dat ie iig  bijna altijd met waaarde wordt vervangen uit partial_options_rc, of p+1 kiezen dan altijd vervangen.

    for (auto k = Partial_options_rc.begin(); k != Partial_options_rc.end(); k++) {

        //Determine the number of ones of state *k
        int no_ones = Determine_Set_indices(*k).size();

        //If no of ones i.e. no of processors in this state is less than the state with the least no of ones until now
        //Replace no_least_ones with no of ones in this state.
        if (no_ones < no_least_ones) {
            no_least_ones = no_ones;
        }
    }

    //Then the new L2_bound for this rowcol is;
    int L2_boun_rc = no_least_ones - 1;

    //Need to save the new L2_bound for this rowcol;
    Partial_Status_rowcols[rc].first = L2_boun_rc;

    //This prints the new L2 bound
    //std::cout << no_least_ones << "and L2 bound thi rc is: " << Partial_Status_rowcols[rc].first;

    return Partial_Status_rowcols;
}

// Vector with binary indices of the states with only 1 processor.
//This vector is used in the function L3bound.
std::vector<int> Info_L3;


//This function determines & saves the binary indices of the states with only 1 processor, in the external variable Info_L3.
void Basic_L3_info() {
   
    for (int j = 0; j < Processors; j++) {

       int  index = pow(2, j)-1;
        Info_L3.push_back(index);
    }
}

//For now this function is the most basic of possible local L3 bound, only looks at rowcols
//that are partial one color, consequently it  only takes into account partition sizes of  single processors.
//This function calculates the "local" L3 bound = local packing bound.
//with as input the Packing sets of the partial partition and the partition sizes of the partial partition.
int L3bound(std::array<std::vector<std::vector<int>>, 2> Packing_Sets, std::vector<int> Partition_Size) {

    //Starting value L3 bound
    int L3bound = 0;
    //Number of packing sets to check, for now this is equal to p i.e. number of processors.
    int no_Partials = Packing_Sets[1].size();

    //We loop over  the rows and cols.
    for (int j = 0; j < 2; j++) {
        std::vector<std::vector<int>> Row_or_Col_packingset = Packing_Sets[j];

        //We loop over all the packingsets of only the rows (or only the columns).
        //At this moment there are p packing sets.
        for (int i = 0; i < no_Partials; i++) {
            std::vector<int> present_Packing_Set = Row_or_Col_packingset[i];

            //How many nonzeros are still allowed in this partition.
            int still_allowed = Max_Partition_size - Partition_Size[Info_L3[i]]; 

            int sum = std::accumulate(present_Packing_Set.begin(), present_Packing_Set.end(), 0);

            //If number allowed is more than the sum of nonzeros that is partially coloured in this colour,
            //then there is nothing to do.
            if (sum <= still_allowed) {
                continue;
            }

            else {
                //Starting value of number of rows or columns cut for this packing set.
                int number_of_cuts = 0;

                //We need to cut rows(columns) as long as the sum of nonzeros in this packing set  >  no of nonzeros stil allowed in this packing set.
                //Cut rows(columns) in this packing set in descending order of no of free nonzeros per row(column).
                while (sum > still_allowed) {

                    int max_entry = *std::max_element(present_Packing_Set.begin(), present_Packing_Set.end());
                    int Index_MaxElement = std::max_element(present_Packing_Set.begin(), present_Packing_Set.end()) - present_Packing_Set.begin();

                    //We set the max element now at -1, so we will certainly not indicate this rowcol as having maxvalue again.
                    present_Packing_Set[Index_MaxElement] = -1;

                    sum -= max_entry;
                    number_of_cuts += 1;
                }
                L3bound += number_of_cuts;
            }


        }
    }
    return L3bound;
}


// This function updates the graph after assigning rowcol "rc_i" a state.
void Bi_Graph::Set_rowcol(int rc_i, std::vector <int> add_rc, std::vector<int> remove_rc, int* M, int* N, matrix * info, std::vector<int> PartialStatus) {  //Pointer matrix A megeven voor interscet rowcol kan nu evt. met vector<vector<int, als dit doet da n hoef je niet M en N los mee te gevn

    if (In_graph[rc_i] == 1) {
        remove_vertex(rc_i, M, N);
    }

    int no_to_remove = remove_rc.size();
    int no_to_add = add_rc.size();


    for (int j = 0; j < no_to_remove; j++) {

        int rcj = remove_rc[j];

        if (In_graph[rcj] == 1) {

            remove_vertex(rcj, M, N);
        }
    }

    for (int i = 0; i < no_to_add; i++) {

        int rck = add_rc[i];

        if (In_graph[rck] == 0) {

           
            add_vertex(rck, info -> Intersecting_RowCol(rck) , M, N, PartialStatus); ///Intersecting rijcol meot meegegeven worden dmv matrix struct.
        }
    }

}


//Constructor of the bipartite graph. 
//Because we use split vertices, we add p-1 vertices to the graph for every rowcol that gets added to the graph .
Bi_Graph::Bi_Graph(int* x, int* y) {

    adj.resize((Processors - 1) * (*x + *y));
    In_graph.resize(*x + *y);
    Match.resize((Processors - 1) * (*x + *y));

    //Initialize Match vector, ie. match[i]=i.
    std::iota(Match.begin(), Match.end(), 0);

    no_Matched = 0;

    Max_V = (Processors - 1) * (*x + *y);
}

//Function that determines if there is an augmenting path for vertex v_i, given the current matching in the Bi_Graph.
void Bi_Graph::Augment_Path(int v_i) {

    if (Match[v_i] != v_i) {
        std::cout << "al gematched";

    }

    // Will be set to 1 when augmented path is found
    bool augm = 0;

    //Vector used to store vertices
    std::vector<int> queue;

    //Vector used to see if a vertex is visited in the BFS tree, initialized as false.
    std::vector<bool> visited(Max_V, 0);

    //Vector that stores the parent of a vertex i the BFS tree, starts at -1 to indicate that no veretx has a parent in bfs.
    // -1 indicates that a veretx has no parent in the BFS tree.
    std::vector<int> parent(Max_V, -1);

    int v_k, match_k;
    int v_j, old_match_j;
    int v_l;

    queue.push_back(v_i);
    visited[v_i] = 1;

    while (augm == 0 && !queue.empty()) {

        v_j = queue.front();
        queue.erase(queue.begin(), queue.begin() + 1);

        for (auto l = adj[v_j].begin(); l != adj[v_j].end(); l++) {

            v_k = *l;

            if (visited[v_k]==1) {
                continue;
            }

            else {
                //v_k unmatched, so match v_k! 
                //We have found and augmenting path.
                if (Match[v_k] == v_k) {

                    parent[v_k] = v_j;

                    visited[v_k] = 1; //Niet perse nodig ka voor volledigheid

                    //v_k will be mathched to v_j
                    Match[v_k] = v_j;


                    while (v_j != v_i) {

                        old_match_j = Match[v_j];
                        Match[v_j] = v_k;
                        v_l = parent[old_match_j];

                        Match[old_match_j] = v_l;

                        v_j = v_l;
                        v_k = old_match_j;


                    }

                    Match[v_i] = v_k; //v_i van gemaakt was eerst v_j maakt niet uit als goed is
                    no_Matched += 1;
                    augm = 1;

                    break;


                }

                //If v_k is alrady matched, extend the BFS tree
                else {
                    match_k = Match[v_k];
                    parent[v_k] = v_j;
                    parent[match_k] = v_k;

                    visited[v_k] = 1;
                    visited[match_k] = 1;

                    queue.push_back(match_k);
                }

            }
        }
    }
}

//This function adds the  p-1 vertices corresponding to rowcol "rc_i" to the graph.
void Bi_Graph::add_vertex(int rc_i, std::vector<int> intersect_rc, int* M, int* N, std::vector<int> PartialStatus) { //ToDo hoe intersectrc meegeven? pointer en m+n 1 keer berekeeknen? of zo houden

  //rowcol i is now in the graph
    In_graph[rc_i] = 1;


    int no = intersect_rc.size();

    //Traverse all rowcols that intersect rowcol "rc_i".
    for (int j = 0; j < no; j++) {

        int k = intersect_rc[j];

        //If rowcol k is in the graph && has an other partial status then rowcol rc_i, add rowcol k to the adjacency list of rowcol rc_i , 
        //and vice-versa.
        if (In_graph[k] == 1 && PartialStatus[k] !=-1 && PartialStatus[rc_i] != PartialStatus[k]) {

            //Determine the index of the vertex that corresponds to rowcol rc_i & can have edges to rowcols that have partial status PartialStatus[k].
            //Determine also the index of the correct split vertex of rowcol k.
            int index_Splitvertex_rci;
            int index_Splitvertex_k;

            if (PartialStatus[k] > PartialStatus[rc_i]) {
                index_Splitvertex_rci = (PartialStatus[k] - 1) * (*M + *N) + rc_i;
                index_Splitvertex_k = (PartialStatus[rc_i]) * (*M + *N) + k;
            }
            else {
                index_Splitvertex_rci = (PartialStatus[k]) * (*M + *N) + rc_i;
                index_Splitvertex_k = (PartialStatus[rc_i] - 1) * (*M + *N) + k;
            }

            //PartialStatus[rc_i] > PartialStatus[k]) 

            adj[index_Splitvertex_rci].push_back(index_Splitvertex_k);
            adj[index_Splitvertex_k].push_back(index_Splitvertex_rci);
        }

    }

    //For every vertex corresponding to rowcol "rc_i" we need to chech if there in augmenting path starting in that vertex.
    //So if p=3, we need to check for p-1=2 vertices if there is an augmenting path starting in that vertex.
    for (int j = 0; j < Processors - 1; j++) {

        int index_Splitvertex_rci = rc_i + (*M + *N) * j;
        Augment_Path(index_Splitvertex_rci);
    }

    
}

//This function removes the  p-1 vertices corresponding to rowcol "rc_i" to the graph.
void Bi_Graph::remove_vertex(int rc_i, int* M, int* N) {

    //In this container the matches of the spit vertices of vertex i are stored.
    std::vector<int> Previous_matches_to_i;
    //rowcol rc_i is no longer in the graph.
    In_graph[rc_i] = 0;

    //Ga alle split vertices af 
    for (int j = 0; j < Processors - 1; j++) {

        int index_Splitvertex_rci = rc_i + (*M + *N) * j;

        for (auto l = adj[index_Splitvertex_rci].begin(); l != adj[index_Splitvertex_rci].end(); l++) {

            int vertex_k = *l;
            

            std::vector<int>::iterator it;
            it = std::find(adj[vertex_k].begin(), adj[vertex_k].end(), index_Splitvertex_rci);

            if (it != adj[vertex_k].end()) {

                adj[vertex_k].erase(it); //Gaat dit goed?
            }
        }

        //Now remove the adjacency list of this split vertex of vertex rc_i that is been removed.
        adj[index_Splitvertex_rci].erase(adj[index_Splitvertex_rci].begin(), adj[index_Splitvertex_rci].end());

        //Check if the splitvertex was matched.
        if (Match[index_Splitvertex_rci] != index_Splitvertex_rci) {

            int vertex_v = Match[index_Splitvertex_rci];

            Previous_matches_to_i.push_back(vertex_v);

            //Reset the matches of this split vertex corresponding to rc_i
            Match[vertex_v] = vertex_v;
            Match[index_Splitvertex_rci] = index_Splitvertex_rci;
            no_Matched--;
        }
    }

    int size_prevMatches = Previous_matches_to_i.size();
    //We need to check for every vertex v, that was previously matched to a split veretx of rowcol rc_i, if there is an augmenting path starting at vertex v.
    for (int i = 0; i < size_prevMatches; i++) {

        Augment_Path(Previous_matches_to_i[i]);
    }
}