#ifndef LCS_HH
#define LCS_HH

#include <vector>
#include <algorithm> 
#include <iostream>
#include <queue>
#include <optional> //https://stackoverflow.com/questions/6822044/checking-if-a-variable-is-initialized

#include "KWSGraph.hh"
#include "PreserveOriginalEdges.hh"

#include "NewGraph.hh"

using namespace std;
using namespace NewGraph;

#define ADD_EMPTY_LABEL false

uint32_t get_num_keywords_from_label(const vector<uint32_t>& label, const vector<uint32_t>& keywords) {
    uint32_t num_of_keywords = 0;
    vector<uint32_t> label_copy = label;
    sort( label_copy.begin(), label_copy.end() );
    label_copy.erase( unique( label_copy.begin(), label_copy.end() ), label_copy.end() );

    for(uint32_t l: label_copy) {
        if(find(keywords.begin(), keywords.end(), l) != keywords.end()) {
            num_of_keywords++;
        }
    }
    return num_of_keywords;
}

void print_level_class(const vector<vector<string>> level_class) {
    cout << "Level class: \n";
    for(uint32_t i=0;i<level_class.size();i++) {
        cout << "length " << i+1 << " [";
        for(uint32_t j=0; j<level_class[i].size();j++) {
            cout << level_class[i][j] << ", ";
        }
        cout << "] \n";
    }
}

uint32_t get_min_keywords_to_preserve(SmallGraph &g, const uint32_t root, const vector<uint32_t>& keywords) {
    uint32_t query_length = keywords.size();
    vector<uint32_t> g_v_list = g.v_list();
    vector<vector<vector<uint32_t>>> level_class(query_length);
    level_class[query_length - 1].push_back(g.label(root));

    uint32_t num_vertices = g.num_vertices();

    for(uint32_t i=0;i<num_vertices;i++) {
        vector<uint32_t> label = g.label(g_v_list[i]);
        uint32_t num_of_keywords = get_num_keywords_from_label(label, keywords);

        if(num_of_keywords>0) {
            level_class[num_of_keywords-1].push_back(label);
        }
    }
    
    int* keywords_to_scan = new int[query_length];
    copy(keywords.begin(),keywords.end(),keywords_to_scan);
    uint32_t min_keywords_to_preserve = 0;

    for(int i=query_length-1; i>=0; i--) {
        for(uint32_t j=0; j<level_class[i].size(); j++){
            vector<uint32_t> label = level_class[i][j];
            for(uint32_t k=0;k<query_length;k++){
                if(keywords_to_scan[k]==-1) {
                    continue;
                }
                for(uint32_t L=0; L<label.size(); L++) {
                    if(keywords_to_scan[k]==(int)label[L]) {
                        keywords_to_scan[k] = -1;
                        break;
                    }
                }

            }
        }
        bool all_elements_empty = true;
        for(uint32_t m=0; m<query_length; m++) {
            if(keywords_to_scan[m]!=-1) {
                all_elements_empty = false;
            }
        }
        if(all_elements_empty) {
            min_keywords_to_preserve = i+1;
            break;
        }
    }

    delete [] keywords_to_scan; 

    return min_keywords_to_preserve;
}

void join_shortest_path(const uint32_t last_node, uint32_t* dist_from_root, bool* added, 
SmallGraph& pruned_graph, const SmallGraph &original_graph, const uint32_t* explored, vector<vector<uint32_t>>& outcoming, unordered_map<int, int>& indexes) {
    queue<uint32_t> frontier;
    vector<uint32_t> og_v_list = original_graph.v_list();
    int ln_i = indexes[last_node];
    frontier.push(ln_i);
    
    while(!frontier.empty()) {
        uint32_t i = frontier.front();
        frontier.pop();
        uint32_t curr = og_v_list[i];
        uint32_t min_cost = dist_from_root[i] - 1;
        vector<uint32_t> neighbours = outcoming[i];
        for(uint32_t j=0;j<neighbours.size();j++) {
            uint32_t neighbour = neighbours[j];
            int n_i = indexes[neighbour];
            if(dist_from_root[n_i]==min_cost && explored[n_i]==1) {
                if(added[n_i]==false) {
                    frontier.push(n_i);
                }
                pruned_graph.add_edge(curr,neighbour, ADD_EMPTY_LABEL);
                pruned_graph.set_label(curr, original_graph.label(curr));
                pruned_graph.set_label(neighbour, original_graph.label(neighbour));
                added[i] = true;
                added[n_i] = true;
            }
        }
    }
}

