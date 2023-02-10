#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <assert.h> 
#include <stdlib.h>
#include <limits>
#ifdef _OPENMP
   #include <omp.h>
#else
   #define omp_get_thread_num() 0
#endif

using namespace std;

vector<uint32_t> max_vertices;
vector<uint32_t> max_edges;

vector<uint32_t> min_vertices;
vector<uint32_t> min_edges;

vector<double> average_vertices;
vector<double> average_edges;

#include "TwoStageParallel.hh"
#include "NewDataGraph.hh"
#include "VecOps.hh"
#include "KWSGraph.hh"

#include "utils.hh"
using namespace NewGraph;

#define PRINT_ALL_LABEL_RANGES_FOR_ORKUT true //decides whether to print how many occurances of labels from 0-9,10-19,...90-99 there are for the orkut graph

#define PRINT_DEGREES_OF_ALL_NODES false

#define MAX_ITERATIONS 50

enum Type {POS_INT, POS_DOUBLE}; 

void print_results_for_unlabelled_graph_with_populated_labels(DataGraph &G) {
    if(G.get_label_range().first==0 && G.get_label_range().second==99) {
        vector<uint32_t> label_list;
        for(uint32_t i = 0; i<G.get_vertex_count(); i++) {
            for(auto l: G.label(i+1)) {
                label_list.push_back(l);
            }
        }
        for(uint32_t i = 0; i<100; i+=10) {
            uint32_t counts = 0;
            for(uint32_t j=0;j<10;j++) {
                uint32_t num_to_count = i+j;
                counts = counts + count(label_list.begin(),label_list.end(),num_to_count);
            }
            utils::Log{} << "number of labels from " << i << " to " << i+9 << " : " << counts << "\n"; 
        }
    }
}

void print_degree_counts_of_nodes(DataGraph &G) { // counts number of vertices with degrees by 0, 1-9, 10-19, 20-29, .... 90-99, 100+
    utils::Log{} << "------------\n";
    vector<uint32_t> counts(12, 0);
    for(uint32_t i=1; i<=G.get_vertex_count();i++) {
        uint32_t degree = G.get_adj(i).length;
        uint32_t index;
        if(degree==0) {
            index = 0;
        }
        else if(degree>=100) {
            index = 11;
        }
        else {
            index = (degree)/10 + 1;
        }
        counts[index] = counts[index] + 1;
    }

    for(uint32_t i=0;i<counts.size();i++) {
        uint32_t range_min = 0;
        if(i>0) {
            range_min = (i-1) * 10;
            if(i==1) {
                range_min = 1;
            }
        }
        uint32_t range_max = 0;
        if(i>0) {
            range_max = (i-1) * 10 + 9;
        }
        if(i<counts.size()-1) {
            utils::Log{} << "vertices of degree from range " << range_min << " to " << range_max  << " occured " << counts[i] << " times.\n";
        }
        else {
            utils::Log{} << "vertices of degree " << range_min << "+ occured " << counts[i] << " times.\n";
        }
    }
}

