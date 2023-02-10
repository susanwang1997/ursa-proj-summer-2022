#ifndef WEIGHTS_H
#define WEIGHTS_H

#include <iostream>
#include <vector>
#include <algorithm> 
#include <cmath>

#ifdef _OPENMP
   #include <omp.h>
#else
   #define omp_get_thread_num() 0
#endif

#include "NewGraph.hh"
#include "NewDataGraph.hh"


using namespace std;
using namespace NewGraph;

class Weights {
    private:
        double* weights;
        uint32_t length;
        void compute_weights(const DataGraph& G) {
                weights = new double[length];

                #pragma omp parallel for
                for(uint32_t i=0; i<length; i++) {
                    //Since edges can't be labelled in Peregrine, will assume that for a node v_i, all incident edges have the same label
                    uint32_t r_bar;
                    r_bar = G.get_adj(i+1).length;
                    double numerator = r_bar*log2(1 + r_bar);
                    double denominator = r_bar;
                    if(r_bar!=0) {
                        weights[i] = numerator/denominator;
                    }
                    else {
                        weights[i]=0;
                    }
                }

                double min = *min_element(weights, weights + length);
                double max = *max_element(weights, weights + length);
                if(min!=max) {
                    for(uint32_t i=0; i<length; i++) {
                        weights[i] = (weights[i] - min)/(max-min);
                    }
                }
                else {
                    for(uint32_t i=0; i<length; i++) {
                        weights[i] = 1;
                    }
                }
            }
    public:
        Weights(const DataGraph &G) : length(G.get_vertex_count()) {
            compute_weights(G);
        }

        double* get_weights() const {
            return weights;
        }

        uint32_t get_length() const {
            return length;
        }

        void print_weights() {
            for(uint32_t i=0;i<length;i++) {
                cout << weights[i] << " ";
            }
            cout << "\n";
        }

        ~Weights() {
            delete [] weights;
        }
};

#endif