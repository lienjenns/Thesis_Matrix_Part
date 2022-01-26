
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
#include <stack>
# include<deque>




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

    //First get the index and partial statussen of the rowcol, "rc".
    for (int i = 0; i < options; i++) {

        if (info[i] == 1) {
            index_partial_Statuses.push_back(i); 
            
            Partial_Statuses.push_back(Index_and_Status[i]);
        }
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
        //If we find a state with only 1 one, there will not be a state with a lower number of ones, so break.
        if (no_least_ones == 1) {
            break;
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

//Fucntion computes the L3 bound 
// Only looks at rowcols that are partial one color, consequently it  only takes into account partition sizes of  single processors.
//This function calculates the "local" L3 bound = local packing bound.
//with as input the Packing sets of the partial partition and the partition sizes of the partial partition.
int L3bound_improved(std::array<std::vector<std::vector<int>>, 2> &Packing_Sets, std::vector<int> &Partition_Size, matrix * A) {

    //Starting value L3 bound
    int L3bound = 0;

    //We loop over  the rows and cols.
    for (int j = 0; j < 2; j++) {
      

        //We loop over all the packingsets of only the rows (or only the columns).
        //There are p packing sets.
        for (int i = 0; i < Processors; i++) {
           

            //How many nonzeros are still allowed in this partition.
            int still_allowed = Max_Partition_size - Partition_Size[Info_L3[i]];
            int sum = 0;
            //Determine the sum of free nonzeros in this packingset which we would like to assign to processor i.
            for (int k = 0; k < (A -> Cmax); k++) {

                sum += k * Packing_Sets[j][i][k];
            }


            //If number allowed is more than the sum of nonzeros that is partially coloured in this colour,
            //then there is nothing to do.
            if (sum <= still_allowed) {
                continue;
            }

            else {
                //Start with the row(column) with most free nonzeros.
                int c = (A -> Cmax ) - 1;
                //Number of rows(columns) with c free nonzeros.
                int present_no_c = Packing_Sets[j][i][c];

                //We need to cut rows(columns) as long as the sum of nonzeros in this packing set  >  no of nonzeros stil allowed in this packing set.
                //Cut rows(columns) in this packing set in descending order of no of free nonzeros per row(column).
                while (sum > still_allowed) {

                    while (present_no_c == 0) {
                        c--;
                        present_no_c = Packing_Sets[j][i][c];
                    }

                    sum -= c;
                    present_no_c -= 1;
                    L3bound += 1;
                }
            }
        }
    }

    return L3bound;
}


//For now this function is the most basic of possible local L3 bound, only looks at rowcols
//that are partial one color, consequently it  only takes into account partition sizes of  single processors.
//This function calculates the "local" L3 bound = local packing bound.
//with as input the Packing sets of the partial partition and the partition sizes of the partial partition.
//int L3bound(std::array<std::vector<std::vector<int>>, 2> Packing_Sets, std::vector<int> Partition_Size) {
//
//    //Starting value L3 bound
//    int L3bound = 0;
//    //Number of packing sets to check, for now this is equal to p i.e. number of processors.
//    int no_Partials = Packing_Sets[1].size();
//
//    //We loop over  the rows and cols.
//    for (int j = 0; j < 2; j++) {
//        std::vector<std::vector<int>> Row_or_Col_packingset = Packing_Sets[j];
//
//        //We loop over all the packingsets of only the rows (or only the columns).
//        //At this moment there are p packing sets.
//        for (int i = 0; i < no_Partials; i++) {
//            std::vector<int> present_Packing_Set = Row_or_Col_packingset[i];
//
//            //How many nonzeros are still allowed in this partition.
//            int still_allowed = Max_Partition_size - Partition_Size[Info_L3[i]]; 
//
//            int sum = std::accumulate(present_Packing_Set.begin(), present_Packing_Set.end(), 0);
//
//            //If number allowed is more than the sum of nonzeros that is partially coloured in this colour,
//            //then there is nothing to do.
//            if (sum <= still_allowed) {
//                continue;
//            }
//
//            else {
//                //Starting value of number of rows or columns cut for this packing set.
//                int number_of_cuts = 0;
//
//                //We need to cut rows(columns) as long as the sum of nonzeros in this packing set  >  no of nonzeros stil allowed in this packing set.
//                //Cut rows(columns) in this packing set in descending order of no of free nonzeros per row(column).
//                while (sum > still_allowed) {
//
//                    int Index_MaxElement = std::max_element(present_Packing_Set.begin(), present_Packing_Set.end()) - present_Packing_Set.begin();
//                    int max_entry = present_Packing_Set[Index_MaxElement];
//                    //We set the max element now at -1, so we will certainly not indicate this rowcol as having maxvalue again.
//                    present_Packing_Set[Index_MaxElement] = -1;
//
//                    sum -= max_entry;
//                    number_of_cuts += 1;
//                }
//                L3bound += number_of_cuts;
//            }
//
//
//        }
//    }
//    return L3bound;
//}


// This function updates the graph after assigning rowcol "rc_i" a state.
void Bi_Graph::Set_rowcol(int rc_i, std::vector <int> add_rc, std::vector<int> remove_rc,  matrix * info, std::vector<int> PartialStatus,
 std::vector<int> Partition_Size, std::array<std::vector<std::vector<int>>, 2>  Packing_Sets2, std::vector<std::vector<int>> &color_count ) {  

    

    if (In_graph[rc_i] == 1) {
        remove_vertex(rc_i, &(info->M),  &(info->N) );
       
    }

    int no_to_remove = remove_rc.size();
    int no_to_add = add_rc.size();


    for (int j = 0; j < no_to_remove; j++) {

        int rcj = remove_rc[j];

        if (In_graph[rcj] == 1) {

            remove_vertex(rcj, &(info->M), &(info->N) );
            
        }
    }

    for (int i = 0; i < no_to_add; i++) {

        int rck = add_rc[i];

        if (In_graph[rck] == 0) {

           
            add_vertex(rck, info -> Intersecting_RowCol(rck) , &(info->M), &(info->N), PartialStatus);
         
        }
    }

   
    if (CombL3_L4 ) {

        for (int k = 0; k < (info->M + info->N); k++) {

            for (int l = 0; l < (Processors - 1) ; l++) {

                int index_vj = k + l * (info->M + info->N);
                if (Match[index_vj] != index_vj) {

                    //If the rowcol=row.
                    if (k < (info->M)) {
                        int index_packingset = PartialStatus[k];
                        int index_partial_status = pow(2, index_packingset) - 1;

                        int no_nz = info->perRow_Col[k] - color_count[index_partial_status][k];
                        Packing_Sets2[0][index_packingset][no_nz] -= 1;
                    }

                    //If the  rowcol=column.
                    else {

                        int index_packingset = PartialStatus[k];
                        int index_partial_status = pow(2, index_packingset) - 1;

                        int no_nz = info->perRow_Col[k] - color_count[index_partial_status][k];
                        Packing_Sets2[1][index_packingset][no_nz] -= 1;
                    }

                    break;
                }
            }   
        }
        
        L4_combL3 = L3bound_improved(Packing_Sets2, Partition_Size, info);
      
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
    L4_combL3 = 0;
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
        if (In_graph[k] == 1 && PartialStatus[k] > -1 && PartialStatus[rc_i] != PartialStatus[k]) {

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


//This function determines the Gobal L3 bound, given the global packingsets and the sizes of every part.
int Compute_value_GL3(std::vector<std::vector<int>> &Global_PackingSets, std::vector<int> Partition_sizes) {

    int Value_GL3 = 0;

    for (int i = 0; i < Processors; i++) {

        std::vector<int> PackingSet_i = Global_PackingSets[i];

        int still_allowed = Max_Partition_size - Partition_sizes[Info_L3[i]]; 
        int sum = std::accumulate(PackingSet_i.begin(), PackingSet_i.end(), 0);
        
        if (sum <= still_allowed) {
            continue;
        }
        else {

            while (sum > still_allowed) {
                int index_max, maxVal;
                index_max = std::max_element(PackingSet_i.begin(), PackingSet_i.end()) - PackingSet_i.begin();
                maxVal = PackingSet_i[index_max];

                sum -= maxVal;
                PackingSet_i[index_max] = -1;

                Value_GL3 += 1;
            }
        }
    }

    return Value_GL3;
}

//This function grows the subgraphs for the global packing bound, i.e. GL3 bound.
//The start point of the subgraphs are partially assigned rowcols (partially assigned to  1 proc) that are not on a matching path in the GL4 bound. 
int GL3_bound(std::vector<bool> Matched_GL4, std::vector<bool> visited_GL4, std::vector<int> Partial_status, matrix * A, std::vector<int> Partition_size, std::vector<std::vector<bool>> states) {

    int GL3 = 0;
    int no_rc = A->M + A->N;

    //Store the index of the partially assigned rowcol in this container. 
    std::deque<int> order_subgraphs;                //This queue gives the order in which we traverse the subgraphs.
    std::vector<std::stack<int>> subgraph_stacks;   //Keeps track of the stack of rowcol i.
    std::vector<int> subgraph_sizes(no_rc,0);       //keeps track of the size of each subgraph.
    std::vector<int> index_subgraphs;               //Contains the first vertex of every subgraph.
    std::vector<int> Startindex_intersect(no_rc,0); //For row/column i it gives the number of the intersecting column/row we will look at.
    std::vector<bool> Claimed(no_rc, 0);            //Used for the unassigned rowcols, to signal if they are claimed by another subgraph and to signal that partial rowcol is starting point subgraph. ToDo

    subgraph_stacks.resize(no_rc);
   
    
    //First determine which partially assigned rowcols are not matched in the GL4 bound.
    //Note: We only take into account partially assigned rowcols, that are partially assigned to one processor, i.e partial status >=0.
    for (int i = 0; i < no_rc; i++) {
        if (Matched_GL4[i] == 0 && (Partial_status[i] >= 0 )) {

            order_subgraphs.push_back(i);
            subgraph_stacks[i].push(i);
            index_subgraphs.push_back(i);
            Claimed[i] = 1;
        }
    }

   //Now do DFS for every subgraph simultaneously.
    while (!order_subgraphs.empty() ) {

        //We will now try to expand the subgraph of vertex/rowcol rc.
        int rc = order_subgraphs.front();
        std::stack<int> stack_v = subgraph_stacks[rc];

        //If stack of veretx v is empty then we cannot expand the subgraph of v, so we go to the next subgraph.
        if (stack_v.empty()) {

            order_subgraphs.pop_front();
            continue;
        }

        //Signals if we are succesfull in expanding the subgraph of rc.
        bool nonzero_claimed = 0;

        int v_i = stack_v.top();

        //Start expanding subgraph from veretx v_i, if not possible we pop v_i from the stack of subgraph rc.
         while (!nonzero_claimed){
 
            int Start_intersect=Startindex_intersect[v_i];
            std::vector<int> intersect_rc_vi =A -> Intersecting_RowCol(v_i);  
            
            if (Start_intersect == intersect_rc_vi.size()) {

                subgraph_stacks[rc].pop();
                break;
            }
                
            Startindex_intersect[v_i] += 1;

            int v_k = intersect_rc_vi[Start_intersect];

            //If the vertex v_k is assigned or is claimed by another subgraph we can do nothing.
            if ( Claimed[v_k]) {
                continue;
            }

            else if (Partial_status[v_k] == -3) {
                std::vector<bool> state_vk = states[v_k];
                int sum = std::accumulate(state_vk.begin(), state_vk.end(), 0);
                int partstate_rc = Partial_status[rc];
                if ((sum > 1) && (state_vk[partstate_rc] == 1)) {

                    subgraph_sizes[rc] += 1;
                    nonzero_claimed = 1;

                }
                else {
                    continue; 
                }

            }
            //If v_k is not assigned and on a matching path in GL4. So partial assigned and matched in GL4 or unassigned and visited in gL4.
            //Then we can take the nonzero, but we cannot expand from vertex v_k.
            //If v_k is partially assigned but not to one processor specifically we can claim the nonzero but not expand from vertex v_k.
            else if (( (Partial_status[v_k] >= 0 ) &&  Matched_GL4[v_k]) || ( Partial_status[v_k]== -1 && visited_GL4[v_k] ) || Partial_status[v_k] < -3 || Partial_status[v_k]== -2 ) {

                subgraph_sizes[rc] += 1;
                nonzero_claimed = 1;
            }

            //If v_k is unassigned and not visisted in GL4 and not claimed in another subgraph, we can claim the edge and
            //we can expand from v_k.
            else if (Partial_status[v_k] == -1 && visited_GL4[v_k] == 0 && Claimed[v_k] == 0) {

                subgraph_stacks[rc].push(v_k);
                Claimed[v_k] = 1;
                subgraph_sizes[rc] += 1;
                nonzero_claimed = 1;
            }

            else {
                continue;
            }
         }

        if (nonzero_claimed) {

            order_subgraphs.pop_front();
            order_subgraphs.push_back(rc);
        }
    }

    std::vector<std::vector<int>> Packing_set_GL3;
    Packing_set_GL3.resize(Processors);

    for (auto l = index_subgraphs.begin(); l != index_subgraphs.end(); l++) {

        int index_rc = *l;
        int partial_status_rc = Partial_status[index_rc];

        Packing_set_GL3[partial_status_rc].push_back(subgraph_sizes[index_rc]);
    }

    GL3 = Compute_value_GL3(Packing_set_GL3, Partition_size);
    return GL3;
}

//Function that determines the Global L4 bond, tries to find paths form  partially assigned rowcols to other different partially assigned rowcols.
//After detrmining the GL4 bound it computes (after removing the paths used in the GL4 bound), the L3  and GL3 bound and adds the maximum of these two to the gL4 bound
int BFS_Global_L4( std::vector<int> Partial_Status,  matrix * A, std::vector<int> Partition_Size, int lowerbound, 
    std::vector<std::vector<bool>> states, std::array<std::vector<std::vector<int>>, 2> Packing_Sets2, std::vector<std::vector<int>> &color_count) {

    //Total number of matches found by the Global L4 bound.
    int GL4_bound = 0;
    //A partially assigned rowcol can match at maximum with p-1 other rowcols.
    int max_matches = Processors - 1;

    //vector used to see if a vertex is visited
    std::vector<bool> visited((A -> M +A -> N), 0);

    //During the whole path finding "bfs"  maintain the partial statuses/colors of the vertices  that match with vertex i.
    //e.g. if maintain_color_match[i]=(0,1,0)then vertex/rowcol i is matched with a vertex that has partial status 1.
    std::vector<bool> Color_match_rc(Processors, 0);  //To-do eigenlijk te lang p-1 kan
    std::vector<std::vector<bool>> maintain_color_match((A->M + A->N), Color_match_rc);
  
    //Keeps track which partilly assigned vertices are matched.
    //This info is needed for the GL3 bound.
    std::vector<bool>Matched((A->M + A->N), 0);

    //Sort the rowcols, to make sure we first try to find matching path with the start and end point being rowcols partially assigned to 1 processor,
    //and then traverse the rowcols partially assigned to 2 processors.
    std::vector<int> order((A->M + A->N));
    std::iota(order.begin(), order.end(), 0); //Initializing
    std::sort(order.begin(), order.end(), [&Partial_Status](int i, int j) {return ((Partial_Status[i] >= 0) && (Partial_Status[j] < 0)); });


    //Traverse all rowcols
    for (auto rowcol = order.begin(); rowcol != order.end(); rowcol ++) {

        int v_i = *rowcol;

        //With which colors is v_i already matched?
         std::vector<bool> maintain_color_matches_i=maintain_color_match[v_i]; 
        //Number of matches of veretx v_i
        int no_matches_vi = std::accumulate(maintain_color_matches_i.begin(), maintain_color_matches_i.end(), 0);

        //If v_i is unassigned, assigned, partially assigned different than partialy assigned to 1 or 2 processors,
        //,or if v_i is already matched with p-1 processors/colors;
        //Then we can find no path that results in a match for v_i.
        if (Partial_Status[v_i] ==-3 || Partial_Status[v_i] == -7 || Partial_Status[v_i]== -1 ||no_matches_vi == max_matches) {
            continue;
        }

        //These 3 vectors are used to free unassigned nodes used in the path finding "bfs" that do not lead to a match.
        std::vector<bool> Succesfull_Child((A->M + A->N), 0); //1 if node i is unassigned and is on a path that gives a match.
        std::vector<int> parent((A->M + A->N), -1); //keeps track of the parent of node i
        std::vector<int> Newly_visited; //Newly_visited contains the unassigned nodes that are visited during the bfs from veretx v_i.
      
        //vector used to store vertices
        std::vector<int> queue;
        queue.push_back(v_i);

        //Number of new matches we find for v_i.
        int new_matches_vi = 0;

        //The following integers are used to keep track of length op path from vertex v_i in path finding "bfs".
        //We only look at path with a maximum length equal to length_path.
        int level = 0;
        int x = 1;      //The number of nodes on the previous level.
        int x_new = 0; //counts the number of nodes on this level
        int y = 0;      //Counts the number of nodes of the previous level we have already branched from 

        //Find paths from vertex v_i that end in a vertex with different partial status then node v_i.
        //If vertex v_i is partially assigned to 1 processor.
        if (Partial_Status[v_i] >= 0) {

            while (level != length_path && !queue.empty()) {

                int v_j = queue.front();
                queue.erase(queue.begin(), queue.begin() + 1);
                y += 1;

                std::vector<int> intersect_rowcols = A->Intersecting_RowCol(v_j);

                //Traverse all rowcols that intersect with rowcol v_j.
                for (auto i = intersect_rowcols.begin(); i != intersect_rowcols.end(); i++) {

                    int v_k = *i;

                    //If v_k, is assigned or partially assigned different than to 1 proc., if v_k is partially assigend to the same processor as v_i
                    //or v_k is unassigned but already visited, or v_k/ v_i is already matched to the processor of (the partial assignment)  v_i/v_k
                    //Then we cannot match v_k or use this vertex on a matching path.
                    if (Partial_Status[v_k] < -1 || Partial_Status[v_k] == Partial_Status[v_i] || visited[v_k] == 1 || ( (Partial_Status[v_k] >=  0) && maintain_color_match[v_i][Partial_Status[v_k]]) 
                        || maintain_color_match[v_k][Partial_Status[v_i]]) {
                        continue;
                    }

                    //Else the vertex v_k is unassigned, i.e. partial status =-1, or the vertex v_k is partially assigned to a processor/color with which vertex v_i has not already a match.
                    else {
                        //If v_k is partially assigned to a processor/color with which vertex v_i has not already a match, MATCH!
                        if (Partial_Status[v_k] != -1) {

                            Matched[v_i] = 1;
                            Matched[v_k] = 1;

                            maintain_color_match[v_i][Partial_Status[v_k]] = 1;
                            maintain_color_match[v_k][Partial_Status[v_i]] = 1;
                           
                            no_matches_vi += 1;
                            new_matches_vi += 1;
                            parent[v_k] = v_j;
                            Succesfull_Child[v_j] = 1;
                            int v_l = v_j;

                            while (v_l != v_i) {

                                v_l = parent[v_l];
                                Succesfull_Child[v_l] = 1;
                            }
                        }

                        //Else vertex v_k is unassigned.
                        else {

                            queue.push_back(v_k);
                            visited[v_k] = 1;
                            parent[v_k] = v_j;
                            Newly_visited.push_back(v_k);
                            x_new += 1;
                        }
                    }

                    if (no_matches_vi == max_matches) {
                        break;
                    }
                }

                if (y == x) {
                    level += 1;
                    x = x_new;
                    x_new = 0;
                    y = 0;
                }

                if (no_matches_vi == max_matches) {
                    break;
                }
            }
        }
        //If v_i is partially assigned to 2 processors, so partial state is -2, -4, -5, -6, -8 or lower
        else if (Partial_Status[v_i] == -2 || (Partial_Status[v_i] < -3 && Partial_Status[v_i] != -7)) {  //else if so we don't have to do unnecessary computations if a rowol is partially assigend to 1 proc.
            int  binair_index_vi = -Partial_Status[v_i];
            std::vector<bool> Part_state_vi = Index_and_Status[binair_index_vi];
            std::vector<int> procs_vi = Determine_Set_indices(Part_state_vi);
            std::sort(procs_vi.begin(), procs_vi.end());

            while (level != length_path && !queue.empty()) {

                int v_j = queue.front();
                queue.erase(queue.begin(), queue.begin() + 1);
                y += 1;

                std::vector<int> intersect_rowcols = A->Intersecting_RowCol(v_j);

                //Traverse all rowcols that intersect with rowcol v_j.
                for (auto i = intersect_rowcols.begin(); i != intersect_rowcols.end(); i++) {

                    int v_k = *i;

                    //If v_k is assigned or partially assigned but not partially assigned to one or two processors then v_k cannot be on a matching path, so we skip v_k.
                    if (Partial_Status[v_k] == -3 || Partial_Status[v_k] == -7) {
                        continue;
                    }

                    std::vector<bool> Part_state_vk;
                    std::vector<int> procs_vk;

                    //If v_k partially assigned, determine the processors to which it is partially assigned
                    if (Partial_Status[v_k] < -1) {
                        int binair_index_vk = -Partial_Status[v_k];
                         Part_state_vk = Index_and_Status[binair_index_vk];
                        procs_vk = Determine_Set_indices(Part_state_vk);
                    }
                    else if (Partial_Status[v_k] > -1) {
                        procs_vk.push_back(Partial_Status[v_k]);
                    }
                    else{}

                    std::sort(procs_vk.begin(), procs_vk.end());

                    bool match_sum_vi, match_sum_vk; 
                    match_sum_vi = 0;
                    match_sum_vk = 0;
                    //Determine for v_k and v_i, if they are already matched with a processor that is also in the partial state of v_i respectively v_k.
                    for (auto j_i = procs_vi.begin(); j_i != procs_vi.end(); j_i++) {

                        if (maintain_color_match[v_k][*j_i] == 1) {
                            match_sum_vk = 1;
                            break;
                        }
                    }

                    for (auto j_k = procs_vk.begin(); j_k != procs_vk.end(); j_k++) {

                        if (maintain_color_match[v_i][*j_k] == 1) {
                            match_sum_vi = 1;
                            break;
                        }
                    }
                    //Determine if v_i and v_k "share "a processor, i.e. both partial assigend rowcols  are assigned to the same processor(s).
                    std::vector<int> intersect_procs;
                    std::set_intersection(procs_vi.begin(), procs_vi.end(), procs_vk.begin(), procs_vk.end(), std::back_inserter(intersect_procs) );

                    //If v_k contains a processor of v_i, or v_k unassigend but visited, or v_i/v_k is already matched with a processor of v_k/v_i
                    //Then we cannot use v_k on matching path from v_i.
                    if ( intersect_procs.size() != 0 || visited[v_k] == 1 || match_sum_vi != 0 || match_sum_vk != 0) {
                        continue;
                    }


                    //Else the vertex v_k is unassigned  i.e. partial status =-1, and not visited, 
                    //or the vertex v_k is partially assigned to one or two processors/color with which vertex v_i has not already a match.
                    else {
                        //If v_k is partially assigned to one or two processor/color with which vertex v_i has not already a match, MATCH!
                        if (Partial_Status[v_k] != -1) {

                            Matched[v_i] = 1;
                            Matched[v_k] = 1;

                            //Update the matches of v_k and v_i.
                            for (auto k_1 = procs_vi.begin(); k_1 != procs_vi.end(); k_1++) {

                                maintain_color_match[v_k][*k_1] = 1;
                            }

                            for (auto k_2 = procs_vk.begin(); k_2 != procs_vk.end(); k_2++) {

                                maintain_color_match[v_i][*k_2] = 1;
                            }

                            no_matches_vi += procs_vk.size();
                            new_matches_vi += 1;
                            parent[v_k] = v_j;
                            Succesfull_Child[v_j] = 1;
                            int v_l = v_j;

                            while (v_l != v_i) {

                                v_l = parent[v_l];
                                Succesfull_Child[v_l] = 1;
                            }
                        }

                        //Else vertex v_k is unassigned.
                        else {

                            queue.push_back(v_k);
                            visited[v_k] = 1;
                            parent[v_k] = v_j;
                            Newly_visited.push_back(v_k);
                            x_new += 1;
                        }
                    }

                    if (no_matches_vi == max_matches) {
                        break;
                    }
                }

                if (y == x) {
                    level += 1;
                    x = x_new;
                    x_new = 0;
                    y = 0;
                }

                if (no_matches_vi == max_matches) {
                    break;
                }
            }
        }



    

        //Before we will branch from the next v_i, free all unassigned vertices that were visited, but are not on a "matching" path.
        for (auto i = Newly_visited.begin(); i != Newly_visited.end(); i++) {

            if (Succesfull_Child[*i] != 1) {
                visited[*i] = 0;
            }
        }
        //Add the number of new matches of v_i to the GL4 bound.
        GL4_bound += new_matches_vi;
    }
    // End determining GL4 bound
    
    //If LB+GL4 > = UB then we can return GL4 bound, this partial solution will not improve upon UB
    //else compute L3 and/or GL3.
    int LB_now = lowerbound + GL4_bound;
    if (LB_now < UB) {

        //Adjust the packingsets, if a rowcol is matched in GL4 it cannot be used in the local packing bound L3
        //So set the packingset of this rowcol to 0.
        for (int l = 0; l < (A->M + A->N); l++) {

            if (Matched[l] == 1) {

                if (Partial_Status[l] >= 0) {
                    if (l < A->M) {
                        int index_packingset = Partial_Status[l];
                        int index_partial_status = pow(2, index_packingset) - 1;
    
                        int no_nz = A ->perRow_Col[l] - color_count[index_partial_status][l];
                        Packing_Sets2[0][index_packingset][no_nz] -= 1;

                    }
                    else {
                        int index_packingset = Partial_Status[l];
                        int index_partial_status = pow(2, index_packingset) - 1;

                        int no_nz = A->perRow_Col[l] - color_count[index_partial_status][l];
                        Packing_Sets2[1][index_packingset][no_nz] -= 1;

                    }

                }
            }

        }

        int comboL3 = L3bound_improved(Packing_Sets2, Partition_Size, A);
        
        if ( (LB_now + comboL3 ) >= UB) {
            GL4_bound += comboL3;
            return GL4_bound;
        }

        if (GL3_on) {

            int GL3 = GL3_bound(Matched, visited, Partial_Status,  A, Partition_Size, states);
         
            int max3 = std::max(comboL3, GL3);

            GL4_bound += max3;
        }
    }
    
    return GL4_bound;
}




