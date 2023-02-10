#include <fstream>
#include <filesystem>
#include <vector>
#include <limits>
#include <algorithm> 
#include <assert.h> 

#ifdef _OPENMP
   #include <omp.h>
#else
   #define omp_get_thread_num() 0
#endif

#include "NewDataGraph.hh"
#include "AvgShortestDistance.hh"

using namespace NewGraph;
using namespace std;

int infinity = numeric_limits<int>::max();

bool is_infinity(const int n){
    // return (n == numeric_limits<int>::max()) || n<0; //n<0 is to check overflow if you add infinity by a number
    return (n == infinity) || (n < 0);
}

bool node_j_adjacent_to_i(const DataGraph& G, const uint32_t& i, const uint32_t& j) {
    bool result = false;
    for(uint32_t k=0; k<G.get_adj(i).length; k++) {
        uint32_t adj = *(G.get_adj(i).ptr + k);
        if(j == adj) {
            result = true;
        }
    }
    return result;
}

void print_cost_matrix(const vector<vector<int>> cost_matrix) {
    cout << "Cost matrix: \n";
    for(uint32_t i = 0; i<cost_matrix.size(); i++){
        for(uint32_t j = 0; j<cost_matrix[i].size(); j++)
            cout << setw(15) << cost_matrix[i][j];
        cout << endl;
    }
    cout << endl;
}

// Source: https://ideone.com/3A3cv
// https://stackoverflow.com/questions/9345087/choose-m-elements-randomly-from-a-vector-containing-n-elements
template<class bidiiter>
bidiiter random_unique(bidiiter begin, bidiiter end, size_t num_random) {
    size_t left = std::distance(begin, end);
    while (num_random--) {
        bidiiter r = begin;
        std::advance(r, rand()%left);
        std::swap(*begin, *r);
        ++begin;
        --left;
    }
    return begin;
}

vector<vector<int>> cost_matrix(const DataGraph& G) {
    bool vertex_size_over_ten_thousand = G.get_vertex_count()>MAX_VERTEX;
    vector<uint32_t> vertices_to_sample;
    uint32_t num_vertices = G.get_vertex_count();
    if(vertex_size_over_ten_thousand) {
        //Random sampling done on cost matrix
        //hist[0] will hold vertices with degrees 0-99
        //hist[1] will hold vertices with degrees 100-199
        //hist[2] will hold vertices with degrees 200-299
        //hist[10] will hold vertices will degrees 1000+
        uint32_t hist_size = 11;
        vector<vector<uint32_t>> hist(hist_size);
        #pragma omp parallel for
        for(uint32_t i=0;i<G.get_vertex_count();i++) {
            uint32_t node = i+1;
            uint32_t degree = G.get_adj(node).length;
            uint32_t index = degree/100;
            if(index>10) {
                index = 10;
            }
            #pragma omp critical
            {
                hist[index].push_back(node);
            }
        }

        double percentage_to_sample;
        percentage_to_sample = 10000.0/G.get_vertex_count();

        srand(1);
        #pragma omp parallel for
        for(uint32_t i=0;i<hist_size;i++) {
            vector<uint32_t> vertices_in_curr_hist = hist[i];
            double curr_hist_size = vertices_in_curr_hist.size();
            uint32_t num_to_sample_in_curr_hist = round(curr_hist_size * percentage_to_sample);
            random_unique(vertices_in_curr_hist.begin(),vertices_in_curr_hist.end(),num_to_sample_in_curr_hist);
            #pragma omp critical
            {
                vertices_to_sample.insert(vertices_to_sample.end(),vertices_in_curr_hist.begin(),vertices_in_curr_hist.begin() + num_to_sample_in_curr_hist);
            }
        }
        num_vertices = vertices_to_sample.size();
    }
    sort(vertices_to_sample.begin(),vertices_to_sample.end());

    vector<vector<int>> cost_matrix;

    for(uint32_t i = 0; i < num_vertices; i++)
    {
        vector<int> temp(num_vertices);
        cost_matrix.push_back(temp);
    }

    #pragma omp for collapse(2)
    for(uint32_t i=0;i<num_vertices;i++){
        for(uint32_t j=0;j<num_vertices;j++){
            uint32_t u;
            uint32_t v;
            if(vertex_size_over_ten_thousand) {
                u = vertices_to_sample[i];
                v = vertices_to_sample[j];
            }
            else {
                u = i+1;
                v = j+1;
            }
            if(i==j) {
                cost_matrix[i][j] = 0;
            }
            else if(node_j_adjacent_to_i(G, u, v)){
                cost_matrix[i][j] = 1;
            }
            else {
                cost_matrix[i][j] = infinity;
            }
        }
    }

    for(uint32_t k = 0; k<num_vertices; k++) {
        #pragma omp parallel for collapse(2)
        for(uint32_t i = 0; i<num_vertices; i++){
            for(uint32_t j = 0; j<num_vertices; j++) {
                if(cost_matrix[i][k]+cost_matrix[k][j] < cost_matrix[i][j]) {
                        if(is_infinity(cost_matrix[i][k]) || is_infinity(cost_matrix[k][j]) || is_infinity(cost_matrix[i][k]+cost_matrix[k][j])) {
                            //do nothing
                        }
                        else {
                            cost_matrix[i][j] = cost_matrix[i][k]+cost_matrix[k][j];
                        }
                } 
            }
        }
    }
    return cost_matrix;
}



//https://www.tutorialspoint.com/all-pairs-shortest-paths
double average_shortest_distance(const DataGraph& G, const string filepath) {

    if(filepath!="") {
        if (filesystem::exists(filepath + "/asd/avg_shortest_dist.txt")) {
            ifstream f(filepath + "/asd/avg_shortest_dist.txt");
            double avg;
            if(f >> avg) {
            // do nothing 
            }
            else {
                cout << "error: cannot read file to get the average shortest distance\n";
            }
            f.close();
            return avg;
        }
    }

    uint32_t num_vertices = G.get_vertex_count();
    assert(num_vertices!=0);
    
    vector<vector<int>> cm = cost_matrix(G);
    if (num_vertices>MAX_VERTEX)
    {
        num_vertices = cm.size();
    }
   
    //print_cost_matrix(cm);
    
    vector<double> avg(num_vertices, 0);
    vector<double> vertices_to_add(num_vertices, 0);

    for(uint32_t i = 0; i<num_vertices; i++) {
        for(uint32_t j = 0; j<num_vertices; j++){
            if(!is_infinity(cm[i][j]) && cm[i][j]>0) {
                avg[i] = avg[i] + cm[i][j];
                vertices_to_add[i] = vertices_to_add[i] + 1;
            }
        }
    }

    #pragma omp parallel for
    for(uint32_t i = 0; i<num_vertices; i++) {
        if(vertices_to_add[i]!=0) {
            avg[i] = avg[i]/(vertices_to_add[i]);
        }
        else {
            avg[i] = 0;
        }
    }

    double sum = 0;
    for(uint32_t i = 0; i<num_vertices; i++) {
        sum = sum + avg[i];
    }

    double ans = sum/num_vertices;

    if(filepath!="") {
        filesystem::create_directories(filepath + "/asd");
        ofstream f(filepath + "/asd/avg_shortest_dist.txt");
        f << ans;
        f.close();
    }

    return ans;
}