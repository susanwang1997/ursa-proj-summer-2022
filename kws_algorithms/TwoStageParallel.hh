#ifndef TSP_H
#define TSP_H

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm> 

#ifdef _OPENMP
   #include <omp.h>
#else
   #define omp_get_thread_num() 0
#endif

#include "Expansion.hh"
#include "TopDownProcessing.hh"
#include "AvgShortestDistance.hh"
#include "KWSGraph.hh"

#include "keywords.hh"
#include "Matrix.hh"
#include "activation.hh"

#include "NewDataGraph.hh"

using namespace std;
using namespace NewGraph;

#include "TwoStageParallel.hh"

void print_graph_info(const DataGraph& G) { //to debug
    uint32_t num_of_v = G.get_vertex_count();
    for(uint32_t i=1;i<=num_of_v;i++) {
        cout << "adjlist[" << i << "] = [ ";
        for(uint32_t j=0 ; j<G.get_adj(i).length ; j++) {
            cout << *(G.get_adj(i).ptr + j) << " ";
        }
        cout << "] \n";
        cout << "label[" << i << "] = ";
        for(auto l: G.label(i)) {
            cout << l << " ";
        } 
        cout << "\n";
    }

    cout << "number of vertices " << num_of_v << "\n";
    cout << "number of edges " << G.get_edge_count() << "\n";
    cout << endl;
}

void initalize_identifiers(const DataGraph& G, uint32_t* CIdentifier, uint32_t* FIdentifier, const uint32_t num_of_v, Keywords& keywords) {
    //If the index of FIdentifier + 1 corresponds to a node id with a label that contains a keyword, set to 1, otherwise 0
    #pragma omp parallel for
    for(uint32_t i=0; i<num_of_v;i++) { 
        vector<uint32_t> labels = G.label(i+1);
        bool contains_keyword = keywords.contains_keyword(labels);
        if(contains_keyword==true){
            FIdentifier[i] = 1;
        }
        else {
            FIdentifier[i] = 0;
        }
        CIdentifier[i] = 0;
    }
}

void identify_central_nodes(uint32_t* CIdentifier, uint32_t** M, const uint32_t* frontiers, const uint32_t num_of_v, const uint32_t num_of_q, const uint32_t max_depth) {
    #pragma omp parallel for
    for(uint32_t i=0; i<num_of_v; i++) {
        if(frontiers[i]==1) {
            CIdentifier[i] = 1;
        }
    }

    #pragma omp parallel for collapse(2)
    for(uint32_t i=0; i<num_of_v; i++) {
        for(uint32_t j=0; j<num_of_q; j++) {
            if(frontiers[i]==1) {
                if(M[i][j]==numeric_limits<uint32_t>::max() || M[i][j]>max_depth) {
                    CIdentifier[i] = 0;
                }
            }
        }
    }
}


void clear_fidentifier(uint32_t* FIdentifier, const uint32_t num_of_v) {
    #pragma omp parallel for
    for(uint32_t i=0;i<num_of_v;i++) {
        FIdentifier[i] = 0;
    }
}

