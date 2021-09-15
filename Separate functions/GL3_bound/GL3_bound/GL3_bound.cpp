// GL3_bound.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<vector>
#include <stack>
# include<deque>
#include<algorithm>
#include<numeric>


int Processors = 3;
int Max_Partition_size = 6;

int Compute_value_GL3(std::vector<std::vector<int>> Global_PackingSets, std::vector<int> Partition_sizes) {

    int Value_GL3 = 0;

    for (int i = 0; i < Processors; i++) {

        std::vector<int> PackingSet_i = Global_PackingSets[i];

        int still_allowed = Max_Partition_size - Partition_sizes[i]; //To do aanpassen aan info L3
        int sum = std::accumulate(PackingSet_i.begin(), PackingSet_i.end(), 0);

        if (sum < still_allowed) {
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

int GL3_bound(std::vector<bool> Matched_GL4, std::vector<bool> visited_GL4, std::vector<int> Partial_status, int no_rc, std::vector<std::vector<int>> Intersectingrc, std::vector<int> Partition_size) {

    int GL3 = 0;

    //First determine which partially assigned rowcols are not matched in the GL4 bound.
    //Note we only take into account partially assigned rowcols, that are partially assigned to one processor.
    //Store the size of the subgraph and  the index of the partially assigned row in this container.
    std::deque<int> order_subgraphs;

    std::vector<std::stack<int>> subgraph_stacks;
    std::vector<int> subgraph_sizes;
    std::vector<int> index_subgraphs;

    subgraph_stacks.resize(no_rc);
    subgraph_sizes.resize(no_rc);

   
    for (int i = 0; i < no_rc; i++) {
        if (Matched_GL4[i] == 0 && Partial_status[i] >= 0) {

            order_subgraphs.push_back(i);
            subgraph_stacks[i].push(i);
            index_subgraphs.push_back(i);

        }
    }

    std::vector<bool> Claimed(no_rc, 0);

    //Queue of the subgraphs, we start doing dfs for the smallest subgraph.

    while (!order_subgraphs.empty()) {

        //We will now try to expand the subgraph of node/rowcol rc.
        int rc = order_subgraphs.front();
        std::stack<int> stack_v = subgraph_stacks[rc];

        //If stack of veretx v is empty then we cannot expand the subgraph of v, so we go to the next subgraph.
        if (stack_v.empty()) {

            order_subgraphs.pop_front();
            std::cout << "Y";
            continue;
        }
        
        int v_i = stack_v.top();

        //Start expanding subgraph from veretx v_i, if not possible we pop v_i from the stack of subgraph rc.

        bool nonzero_claimed = 0;
        std::vector<int> intersect_rc_vi = Intersectingrc[v_i]; //ToDo naar matrix pointer 
        int no_intersect = intersect_rc_vi.size();

        for (int j = 0; j < no_intersect; j++) {
            std::cout << " x";
            int v_k = intersect_rc_vi[j];

            //If the vertex v_k is assigned or is claimed by another subgraph we can do nothing.
            if (Partial_status[v_k] == -3 ||Claimed[v_k]) {
                continue;
            }

            //If v_k is not assigned and on a flow path in GL4. So partial assigned and matched or visited in GL4 or unassigned and visited in gL4.
            //Then we can take the nonzero, but we cannot expand from vertex v_k.
            //If v_k is partially assigned but not to one processor specifically we can claim the nonzero but not expand from vertex v_k.
            else if ( (Partial_status[v_k] != -3 && (visited_GL4[v_k] || Matched_GL4[v_k] ) ) || Partial_status[v_k]==-2) {


                subgraph_sizes[rc] += 1;
                nonzero_claimed = 1;
            }
            //If v_k is unassigned and not visisted in GL4 and not claimed in another subgraph, we can claim the edge and
            //we can expand from v_k.
            else if (Partial_status[v_k] == -1 && visited_GL4[v_k] == 0 && Claimed[v_k]==0) {

                subgraph_stacks[rc].push(v_k);
                Claimed[v_k] = 1;
                subgraph_sizes[rc] += 1;
                nonzero_claimed = 1;
            }


            if (nonzero_claimed) {
                break;
            }

        }

        if (nonzero_claimed) {

            order_subgraphs.pop_front();
            order_subgraphs.push_back(rc);

        }
        else {
            subgraph_stacks[rc].pop();
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



int main()
{
    std::cout << "Hello World!\n";

    std::vector<bool> match(8, 0);
    int n = 8;
    std::vector<bool> vis(8, 0);
    std::vector<int> Part = { 0,-1,0,-3,-1,-1,-1,-3 };
    std::vector<int> inter1 = { 7,6 };
    std::vector<int> inter2 = { 6,5,4 };
    std::vector<int> inter3 = { 3,4 };
    std::vector<int> iter4 = { 2 };
    std::vector<int> iter5 = { 1,2 };
    std::vector<int> iter6 = { 1 };
    std::vector<int> iter7 = {0, 1 };
    std::vector<int> iter8 = { 0 };
    std::vector<std::vector<int>> iter = { inter1, inter2, inter3, iter4, iter5, iter6, iter7, iter8};
    std::vector<int> Partsize = { 0,0,0 };

    std::cout<<GL3_bound(match, vis, Part, n, iter, Partsize);

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