SmallGraph level_cover_strategy(SmallGraph &g, const uint32_t root, const vector<uint32_t>& keywords,
double* lcs_get_min_kw=nullptr, double* lcs_lcs=nullptr, double* lcs_shortest_path=nullptr) {
    vector<uint32_t> v_list = g.v_list();

    uint32_t min;
    if(lcs_get_min_kw!=nullptr) {
        auto t1_min = utils::get_timestamp();
        min = get_min_keywords_to_preserve(g, root, keywords);
        auto t2_min = utils::get_timestamp();
        lcs_get_min_kw[omp_get_thread_num()] =  lcs_get_min_kw[omp_get_thread_num()] + (t2_min-t1_min);
    }
    else {
        min = get_min_keywords_to_preserve(g, root, keywords);
    }

    uint32_t num_vertices = g.num_vertices();

    SmallGraph pruned_graph;

    queue<uint32_t> frontier;

    bool* added = new bool[num_vertices];
    uint32_t* dist_from_root= new uint32_t[num_vertices];
    uint32_t* explored = new uint32_t[num_vertices];

    for(uint32_t i=0;i<num_vertices;i++) {
        added[i] = false;
        dist_from_root[i] = numeric_limits<uint32_t>::max();
        explored[i] = 0;
    }

    // From KWSGraph.hh, get_indexes returns index of each vertex from the vertex list v_list (e.g. if v_list[1] = vertex 5, index[5] = 1)
    unordered_map<int, int> indexes = get_indexes(v_list); 

    int r_i = indexes[root];
    added[r_i] = true;
    frontier.push(r_i);
    dist_from_root[r_i] = 0;
    explored[r_i] = 1;

    vector<vector<uint32_t>> outcoming(num_vertices);

    optional<utils::timestamp_t> t1_outside_lcs;
    optional<utils::timestamp_t> t1;
    if(lcs_lcs!=nullptr) {
        t1_outside_lcs = utils::get_timestamp();
    }
    while(!frontier.empty()) {
        uint32_t i = frontier.front();
        frontier.pop();
        uint32_t curr = v_list[i];
        vector<uint32_t> curr_labels = g.label(curr);
        uint32_t number_of_keywords = get_num_keywords_from_label(curr_labels,keywords);
        if(number_of_keywords >= min && curr!=root) {
            if(lcs_shortest_path!=nullptr) {
                t1 = utils::get_timestamp();
            }
            join_shortest_path(curr, dist_from_root, added, pruned_graph, g, explored, outcoming, indexes);
            if(lcs_shortest_path!=nullptr && t1) {
                auto t2 = utils::get_timestamp();
                lcs_shortest_path[omp_get_thread_num()] = lcs_shortest_path[omp_get_thread_num()] + (t2-*t1);
            }
        }
        vector<uint32_t> neighbours = g.get_neighbours(curr);
        for(uint32_t j=0;j<neighbours.size();j++) {
            uint32_t neighbour = neighbours[j];
            int n_i = indexes[neighbour];
            if(explored[n_i]==0) {
                explored[n_i] = 1;
                frontier.push(n_i);  
            }
            outcoming[n_i].push_back(curr);
            if(dist_from_root[n_i]==numeric_limits<uint32_t>::max()) {
                dist_from_root[n_i] = dist_from_root[i] + 1;
            }
        }
    }
    if(lcs_lcs!=nullptr && t1_outside_lcs) {
        auto t2_outside_lcs = utils::get_timestamp();
        lcs_lcs[omp_get_thread_num()] =  lcs_lcs[omp_get_thread_num()] + (t2_outside_lcs-*t1_outside_lcs);
    }
    

    if(pruned_graph.num_vertices()==0) {
        pruned_graph.add_node(root,ADD_EMPTY_LABEL).set_label(root,g.label(root));
    }


    delete [] added;
    delete [] dist_from_root;
    delete [] explored;

    return pruned_graph;
}

#endif