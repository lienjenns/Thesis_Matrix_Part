// Global_L4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<vector>
#include<numeric>
#include<algorithm>

int Processors = 3;

class Tree_GL4 {

    std::vector<bool> visited;
    std::vector<int> no_paths;
    std::vector<std::vector<bool>> color_matches;
    int GL4_bound;

    Tree_GL4(int M, int N);
};

//Construtor of the tree for the global L4 bound
Tree_GL4::Tree_GL4(int M, int N) {

    visited.assign(M + N, 0);
     no_paths.assign(M + N, 0);
     std::vector<bool> colormatch_i(Processors, 0);
     color_matches.assign(M+N,colormatch_i);
    GL4_bound=0;

}

//Constructor of the bipartite graph. 
//Because we use split vertices, we add p-1 vertices to the graph for every rowcol that gets added to the graph .
//Bi_Graph::Bi_Graph(int* x, int* y) {
//
//    adj.resize((Processors - 1) * (*x + *y));
//    In_graph.resize(*x + *y);
//    Match.resize((Processors - 1) * (*x + *y));
//
//    //Initialize Match vector, ie. match[i]=i.
//    std::iota(Match.begin(), Match.end(), 0);
//
//    no_Matched = 0;
//
//    Max_V = (Processors - 1) * (*x + *y);
//}

int BFS_Global_L4(int length_path,  std::vector<int> Partial_Status, int M, int N, std::vector<std::vector<int>> funct_intersect_rowcol) {


    //Total number of matches found by the Global L4 bound.
    int GL4_bound = 0;
    int max_matches = Processors-1;


      //vector used to see if a vertex is visited
        std::vector<bool> visited(M + N, 0);
        

        //Nodig ls je niet elke keer hele bfs wil doen ToDo
        std::vector<int> No_paths(M + N, 0); //nog niet in gebruik ToDo
       
        //During the whole bfs keep up with which different  colors a partial colored rowcol is matchded
        std::vector<bool> Color_match_rc(Processors, 0);
        std::vector<std::vector<bool>> maintain_color_match(M + N, Color_match_rc);


        for (int v_i = 0; v_i < (M + N); v_i++) {



            std::vector<bool> maintain_color_matches_i = maintain_color_match[v_i]; // To-do eigenlijk te lang p-1 kan
     //Number of matches of veretx v_i
            int no_matches_vi = std::accumulate(maintain_color_matches_i.begin(), maintain_color_matches_i.end(), 0);

            if (Partial_Status[v_i] < 0 || visited[v_i] == 1 || no_matches_vi == max_matches) {
                continue;
            }
        
            //Deze 2 worden egbruikt om dingen weer vrij te maken als ze niet gebruikt worden.
            std::vector<bool> Succesfull_Child(M + N, 0);
            std::vector<int> parent(M + N, -1);

            //Vector to maintain the colors/partial statuses of the vertices that match with vertex v_i;
            //vertex v_i can be matched with at most one vertex with color j.
            //std::vector<bool> maintain_color_matches(Processors, 0); // To-do eigenlijk te lang p-1 kan
            ////Number of matches of veretx v_i
            //int no_matches_vi = 0;

            std::vector<int> Newly_visited;

              int new_matches_vi = 0;

            std::vector<int> queue;
            queue.push_back(v_i);
            visited[v_i] = 1;

            //The following integers are used to keep track of length op path from veretx v_i in bfs.
            //We only look at path with a maximum length equal to length_path.
            int level = 0;
            int x = 1;
            int x_new = 0;
            int y=0;
            while (level != length_path && !queue.empty()) {
           
                int v_j = queue.front();
                queue.erase(queue.begin(), queue.begin() + 1);
                 std::cout << v_j << "\n";
                 y += 1;
                 Newly_visited.push_back(v_j);
                 
                std::vector<int> intersect_rowcols = funct_intersect_rowcol[v_j];

                for(auto i=intersect_rowcols.begin(); i!= intersect_rowcols.end(); i++){

                    int v_k = *i;
               

                    if (Partial_Status[v_k] < -1 || Partial_Status[v_k] == Partial_Status[v_i] || visited[v_k] == 1|| (Partial_Status[v_k]>=0 && maintain_color_matches_i[Partial_Status[v_k]])) {
                        continue;
                    }

                    //Else the vertex is unassigne, i.e. partial status =-1, or the vertex is partially assigned to a processor eith ehich it hasn't a match
                    else {
                     std::cout << v_j << " " << v_k<< "\n";
                        if (Partial_Status[v_k] != -1) {


                            //new
                            maintain_color_match[v_i][Partial_Status[v_k]] = 1;
                                maintain_color_match[v_k][Partial_Status[v_i]] = 1;
                            
                            no_matches_vi += 1;
                            new_matches_vi += 1;
                            //visited[v_k] = 1;
                            parent[v_k] = v_j;
                            No_paths[v_k] += 1;
                            No_paths[v_j] += 1;
                            Succesfull_Child[v_j] = 1;
                            int v_l = v_j;
                            while (v_l != v_i) {

                                v_l = parent[v_l]; 
                                Succesfull_Child[v_l] = 1;
                                No_paths[v_l] += 1; 
                            }

                        }

                        else {

                            queue.push_back(v_k);
                            visited[v_k] = 1;
                            parent[v_k] = v_j;
                            x_new += 1;

                        }
                    }

                    if (no_matches_vi == max_matches) {
                        std::cout << "eerder stoppen!";
                        break;

                    }

                }
                if (y == x) {
                    level += 1;
                    x = x_new;
                    x_new = 0;
                    y = 0;
                }

            

                std::cout << "\n" << " level: " << level << "\n";

                if (no_matches_vi == max_matches) {
                    std::cout << "eerder stoppen!";
                    break;

                }
            }
                for (auto i = Newly_visited.begin(); i != Newly_visited.end(); i++) {

                    if (Succesfull_Child[*i] != 1) {
                                    visited[*i] = 0;

                    }

                }


            std::cout << " aantal matches van rijcol " << v_i << " : " << no_matches_vi<< "\n";
            GL4_bound += new_matches_vi;
        }
   
        return GL4_bound;
}




int main()
{
    //Example 1
    std::vector<int> PartState = { 0,1,2,-1,-1,-1 };
    int M = 4;
    int N = 2;
    std::vector<int> inter_r0 = { 4 };
    std::vector<int> inter_r1 = { 4,5 };
    std::vector<int> inter_r2 = { 4,5 };
    std::vector<int> inter_r3 = { 5 };
    std::vector<int> inter_c0 = {0,1,2 };
    std::vector<int> inter_c1 = { 1,2,3 };
   
    std::vector<std::vector<int>> X = { inter_r0, inter_r1, inter_r2,inter_r3, inter_c0 , inter_c1};
    int lengthpath = 3;

   std::cout<< "Global L4 bound: "<<BFS_Global_L4(lengthpath, PartState, M, N, X);

 
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
