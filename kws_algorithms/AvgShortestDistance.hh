#ifndef ASD_H
#define ASD_H

#include "NewDataGraph.hh"
using namespace NewGraph;
using namespace std;

bool is_infinity(const int n);
bool node_j_adjacent_to_i(const DataGraph& G, const uint32_t& i, const uint32_t& j);
void print_cost_matrix(const vector<vector<int>> cost_matrix);
template<class bidiiter>
bidiiter random_unique(bidiiter begin, bidiiter end, size_t num_random);
vector<vector<int>> cost_matrix(const DataGraph& G);
double average_shortest_distance(const DataGraph& G, const string filepath="");


#endif

