#ifndef MATRIX_H
#define MATRIX_H

#include <iomanip>
#include <vector>
#include <iostream>
#include <algorithm> 
#include <limits>

#ifdef _OPENMP
   #include <omp.h>
#else
   #define omp_get_thread_num() 0
#endif

#include "NewDataGraph.hh"

using namespace std;
using namespace NewGraph;

class Matrix {
    private:
        uint32_t** matrix;
        uint32_t num_vertices;
        const uint32_t num_queries;
    public:
        Matrix(const DataGraph& G, const vector<uint32_t>& keywords) : num_vertices(G.get_vertex_count()), num_queries(keywords.size()) {
            matrix = new uint32_t*[num_vertices];

            #pragma omp parallel for
            for(uint32_t i=0; i<num_vertices; i++) {
                matrix[i] = new uint32_t[num_queries];
            }

            #pragma omp parallel for collapse(2)
            for(uint32_t i=0; i<num_vertices; i++) {
                for(uint32_t j=0; j<num_queries; j++) {
                    vector<uint32_t> labels = G.label(i+1);
                    uint32_t keyw = keywords[j];
                    if(find(labels.begin(), labels.end(), keyw) != labels.end()) {
                        matrix[i][j]=0;
                    }
                    else {
                        matrix[i][j]=numeric_limits<uint32_t>::max();
                    }
                }
            }
        }

        uint32_t** get_matrix() {
            return matrix;
        }

        uint32_t get_num_vertices() const{
            return num_vertices;
        }

        uint32_t get_num_queries() const{
            return num_queries;
        }

        void print_matrix() {
            for(uint32_t i=0;i<num_vertices;i++) {
                for(uint32_t j=0; j<num_queries;j++){
                    cout << setw(15) << matrix[i][j];
                }
                cout << "\n";
            }
        }

        ~Matrix() {
            #pragma omp parallel for
            for(uint32_t i=0; i<num_vertices; i++) {
                delete[] matrix[i];
            }
            delete[] matrix;
        }
    
};
#endif