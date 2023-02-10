#ifndef PRESERVE_OG_EDGE_H
#define PRESERVE_OG_EDGE_H

#include <algorithm> 
#include <vector>

#include "NewDataGraph.hh"
#include "NewGraph.hh"

using namespace std;
using namespace NewGraph;

#define ADD_EMPTY_LABEL false

void preserve_original_edges_from_DataGraph(const DataGraph& original_graph, SmallGraph &sub_graph, double* preserve_edge_insert_time=nullptr) {
    vector<uint32_t> sg_v_list = sub_graph.v_list();
    vector<uint32_t> sg_neighbours;
    vector<uint32_t> intersection;
    vector<uint32_t> difference;
    uint32_t curr;
    for(uint32_t i=0;i<sg_v_list.size();i++) {
        curr = sg_v_list[i];
        set_intersection(original_graph.get_adj(curr).ptr, original_graph.get_adj(curr).ptr + original_graph.get_adj(curr).length, sg_v_list.begin(), sg_v_list.end(), back_inserter(intersection));
        if(intersection.empty()) {
            continue;
        }
        sg_neighbours = sub_graph.get_neighbours(curr);
        sort(sg_neighbours.begin(),sg_neighbours.end());
        set_difference(intersection.begin(),intersection.end(), sg_neighbours.begin(),sg_neighbours.end(), back_inserter(difference));
        if(difference.empty()) {
            intersection.clear();
            continue;
        }
        auto t1 = utils::get_timestamp();
        sub_graph.add_multiple_edges(curr,difference,ADD_EMPTY_LABEL);
        auto t2 = utils::get_timestamp();
        if(preserve_edge_insert_time!=nullptr) {
            preserve_edge_insert_time[omp_get_thread_num()] = preserve_edge_insert_time[omp_get_thread_num()] + (t2-t1);
        }
        intersection.clear();
        difference.clear();
    }
}

void preserve_original_edges_from_SmallGraph(const SmallGraph& original_graph, SmallGraph& sub_graph) {
    vector<uint32_t> sg_v_list = sub_graph.v_list();
    vector<uint32_t> sg_neighbours;
    vector<uint32_t> intersection;
    vector<uint32_t> difference;
    uint32_t curr;
    vector<uint32_t> og_neighbours;
    for(uint32_t i=0;i<sg_v_list.size();i++) {
        curr = sg_v_list[i];
        og_neighbours = original_graph.get_neighbours(curr);
        sort(og_neighbours.begin(),og_neighbours.end());
        set_intersection(og_neighbours.begin(), og_neighbours.end(), sg_v_list.begin(), sg_v_list.end(), back_inserter(intersection));
        if(intersection.empty()) {
            continue;
        }
        sg_neighbours = sub_graph.get_neighbours(curr);
        sort(sg_neighbours.begin(),sg_neighbours.end());
        set_difference(intersection.begin(),intersection.end(), sg_neighbours.begin(),sg_neighbours.end(), back_inserter(difference));
        if(difference.empty()) {
            intersection.clear();
            continue;
        }
        sub_graph.add_multiple_edges(curr,difference,ADD_EMPTY_LABEL);
        intersection.clear();
        difference.clear();
    }
}

#endif


