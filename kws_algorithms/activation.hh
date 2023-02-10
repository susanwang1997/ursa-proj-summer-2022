#ifndef ACTV_H
#define ACTV_H

#include <cmath>

#ifdef _OPENMP
   #include <omp.h>
#else
   #define omp_get_thread_num() 0
#endif

#include "weights.hh"

#include "NewDataGraph.hh"

using namespace std;
using namespace NewGraph;

class Activation {
    private:
        uint32_t* activation;
        double* act_weights;
        uint32_t length;
        
    public:
        Activation(const DataGraph& G, const double alpha, const double average_shortest_dist) : length(G.get_vertex_count()) {
            activation = new uint32_t[length];
            act_weights = new double[length];

            double A_bar = average_shortest_dist;

            Weights w(G);

            double* weights = w.get_weights();
            // cout << "Weights: ";
            // w.print_weights();
            
            #pragma omp parallel for
            for(uint32_t i=0; i<length; i++) {
                double rounded_weight = round(weights[i] * 10.0) / 10.0;
                double weight = weights[i];
                act_weights[i] = weight;
                
                if(rounded_weight<alpha){
                    double reward = A_bar * ((alpha - weight)/(alpha));
                    double a = round(A_bar - reward);
                    activation[i] = a;
                }
                else if(rounded_weight==alpha){
                    double a = round(A_bar);
                    activation[i] = a;
                }
                else if(rounded_weight>alpha){
                    double penalty = A_bar * ((weight-alpha)/(1-alpha));
                    double a = round(A_bar + penalty);
                    activation[i] = a;
                }
            }
        }

        uint32_t* get_activation() const {
            return activation;
        }

        double* get_weights() const {
            return act_weights;
        }

        uint32_t get_length() const {
            return length;
        }

        void print_activation() {
            for(uint32_t i=0; i<length; i++) {
                cout << activation[i] << " ";
            }
            cout << "\n";
        }

        ~Activation() {
            delete[] activation;
            delete[] act_weights;
        }
};

#endif