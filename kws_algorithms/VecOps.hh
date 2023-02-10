#ifndef VEC_OPS_H
#define VEC_OPS_H

#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

string uint32t_vec_to_str(const vector<uint32_t> v);

vector<uint32_t> str_to_uint32t_vec(const string str);

double average_from_double_vec(const vector<double>* v);

uint32_t max_from_uint32t_vec(const vector<uint32_t>* v);

uint32_t min_from_uint32t_vec(const vector<uint32_t>* v);

#endif