void perf_test(DataGraph &G, const string graph_path, uint32_t knum, const double alpha, const uint32_t heap_size, const uint32_t max_depth, const uint32_t num_threads, const bool vertex_induced) {
    uint32_t iterations = MAX_ITERATIONS;

    vector<double> initalization_time;
    vector<double> enqueue_frontier_time;
    vector<double> identify_cn_time;
    vector<double> expansion_time;
    vector<double> tdp_time;
    vector<double> total_time;

    initalization_time.reserve(iterations);
    enqueue_frontier_time.reserve(iterations);
    identify_cn_time.reserve(iterations);
    expansion_time.reserve(iterations);
    tdp_time.reserve(iterations);
    total_time.reserve(iterations);

    #if !defined(TESTING)
        vector<uint32_t> max_vertices_heap;
        vector<uint32_t> max_edges_heap;
        vector<uint32_t> min_vertices_heap;
        vector<uint32_t> min_edges_heap;
        vector<double> average_vertices_heap;
        vector<double> average_edges_heap;

        max_vertices.reserve(iterations);
        max_edges.reserve(iterations);

        min_vertices.reserve(iterations);
        min_edges.reserve(iterations);

        average_vertices.reserve(iterations);
        average_edges.reserve(iterations);

        max_vertices_heap.reserve(iterations);
        max_edges_heap.reserve(iterations);

        min_vertices_heap.reserve(iterations);
        min_edges_heap.reserve(iterations);

        average_vertices_heap.reserve(iterations);
        average_edges_heap.reserve(iterations);
    #endif
    
    vector<double>* initalization_time_addr=&initalization_time;
    vector<double>* enqueue_frontier_time_addr=&enqueue_frontier_time;
    vector<double>* identify_cn_time_addr=&identify_cn_time;
    vector<double>* expansion_time_addr=&expansion_time;
    vector<double>* tdp_time_addr=&tdp_time;
    vector<double>* total_time_addr=&total_time;

    vector<uint32_t> label_list;
    for(uint32_t i = 0; i<G.get_vertex_count(); i++) {
        for(auto l: G.label(i+1)) {
            label_list.push_back(l);
        }
    }

    srand (1);
    uint32_t counter = 0;
    omp_set_num_threads(num_threads);

    if(knum>label_list.size()) {
        cout << "WARNING: Knum set was greater than the number of labels in the graph. Knum has been set to the max number of labels in the graph to prevent queries with duplicate keywords.\n";
        knum = label_list.size();
        cout << "Press Enter to Continue: ";
        cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
    }
    while(counter<iterations) {
        vector<uint32_t> query;

        //creating queries of size knum
        for(uint32_t i=0;i<knum;i++) {
            uint32_t rand_num = label_list[rand() % label_list.size()];
            while(find(query.begin(), query.end(), rand_num) != query.end()) { //if rand_num is already in query, pick a new rand_num
                rand_num = label_list[rand() % label_list.size()];
            }
            query.push_back(rand_num);
        }

        auto t1 = utils::get_timestamp();
        vector<KWSGraph> my_heap = get_heap(G,query,alpha,heap_size,max_depth,vertex_induced,graph_path,
        initalization_time_addr,enqueue_frontier_time_addr,identify_cn_time_addr,expansion_time_addr,tdp_time_addr);
        auto t2 = utils::get_timestamp();

        total_time_addr->push_back((t2-t1)/1e6);

        #if !defined(TESTING)
            for(auto graph: my_heap) {
                uint32_t num_vertices = graph.get_graph().num_vertices();
                uint32_t num_edges = graph.get_graph().num_true_edges();
                max_vertices_heap.push_back(num_vertices);
                max_edges_heap.push_back(num_edges);

                min_vertices_heap.push_back(num_vertices);
                min_edges_heap.push_back(num_edges);

                average_vertices_heap.push_back(num_vertices);
                average_edges_heap.push_back(num_edges);
            }
            if(my_heap.empty()) {
                uint32_t num_vertices = 0;
                uint32_t num_edges = 0;

                max_vertices_heap.push_back(num_vertices);
                max_edges_heap.push_back(num_edges);

                min_vertices_heap.push_back(num_vertices);
                min_edges_heap.push_back(num_edges);

                average_vertices_heap.push_back(num_vertices);
                average_edges_heap.push_back(num_edges);
            }
        #endif

        counter++;
    } 

    cout << "Top-k heap size: " << heap_size << "\n";
    cout << "Number of keywords in a query: " << knum << "\n";
    cout << "Alpha: " << alpha << "\n";
    cout << "#THREADS: " << omp_get_max_threads() << "\n";
    cout << "Max depth: " << max_depth << "\n";
    cout << "Average initalization time for all runs " << average_from_double_vec(initalization_time_addr) << endl;
    cout << "Average enqueue time for all runs " << average_from_double_vec(enqueue_frontier_time_addr) << endl;
    cout << "Average identify central node time for all runs " << average_from_double_vec(identify_cn_time_addr) << endl;
    cout << "Average expansion time for all runs " << average_from_double_vec(expansion_time_addr) << endl;
    cout << "Average top down processing time for all runs " << average_from_double_vec(tdp_time_addr) << endl;
    cout << "Average total time for all runs " << average_from_double_vec(total_time_addr) << endl;
    
    #if !defined(TESTING)
        cout << endl;
        vector<uint32_t>* max_vertices_addr=&max_vertices;
        vector<uint32_t>* max_edges_addr=&max_edges;

        vector<uint32_t>* min_vertices_addr=&min_vertices;
        vector<uint32_t>* min_edges_addr=&min_edges;

        vector<double>* average_vertices_addr=&average_vertices;
        vector<double>* average_edges_addr=&average_edges;

        vector<uint32_t>* max_vertices_heap_addr=&max_vertices_heap;
        vector<uint32_t>* max_edges_heap_addr=&max_edges_heap;

        vector<uint32_t>* min_vertices_heap_addr=&min_vertices_heap;
        vector<uint32_t>* min_edges_heap_addr=&min_edges_heap;

        vector<double>* average_vertices_heap_addr=&average_vertices_heap;
        vector<double>* average_edges_heap_addr=&average_edges_heap;

        cout << "Largest number of vertices after top down processing " << max_from_uint32t_vec(max_vertices_addr) << "\n";
        cout << "Largest number of edges after top down processing " << max_from_uint32t_vec(max_edges_addr) << "\n";
        cout << "Smallest number of vertices after top down processing " << min_from_uint32t_vec(min_vertices_addr) << "\n";
        cout << "Smallest number of edges after top down processing " << min_from_uint32t_vec(min_edges_addr) << "\n";
        cout << "Average number of vertices after top down processing " << average_from_double_vec(average_vertices_addr) << "\n";
        cout << "Average number of edges after top down processing " << average_from_double_vec(average_edges_addr) << "\n";
        cout << endl;
        cout << "Largest number of vertices in the heap for all runs " << max_from_uint32t_vec(max_vertices_heap_addr) << "\n";
        cout << "Largest number of edges in the heap for all runs " << max_from_uint32t_vec(max_edges_heap_addr) << "\n";
        cout << "Smallest number of vertices in the heap for all runs " << min_from_uint32t_vec(min_vertices_heap_addr) << "\n";
        cout << "Smallest number of edges in the heap for all runs " << min_from_uint32t_vec(min_edges_heap_addr) << "\n";
        cout << "Average number of vertices in the heap for all runs " << average_from_double_vec(average_vertices_heap_addr) << "\n";
        cout << "Average number of edges in the heap for all runs " << average_from_double_vec(average_edges_heap_addr) << "\n";
    #endif

}

