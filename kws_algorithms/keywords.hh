#ifndef KW_HH
#define KW_HH

#include <vector>
#include <iostream>
#include <algorithm> 

using namespace std;

class Keywords {
    private:
        vector<uint32_t> keywords;
        uint32_t length;

    public:
        Keywords(const vector<uint32_t> query) {
            keywords = query;
            length = query.size();
        }

        vector<uint32_t> get_keywords() const {
            return keywords;
        }

        uint32_t get_length() const {
            return length;
        }

        
        bool contains_keyword(vector<uint32_t>& labels) const {
            for(uint32_t label: labels) {
                if(find(keywords.begin(), keywords.end(), label) != keywords.end()) {
                    return true;
                }
            }
            return false;
        }

        bool contains_keyword(uint32_t label) const {
            if(find(keywords.begin(), keywords.end(), label) != keywords.end()) {
                return true;
            }
            return false;
        }

        void print_keywords() const {
            for(uint32_t i=0; i<length;i++) {
                cout << keywords[i] << " ";
            }
            cout <<"\n";
        }
};

#endif
