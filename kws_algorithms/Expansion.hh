#ifndef EXPANSION_H
#define EXPANSION_H

#include <limits>

#ifdef _OPENMP
   #include <omp.h>
#else
   #define omp_get_thread_num() 0
#endif


#include "NewDataGraph.hh"
#include "keywords.hh"

using namespace std;
using namespace NewGraph;

void expansion(const DataGraph& G, uint32_t** M, const uint32_t* frontiers, const uint32_t* CIdentifier, uint32_t* FIdentifier, const uint32_t* activation, const Keywords& keywords, const uint32_t level, const uint32_t vertex_length, const uint32_t query_length) {
    // #pragma omp parallel for schedule(dynamic) //noticed that dynamic scheduling incured overhead so removed it
    #pragma omp parallel for
    for(uint32_t i=0; i<vertex_length; i++) {
        if(frontiers[i] == 1) {
            uint32_t f_i = i;
            if(CIdentifier[f_i]==1){
                continue;
            }
            if(activation[f_i]>level) {
                FIdentifier[f_i]=1;
                continue;
            }
            for(uint32_t q=0;q<query_length;q++) {
                uint32_t hf = M[f_i][q];
                if(hf>level){
                    continue;
                }
                for(uint32_t j=0;j<G.get_adj(f_i+1).length;j++) {
                    uint32_t n = *(G.get_adj(f_i+1).ptr + j);
                    uint32_t hn = M[n-1][q];
                    if(hn!=numeric_limits<uint32_t>::max()) {
                        continue;
                    } 
                    vector<uint32_t> labels = G.label(n);
                    if(!keywords.contains_keyword(labels)){
                        if(activation[n-1]>level+1) {
                            FIdentifier[f_i] = 1;
                            continue;
                        }
                    }
                    M[n-1][q] = level+1;
                    FIdentifier[n-1] = 1;
                }
            }
        }
    }  
}

#endif