// Nullptr as default for vector: https://stackoverflow.com/questions/39391197/c-optional-stdvector-argument-for-function
vector<KWSGraph> get_heap(DataGraph &G, const vector<uint32_t>& query, double alpha=0.5, const uint32_t heap_size=20, const int max_depth=10, const bool vertex_induced=true,  const string graph_path="",
vector<double>* intialization_time_arr=nullptr,
vector<double>* enqueue_frontier_time_arr=nullptr,
vector<double>* identify_cn_time_arr=nullptr,
vector<double>* expansion_time_arr=nullptr,
vector<double>* tdp_time_arr=nullptr) {

    if(!G.is_labelled()) {
        throw invalid_argument("Graph is not labelled\n");
    }

    auto t1_i = utils::get_timestamp();    
    Keywords keywords(query);
    
    Matrix M(G, query); //will be size |V| x |q| = num of vertices * num of keywords in the query

    // auto asd_t1 = utils::get_timestamp();
    double A_bar = average_shortest_distance(G, graph_path);
    // auto asd_t2 = utils::get_timestamp();
    // utils::Log{} << "---------\nTwo Stage Parallel time overview:\n";
    // utils::Log{} << "Average shortest distance finished after " << (asd_t2-asd_t1)/1e6 << "s" << "\n";

    Activation a(G,alpha, A_bar);

    uint32_t num_of_v = G.get_vertex_count();
    uint32_t num_of_q = keywords.get_length();

    uint32_t* frontiers = new uint32_t[num_of_v];
    uint32_t* FIdentifier = new uint32_t[num_of_v];
    uint32_t* CIdentifier = new uint32_t[num_of_v];

    initalize_identifiers(G, CIdentifier,FIdentifier,num_of_v,keywords);
    int BFSLevel = 0;

    double initalization_time = 0.0;
    double enqueue_frontier_time = 0.0;
    double expansion_time = 0.0;
    double identify_central_nodes_time = 0.0;
    double tdp_time = 0.0;

    auto t2_i = utils::get_timestamp(); 
    initalization_time = (t2_i-t1_i)/1e6;
    if(intialization_time_arr!=nullptr) {
        intialization_time_arr->push_back(initalization_time);
    }
    utils::Log{} << "Initialization of variables for two stage parallel finished after " << initalization_time << "s" << "\n";

    auto t1 = utils::get_timestamp();
    while(BFSLevel < max_depth) {
        // cout << "Level " << BFSLevel << "\n";
        // cout << "Matrix: \n";
        // M.print_matrix();
        // cout << "---\n";
        auto t1_enqueue = utils::get_timestamp();
        copy(FIdentifier, FIdentifier+num_of_v, frontiers);
        auto t2_enqeue = utils::get_timestamp();
        enqueue_frontier_time = enqueue_frontier_time + (t2_enqeue - t1_enqueue);
        clear_fidentifier(FIdentifier,num_of_v);
        auto t1_cn = utils::get_timestamp();
        identify_central_nodes(CIdentifier,M.get_matrix(),frontiers,num_of_v,num_of_q, max_depth);
        auto t2_cn = utils::get_timestamp();
        identify_central_nodes_time = identify_central_nodes_time + (t2_cn-t1_cn);
        auto t1_e = utils::get_timestamp();
        expansion(G, M.get_matrix(),frontiers, CIdentifier, FIdentifier, a.get_activation(),keywords,BFSLevel,num_of_v, num_of_q);
        auto t2_e = utils::get_timestamp();
        expansion_time = expansion_time + (t2_e-t1_e);
        // cout << "Level " << BFSLevel << "\n";
        // cout << "Matrix: \n";
        // M.print_matrix();
        // cout << "---\n";
        BFSLevel++;
    }

    auto t1_enqueue = utils::get_timestamp();
    copy(FIdentifier, FIdentifier+num_of_v, frontiers);
    auto t2_enqeue = utils::get_timestamp();
    enqueue_frontier_time = enqueue_frontier_time + (t2_enqeue - t1_enqueue);
    enqueue_frontier_time = enqueue_frontier_time/1e6;
    if(enqueue_frontier_time_arr!=nullptr) {
        enqueue_frontier_time_arr->push_back(enqueue_frontier_time);
    }
    utils::Log{} << "Total time to enqueue frontiers is " << enqueue_frontier_time << "s" << "\n";

    auto t1_cn2 = utils::get_timestamp();
    identify_central_nodes(CIdentifier,M.get_matrix(),frontiers,num_of_v,num_of_q, max_depth);
    auto t2_cn2 = utils::get_timestamp();
    identify_central_nodes_time = identify_central_nodes_time + (t2_cn2-t1_cn2);
    identify_central_nodes_time = identify_central_nodes_time/1e6;
    if(identify_cn_time_arr!=nullptr) {
        identify_cn_time_arr->push_back(identify_central_nodes_time);
    }
    utils::Log{} << "Total time to identify central nodes is " << identify_central_nodes_time << "s" << "\n";

    expansion_time = expansion_time/1e6;
    if(expansion_time_arr!=nullptr) {
        expansion_time_arr->push_back(expansion_time);
    }
    utils::Log{} << "Total expansion time is " << expansion_time << "s" << "\n";
    

    auto t2 = utils::get_timestamp();
    utils::Log{} << "Two stage parallel loop finished after " << (t2-t1)/1e6 << "s" << "\n";
    utils::Log{} << "---------" << "\n";
    // cout << "CIdentifier: ";
    // for(uint32_t i=0;i<num_of_v;i++) {
    //     cout << CIdentifier[i] << " ";
    // }
    // cout << "\n";
    // cout << "Activation: ";
    // a.print_activation();
    // cout << "Matrix: \n";
    // M.print_matrix();
    t1 = utils::get_timestamp();
    vector<KWSGraph> heap = top_down_processing(G,M.get_matrix(),CIdentifier, a.get_weights(), a.get_activation(), keywords, heap_size, num_of_v, num_of_q, vertex_induced);
    t2 = utils::get_timestamp();
    tdp_time = (t2-t1)/1e6;
    if(tdp_time_arr!=nullptr) {
        tdp_time_arr->push_back(tdp_time);
    }
    utils::Log{} << "---------" << "\n";
    utils::Log{} << "Total time for all algorithms (i.e. top down, lcs, preserve edges) in top down processing function " << tdp_time << "s" << "\n";
    utils::Log{} << "---------" << "\n";
    utils::Log{} << "#THREADS: " << omp_get_max_threads() << "\n";


    delete [] FIdentifier;
    delete [] CIdentifier;
    delete [] frontiers;
    return heap;
}

void print_heap(vector<KWSGraph> heap, bool print_edges=true) { //print_edges is a parameter which sets whether the edges of the graphs in the heap should be printed
    cout << "RESULTS: \n";
    cout << "---------\n";
    if(heap.size()==0) {
        cout << "Empty heap. No results found.\n";
        cout << "---------\n";
    }
    else {
        for(size_t i=0; i<heap.size(); i++){
            heap[i].print_graph(print_edges);
            cout << "---------\n";
        }
    }
}

#endif