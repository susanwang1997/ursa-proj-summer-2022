#ifndef KWSGRAPH_H
#define KWSGRAPH_H

#include <assert.h> 
#include <iostream>
#include <algorithm> 

#include "weights.hh"

#include "NewGraph.hh"

#include "VecOps.hh"

using namespace std;
using namespace NewGraph;

#define LAMBDA 0.2

double compute_score(const SmallGraph& g, const double* weights, const uint32_t depth) {
    vector<uint32_t> v_list = g.v_list();
            
    double sum_of_weights = 0;

    for(auto id: v_list) {
        uint32_t index = id - 1;
        sum_of_weights = sum_of_weights + weights[index];
    }
    return pow(depth,LAMBDA) * sum_of_weights;
}

unordered_map<int, int> get_indexes(vector<uint32_t> v_list) {
    unordered_map<int, int> indexes;
    for(uint32_t i=0;i<v_list.size();i++) {
        indexes[v_list[i]] = i;
    }
    return indexes;
}

class KWSGraph {
    private:
        double score;
        uint32_t depth;
        uint32_t root;
        vector<uint32_t> v_list;
        SmallGraph graph;

    public:
        KWSGraph(const SmallGraph& g, const double* weights, const uint32_t depth, const uint32_t root) { 
            graph = g;
            this->root = root;
            this->depth = depth;
            v_list = g.v_list();
            score = compute_score(g,weights,depth);
        }

        KWSGraph(const SmallGraph& g, const double score, const uint32_t depth, const uint32_t root) { 
            graph = g;
            this->root = root;
            this->depth = depth;
            v_list = g.v_list();
            this->score = score;
        }

        SmallGraph get_graph() const {
            return graph;
        }

        uint32_t get_num_vertices() const {
            return graph.num_vertices();
        }

        uint32_t get_num_edges() const {
            return graph.num_true_edges();
        }

        uint32_t get_root() const {
            return root;
        }

        uint32_t get_depth() const {
            return depth;
        }

        vector<uint32_t> get_v_list() const {
            return v_list;
        }

        double get_score() const {
            return score;
        }
        
        bool operator==(KWSGraph &kgraph) const //used for testing purposes
        {
            bool result = true;
            if(score!=kgraph.get_score()) {
                cout << "score is not equal\n";
                result = false;
            }
            if(root!=kgraph.get_root()) {
                cout << "root is not equal\n";
                result = false;
            }
            if(depth!=kgraph.get_depth()) {
                cout << "depth is not equal\n";
                result = false;
            }
            if(graph.num_vertices()!=kgraph.get_graph().num_vertices()) {
                cout << "num vertices is not equal\n";
                result = false;
                return result;
            }
            if(graph.num_true_edges()!=kgraph.get_graph().num_true_edges()) {
                cout << "num true edges is not equal\n";
                result = false;
                return result;
            }

            for(uint32_t i=0;i<v_list.size();i++) {
                uint32_t curr = v_list[i];
                vector<uint32_t> this_label = graph.label(curr);
                vector<uint32_t> kgraph_label = kgraph.get_graph().label(curr);
                if(v_list[i]!=kgraph.get_v_list()[i]) {
                    cout << "vertices are not equal\n";
                    result = false;
                    break;
                }
                if(this_label!=kgraph_label) {
                    cout << "labels are not equal\n";
                    result = false;
                    break;
                }
                if(graph.get_neighbours(curr).size()!=kgraph.get_graph().get_neighbours(curr).size()) {
                    cout << "neighbour size not equal\n";
                    result = false;
                    break;
                }
                vector<uint32_t> graph_neighbours = graph.get_neighbours(curr);
                vector<uint32_t> kgraph_neighbours = kgraph.get_graph().get_neighbours(curr);
                sort( graph_neighbours.begin(), graph_neighbours.end() );

                sort( kgraph_neighbours.begin(), kgraph_neighbours.end() );

                for(uint32_t j=0;j<graph.get_neighbours(curr).size();j++) {
                    if(graph_neighbours[j]!=kgraph_neighbours[j]) {
                        cout << "neighbours not equal\n";
                        result = false;
                        break;
                    }
                }
            }
            return result;
        }
        

        void print_graph(bool print_edges=true) {
            uint32_t num_vertices = graph.num_vertices();
            cout << "score: " << score << "\n";
            cout << "central node: " << root << "\n";
            cout << "vertex size: " << num_vertices << "\n";
            uint32_t edge_size = graph.num_true_edges();
            cout << "edge size: " << edge_size << "\n";
            if(print_edges) {
                uint32_t num_of_edges = 0;
                unordered_map<int,int> indexes = get_indexes(graph.v_list());
                if(graph.num_vertices()>0) {
                    vector<vector<uint32_t>> explored(num_vertices, vector<uint32_t>(num_vertices, 0));
                    queue<uint32_t> q;
                    queue<uint32_t> qp; //parent of q in the queue
                    q.push(root);
                    while(!q.empty()) {
                        if(qp.empty()) {
                            cout << "(id=" << q.front() << ", label=" << uint32t_vec_to_str(graph.label(q.front())) << ")\n";
                        }
                        else {
                            num_of_edges++;
                            cout << "(id=" << q.front() << ", parent_id=" << qp.front() << ", label=" << uint32t_vec_to_str(graph.label(q.front())) << ")\n";
                            qp.pop();
                        }
                        for(uint32_t i=0; i<graph.get_neighbours(q.front()).size(); i++) {
                            uint32_t neighbour = graph.get_neighbours(q.front())[i];
                            int neighbour_index = indexes[neighbour];
                            int curr_index = indexes[q.front()];
                            if(explored[curr_index][neighbour_index]!=1) {
                                explored[curr_index][neighbour_index] = 1;
                                explored[neighbour_index][curr_index] = 1;
                                qp.push(q.front());
                                q.push(neighbour);
                            }
                        }
                        q.pop();
                    }
                }
                assert(num_of_edges == edge_size);
            }
        }
};

bool heap_score_is_desc(vector<KWSGraph> heap) { //check if heap is sorted by score in descending order
    for(uint32_t i=0;i<heap.size();i++) {
        if(i>0) {
            if(heap[i-1].get_score()<heap[i].get_score()) {
                return false;
            }
            if(heap[i-1].get_score()==heap[i].get_score()) {
                if(heap[i-1].get_root() < heap[i].get_root()) {
                    return false;
                }
            }
        }
    }
    return true;
}

#endif