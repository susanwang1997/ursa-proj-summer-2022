#include "VecOps.hh"

#include <numeric>
#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

string uint32t_vec_to_str(const vector<uint32_t> v) {
    string s = "[";
    for(auto& el: v) {
        s = s + to_string(el);
        if(&el!=&v.back()) {
            s = s + ", ";
        }
    }
    s = s + "]";
    return s;
}

vector<uint32_t> str_to_uint32t_vec(const string str) {
    string error_msg = "\nFormat of the query must start with a quotation \" followed by an opening bracket [ and get closed by a closing bracket ] and another quotation \" and there must be positive integers separated by commas within these brackets. E.g. \"[1, 2, 4, 20]\" is a valid query.\n";
	int str_length = str.length();
    if(!(str_length>0)) {
        throw invalid_argument(error_msg);
    }
    if(!(str[0]=='[' && str[str_length-1]==']')) {
        throw invalid_argument(error_msg);
    }

	vector<uint32_t> arr;

	int i;

	// Traverse the string
	for (i = 0; i<str_length; i++) {
		if (str[i] == ',' || str[i] == '[' || str[i] == ']' || str[i]==' ')
			continue;
		else {
            string number = "";
            while(str[i]!=',' && str[i]!=']') {
                if(str[i]==' ') {
                  i++;
                  continue;
                }
                if(!(str[i] >= '0' && str[i] <= '9')) {
                    throw invalid_argument(error_msg);
                }
				number = number + str[i];
                i++;
            }
            arr.push_back(stoi(number));
		}
	}

	return arr;	
}

double average_from_double_vec(const vector<double>* v=nullptr) {
    if(v==nullptr){
        return 0;
    }

    if(v->empty()){
        return 0;
    }

    auto const count = static_cast<double>(v->size());
    return reduce(v->begin(), v->end()) / count;
}

uint32_t max_from_uint32t_vec(const vector<uint32_t>* v=nullptr) {
    if(v==nullptr){
        return 0;
    }

    if(v->empty()){
        return 0;
    }
    return *max_element(v->begin(), v->end());
}

uint32_t min_from_uint32t_vec(const vector<uint32_t>* v=nullptr) {
    if(v==nullptr){
        return 0;
    }
    
    if(v->empty()){
        return 0;
    }
    return *min_element(v->begin(), v->end());
}