//Pass enum to function: https://stackoverflow.com/questions/4654655/how-to-pass-enum-as-function-argument-in-c
//Function that accepts multiple types: https://stackoverflow.com/questions/8627625/is-it-possible-to-make-function-that-will-accept-multiple-data-types-for-given-a
template <typename T> 
T char_to_other_type(char* value, enum Type param_type, string& failure_message, string& exception_message, optional<double> less_or_equal_exception=nullopt, optional<double> more_or_equal_exception=nullopt) {
    T value_to_evaluate;
    bool exception_condition; //condition that checks if value is an invalid input (e.g. getting a double value when expected to be a string), set to true if input is invalid
    string unsupported_type_msg = "Unsupported type in char_to_other_type.\n";
    try {
        if(param_type==POS_DOUBLE) {
            value_to_evaluate = stod(value);
        }
        else if(param_type==POS_INT) {
            value_to_evaluate = stoi(value);
        }
        else {
            throw invalid_argument(unsupported_type_msg);
        }
    }
    catch(const invalid_argument& ia) {
        throw invalid_argument(failure_message);
    }

    if(param_type==POS_INT) {
        exception_condition = value_to_evaluate<0 || string(value).find(".")!=string::npos || string(value).find("-")!=string::npos;
    }
    else if(param_type==POS_DOUBLE) {
        exception_condition = string(value).find("-")!=string::npos;
    }
    else {
        throw invalid_argument(unsupported_type_msg);
    }

    if(less_or_equal_exception && more_or_equal_exception) { //check if value falls out of bounds in a certain range defined by the programmer
        exception_condition = exception_condition || (value_to_evaluate<=less_or_equal_exception || value_to_evaluate>=more_or_equal_exception);
    }

    if(exception_condition) {
        throw invalid_argument(exception_message);
    }
    return value_to_evaluate;
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        // Source for multi line string: https://stackoverflow.com/questions/1135841/c-multiline-string-literal
        cout << R""""(
        Need to run: 'bin/kws <graph path> \"[query]\" (<alpha>) (<heap size>) (<depth>) (<num threads>)' E.g. bin/kws data/orkut "[19,29,39]" 0.2 20 5 4. 
        However, setting alpha, heap size, depth, and number of threads is optional. 
        By default, alpha is set to 0.1, heap size is 20, depth is 10, and number of threads is the max number of threads your system can support.
        )"""";

        cout << "If you are doing performance evaluation, then run 'bin/kws <graph path>' instead.\n\n";
        return -1;
    }

    string graph_path = argv[1];
    DataGraph G(graph_path);
    double alpha=0.1;
    int heap_size=20;
    int max_depth=10;
    int num_threads = omp_get_max_threads();
    bool vertex_induced;

    #if defined(ALPHA)
        alpha = ALPHA;
    #endif

    #if defined(HEAP_SIZE)
        heap_size = HEAP_SIZE;
    #endif

    #ifdef THREADS
        num_threads = THREADS;
    #endif

    #ifdef MAX_DEPTH
        max_depth = MAX_DEPTH;
    #endif

    #if defined(EDGE_INDUCED)
        vertex_induced = false;
        utils::Log{} << "Edge induced solution\n";
    #else
        vertex_induced = true;
        utils::Log{} << "Vertex induced solution\n";
    #endif

    if(argc==2) { //performance evaluation only
        uint32_t knum = 6;

        #if defined(KNUM)
            knum=KNUM;
        #endif

        perf_test(G,graph_path,knum,alpha,heap_size,max_depth,num_threads,vertex_induced);
        return 0;
    }
    //running the program the standard way: 

    string q = argv[2];
    vector<uint32_t> query = str_to_uint32t_vec(q);
    sort( query.begin(), query.end() );
    query.erase( unique( query.begin(), query.end() ), query.end() ); //Sort and remove duplicate labels
    utils::Log{} << "Keywords from query detected: " << uint32t_vec_to_str(query) << "\n";

    if(argc > 3) {
        string err_msg_alpha = "The alpha value specified must be a double.\n";
        string exception_msg_alpha = "The alpha size specified must be a positive double between (0,1), not including 0 or 1.\n";
        alpha = char_to_other_type<double>(argv[3], POS_DOUBLE, err_msg_alpha , exception_msg_alpha, 0, 1);
    }
    utils::Log{} << "alpha: " << alpha << "\n";
    
    if(argc > 4) {
        string err_msg_heap = "The heap size specified must be an integer.\n";
        string exception_msg_heap = "The heap size specified must be a positive integer.\n";
        heap_size = char_to_other_type<int>(argv[4], POS_INT, err_msg_heap , exception_msg_heap);
    }
    utils::Log{} << "heap_size: " << heap_size << "\n";

    if(argc > 5) {
        string err_msg_depth = "The max depth specified must be an integer.\n";
        string exception_msg_depth = "The max depth specified must be a positive integer.\n";
        max_depth = char_to_other_type<int>(argv[5], POS_INT, err_msg_depth , exception_msg_depth);
    }
    utils::Log{} << "max_depth: " << max_depth << "\n";

    if(argc > 6) {
        string err_msg_td = "The number of threads must be an integer.\n";
        string exception_msg_td = "The number of threads must be a positive integer.\n";
        num_threads = char_to_other_type<int>(argv[6], POS_INT, err_msg_td , exception_msg_td);
    }
    omp_set_num_threads(num_threads);
    utils::Log{} << "num_threads: " << omp_get_max_threads() << "\n";

    // print orkut results
    if(graph_path.find("data/orkut")!=string::npos && PRINT_ALL_LABEL_RANGES_FOR_ORKUT) {
        print_results_for_unlabelled_graph_with_populated_labels(G);
    }
    if(PRINT_DEGREES_OF_ALL_NODES) {
        print_degree_counts_of_nodes(G);
    }
    
    utils::Log{} << "Label range: " << G.get_label_range().first << " " << G.get_label_range().second << "\n";
    utils::Log{} << "Num vertices: " << G.get_vertex_count() << ", Num edges: " << G.get_edge_count() << "\n";

    bool print_edges_bool = false;
    #if !defined(TESTING)
        string print_edges_str;
        while(print_edges_str!="y" && print_edges_str!="n" && print_edges_str!="Y" && print_edges_str!="N") {
            cout << "Do you want to print the edges of the graphs in the heap? (type 'y' for yes or 'n' for no, not including the single quotations) ";
            cin >> print_edges_str;
        }
        if(print_edges_str=="y" || print_edges_str=="Y") {
            print_edges_bool = true;
        }
        else if(print_edges_str=="n" || print_edges_str=="N") {
            print_edges_bool = false;
        }
        else {
            cout << "Something went wrong with entering the prompt. Try again.\n";
            return -1;
        }
    #endif

    auto t1 = utils::get_timestamp();
    vector<KWSGraph> my_heap = get_heap(G,query,alpha,heap_size,max_depth,vertex_induced,graph_path);
    auto t2 = utils::get_timestamp();

    #if !defined(TESTING)
        utils::Log{} << "Heap size of " << my_heap.size() << "\n";
        print_heap(my_heap, print_edges_bool);
    #endif
     
    utils::Log{} << "---------\n";
    utils::Log{} << "Full program finished after " << (t2-t1)/1e6 << "s" << "\n";

    utils::Log{} << "\n";

    assert(heap_score_is_desc(my_heap)==true);

    if(argc > 7) {
        string rc_flag = argv[7];
        if(rc_flag=="-rc") { //check for race conditions
            cout << "Comparing your original heap with a sequential copy of your heap of a size of (your defined size + 10)\n";
            omp_set_num_threads(1);
            vector<KWSGraph> my_heap_extended = get_heap(G,query,alpha,heap_size+10,max_depth,vertex_induced,graph_path);
            for(uint32_t i=0;i<my_heap.size();i++) {
                if(my_heap[i]!=my_heap_extended[i]) {
                    cout << "ERROR: The top-k heap result is incorrect. Your results are not top-k. \n";
                    return -1;
                }
            }
            cout << "End comparison\n";
            omp_set_num_threads(num_threads);
            vector<KWSGraph> my_new_heap = get_heap(G,query,alpha,heap_size,max_depth,vertex_induced,graph_path);
            while(heap_score_is_desc(my_new_heap)==true) {
                auto t1 = utils::get_timestamp();
                my_new_heap = get_heap(G,query,alpha,heap_size,max_depth,vertex_induced,graph_path);
                auto t2 = utils::get_timestamp();

                cout << "Heap size of " << my_new_heap.size() << "\n";
                print_heap(my_new_heap, print_edges_bool);

                cout << "---------\n";
                cout << "Full program finished after " << (t2-t1)/1e6 << "s" << "\n";
                for(uint32_t i=0;i<my_new_heap.size();i++) {
                    if(my_heap_extended[i]!=my_new_heap[i]) {
                        cout << "ERROR: Graphs in the heap are not equal after multiple runs of the algorithm.\n";
                        return -1;
                    }
                }
            }
            cout << "ERROR: Heap is not correctly sorted by [Score, Central Node] is descending order.\n";
            return -1;
        }
        else {
            cout << "Flag specified is invalid.\n";
            return -1;
        }
    }

    return 0;
}