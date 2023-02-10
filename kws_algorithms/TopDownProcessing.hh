#ifndef TDP_H
#define TDP_H

#include <queue>
#include <algorithm> 
#include <limits>
#include <assert.h>

#ifdef _OPENMP
   #include <omp.h>
#else
   #define omp_get_thread_num() 0
#endif

#include "LevelCoverStrategy.hh"

#include "NewDataGraph.hh"
#include "NewGraph.hh"
#include "keywords.hh"

extern vector<uint32_t> max_vertices;
extern vector<uint32_t> max_edges;

extern vector<uint32_t> min_vertices;
extern vector<uint32_t> min_edges;

extern vector<double> average_vertices;
extern vector<double> average_edges; 

using namespace std;
using namespace NewGraph;

#define ADD_EMPTY_LABEL false

bool edge_exists(const SmallGraph& g, const uint32_t u, const uint32_t v) {
    vector<uint32_t> g_v_list = g.v_list();
    if(!(find(g_v_list.begin(), g_v_list.end(), u) != g_v_list.end() && find(g_v_list.begin(), g_v_list.end(), v) != g_v_list.end())) { //neither u or v are in g
        return false;
    }
    else if(find(g_v_list.begin(), g_v_list.end(), u) != g_v_list.end()) { //u is in g
        vector<uint32_t> u_neighbours = g.get_neighbours(u);
        if(!(find(u_neighbours.begin(), u_neighbours.end(), v) != u_neighbours.end())) { 
            return false;
        }
    }
    else if(find(g_v_list.begin(), g_v_list.end(), v) != g_v_list.end()) { //v is in g
        vector<uint32_t> v_neighbours = g.get_neighbours(v);
        if(!(find(v_neighbours.begin(), v_neighbours.end(), u) != v_neighbours.end())) {
            return false;
        }
    }
    return true;
}

//https://www.geeksforgeeks.org/c-program-find-largest-element-array/
uint32_t largest(uint32_t arr[], uint32_t size)
{
    uint32_t i;
    uint32_t max = arr[0];
    for (i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
 
    return max;
}

bool is_in_queue(const queue<uint32_t>& q, const uint32_t val) {
  bool result = false;
  queue<uint32_t> cpy_q = q;
  while(!cpy_q.empty()) {
    uint32_t f = cpy_q.front();
    if(f==val) {
      result = true;
      break;
    }
    cpy_q.pop();
  }
  return result;
}

// if the vertex has one of the top-k scores, insert it into the heap
// the heap is sorted by score and central node of the graph in descending order
// top_k_scores is an array that keeps track of the top-k scores in the heap
void insert_top_k_into_heap(double* top_k_scores, const double* weights, vector<KWSGraph>&heap, const uint32_t& heap_size, const SmallGraph& pruned_graph, const uint32_t& max_depth, const uint32_t& root) {
  vector<uint32_t> pg_v_list = pruned_graph.v_list();
  int index_to_insert_into_heap = -1;
  double score = compute_score(pruned_graph,weights,max_depth);
  if(score<top_k_scores[heap_size-1]) { //if score is not top-k then skip
    return;
  }
  #pragma omp critical
  {
    for(uint32_t i=0;i<heap_size;i++) {
      // score_is_greatest: checks if the graph's score is larger than any value in the heap
      bool score_is_greatest = false; 

      // score_is_between_greatest_and_last_top_k_or_equal_to_last_top_k:
      // checks if the graph's score is between (but not including) the largest score in the heap and last top-k score in the heap 
      // OR is equal to the last top-k score
      bool score_is_between_greatest_and_last_top_k_or_equal_to_last_top_k = false; 

      // score_is_equal_with_diff_root: checks if the graph's score is equal to one of the top k scores 
      // (assumes that the input graph will have a different central node/root to the one in the heap)
      bool score_is_equal_with_diff_root = false;

      if(i==0) { 
        if((score>top_k_scores[i]) && (score>top_k_scores[heap_size-1]))
        {
          // checks if graph's score is larger than every element in the heap
          // if true, insert the graph at the top of the heap
          score_is_greatest = true; 
        }

        if((score==top_k_scores[i]) && (score>=top_k_scores[heap_size-1]) && (root>heap[i].get_root()))
        {
          // checks if graph's score is equal to the first top-k score in the heap and that it has a central node with a higher number id than
          // the current graph at the first top-k index
          // if true, insert the graph at the top of the heap
          score_is_equal_with_diff_root = true; 
        }
      }
      else if(i>0) {
        if((score<top_k_scores[i-1]) && (score>top_k_scores[i]))
        {
          // checks if the graph's score is less than the score at the previous index heap[i-1], but is equal to the current index at heap[i]
          // if true, insert the graph between heap[i-1] and heap[i]
          score_is_between_greatest_and_last_top_k_or_equal_to_last_top_k = true;
        }
        
        if((score<top_k_scores[i-1]) && (score==top_k_scores[i]) && (root>heap[i].get_root())){
          // checks if the graph's score is less than the score at the previous index heap[i-1], but is equal to the current index at heap[i] and has a
          // central node with a higher number id than the graph currently stored at heap[i]
          // if true, insert the graph between heap[i-1] and heap[i]
          score_is_equal_with_diff_root = true;
        }
        else if((score==top_k_scores[i-1]) && (score==top_k_scores[i]) && (root<heap[i-1].get_root()) && (root>heap[i].get_root())){
          // checks if the graph's score is equal to the score at the previous index heap[i-1] and equal to the current index at heap[i], but has a central node 
          // that is a lower number id than the previous index at heap[i-1] and greater than the current index at heap[i]
          // if true, insert the graph between heap[i-1] and heap[i]
          score_is_equal_with_diff_root = true;
        }
        else if((score==top_k_scores[i-1]) && (score>top_k_scores[i]) && (root<heap[i-1].get_root())){
          // checks if the graph's score is equal to the score at the previous index heap[i-1], but is greater than the score at the current index heap[i]
          // and if the graph's central node also has a lower number id than the central node of the graph at previous index heap[i-1] despite having the same score
          // if true, insert the graph between heap[i-1] and heap[i]
          score_is_equal_with_diff_root = true;
        }
      }

      // if any of the boolean conditions are satisified at index i of the heap, shift all current elements from i to n to the right
      // and insert the graph into the heap at index i
      if(score_is_greatest || score_is_between_greatest_and_last_top_k_or_equal_to_last_top_k || score_is_equal_with_diff_root) {
          KWSGraph k_pruned_graph(pruned_graph,score,max_depth,root);
          index_to_insert_into_heap = i;
          KWSGraph KWSGraph_copy = heap[index_to_insert_into_heap];
          double copy = top_k_scores[index_to_insert_into_heap];
          for(uint32_t j=i+1;j<=heap_size-1;j++) {
            double copy2 = top_k_scores[j];
            top_k_scores[j] = copy;
            copy = copy2;

            KWSGraph KWSGraph_copy2 = heap[j];
            heap[j] = KWSGraph_copy;
            KWSGraph_copy = KWSGraph_copy2; 
          }
          top_k_scores[index_to_insert_into_heap] = score;
          heap[index_to_insert_into_heap] = k_pruned_graph;
          break;
      }
    }
  }
}


vector<KWSGraph> top_down_processing(DataGraph& G, uint32_t**M, const uint32_t* CIdentifier, const double* weights, const uint32_t* activation,  const Keywords& keywords, const uint32_t heap_size, const uint32_t vertex_length, const uint32_t query_length, const bool vertex_induced=true) {
  vector<KWSGraph> heap;
  
  double* top_k_scores = new double[heap_size]; //array that keeps track of the top-k scores
  double* empty_arr = {0};
  for(uint32_t i=0;i<heap_size;i++) {
    SmallGraph g;
    KWSGraph kg(g,empty_arr,0,0);
    heap.push_back(kg);
    top_k_scores[i] = -1;
  }


  #if !defined(TESTING)
    uint32_t num_of_central_nodes = 0;
    for(uint32_t i=0;i<vertex_length;i++) {
      if(CIdentifier[i]==1) {
        num_of_central_nodes++;
      }
    }
    utils::Log{} << "Number of central nodes " << num_of_central_nodes << "\n";
  #endif

  #if !defined(TESTING)
    #if defined(_OPENMP) && !defined(THREADS)
      uint32_t num_threads = omp_get_max_threads();
    #elif defined(_OPENMP) && defined(THREADS)
      uint32_t num_threads = THREADS;
    #else
      uint32_t num_threads = 1;
    #endif
  #endif

  #if !defined(TESTING)
    double* tdp_algorithm_time = new double[num_threads];
    double* preserve_edges_time = new double[num_threads];
    double* lcs_time = new double[num_threads];
    double* heap_insert_time = new double[num_threads];
    double* preserve_edge_insert_time = new double[num_threads]; //time for inserting original edges of graph before pruning into pruned graph


    double* lcs_get_min_kw = new double[num_threads];
    double* lcs_lcs = new double[num_threads];
    double* lcs_shortest_path = new double[num_threads];
    double* lcs_preserve_original_edges = new double[num_threads];

    double avg_vertices_after_tdp = 0.0;
    double avg_edges_after_tdp = 0.0;
    uint32_t largest_vertices_after_tdp = 0;
    uint32_t largest_edges_after_tdp = 0;
    uint32_t smallest_vertices_after_tdp = numeric_limits<int>::max();
    uint32_t smallest_edges_after_tdp = numeric_limits<int>::max();

    uint32_t largest_vertices_after_lcs = 0;
    uint32_t smallest_vertices_after_lcs = numeric_limits<int>::max();
    double avg_vertices_after_lcs = 0.0;
    uint32_t largest_edges_after_lcs = 0;
    uint32_t smallest_edges_after_lcs = numeric_limits<int>::max();
    double avg_edges_after_lcs = 0.0;

    #pragma omp parallel for
    for(uint32_t i=0;i<num_threads;i++) {
      tdp_algorithm_time[i] = 0.0;
      preserve_edges_time[i] = 0.0;
      preserve_edge_insert_time[i] = 0.0;
      lcs_time[i] = 0.0;
      heap_insert_time[i] = 0.0;

      lcs_get_min_kw[i] = 0.0;
      lcs_lcs[i] = 0.0;
      lcs_shortest_path[i] = 0.0;
      lcs_preserve_original_edges[i] = 0.0;
    }
  #endif

  #pragma omp parallel for schedule(dynamic)
  for(uint32_t index=0; index<vertex_length; index++) {
    uint32_t i = index;
    uint32_t root = i+1;
    
    if(CIdentifier[i]==1) {
      #if !defined(TESTING)
        auto t1 = utils::get_timestamp();
      #endif
      uint32_t max_depth = 0;
      for(uint32_t heap_size=0; heap_size<query_length; heap_size++) {
        if(M[i][heap_size]>max_depth) {
          max_depth = M[i][heap_size];
        }
      }

      SmallGraph g;

      vector<queue<uint32_t>> queues;
      for(uint32_t j=0; j<query_length; j++){
        queue<uint32_t> q;
        q.push(i);
        queues.push_back(q);
      }
      
      bool will_extend = false;

      for(uint32_t b=0; b<query_length; b++) {
        vector<uint32_t> explored;
        while(!queues[b].empty()) {
          queue<uint32_t> qp;
          uint32_t vf = queues[b].front() + 1;
          queues[b].pop();
          explored.push_back(vf);

          for(uint32_t n=0;n<G.get_adj(vf).length;n++) {
            uint32_t vn = *(G.get_adj(vf).ptr + n);
            if(find(explored.begin(), explored.end(), vn) != explored.end()) {
              continue;
            }
            uint32_t hit_neighbour = M[vn-1][b];
            if(hit_neighbour == numeric_limits<uint32_t>::max()) {
              hit_neighbour = hit_neighbour - 1; //prevent overflowing from infinity+1
            }
            uint32_t arr[3] = {activation[vn-1],hit_neighbour,activation[vf-1]-1};

            vector<uint32_t> vf_labels = G.label(vf);
            if(
            ((keywords.contains_keyword(vf_labels) && (M[vf-1][b] == 1 + max(activation[vn-1],hit_neighbour))) 
              ||
            (!keywords.contains_keyword(vf_labels) && (M[vf-1][b] == 1 + largest(arr,3))) ) 
              ) {

              bool edge_is_already_added;
              edge_is_already_added = edge_exists(g,vf,vn);
              if(!edge_is_already_added) {
                vector<uint32_t> f_label = G.label(vf);
                vector<uint32_t> n_label = G.label(vn);
                g.add_edge(vf, vn, ADD_EMPTY_LABEL).set_label(vf,f_label).set_label(vn,n_label);
                will_extend = true;
              }

              if(!is_in_queue(qp,vn-1)) {
                qp.push(vn-1);
              }
        
            }
          }
          queues[b] = qp;
        }
      }
      
      if(!will_extend) {
        g.add_node(root,ADD_EMPTY_LABEL).set_label(root,G.label(root));
      }

      #if !defined(TESTING)
        auto t2 = utils::get_timestamp();
        tdp_algorithm_time[omp_get_thread_num()] = tdp_algorithm_time[omp_get_thread_num()] + (t2-t1);

        uint32_t num_vertices_before_preserving_original_edges = g.num_vertices();
      #endif

      
      if(vertex_induced)
      {
        #if !defined(TESTING)
          auto t11 = utils::get_timestamp();
          preserve_original_edges_from_DataGraph(G,g,preserve_edge_insert_time);
          auto t22 = utils::get_timestamp();
          preserve_edges_time[omp_get_thread_num()] = preserve_edges_time[omp_get_thread_num()] + (t22-t11);
          uint32_t num_of_vertices_after_preserving_original_edges = g.num_vertices();
          assert(num_vertices_before_preserving_original_edges == num_of_vertices_after_preserving_original_edges);
        #else
          preserve_original_edges_from_DataGraph(G,g);
        #endif
      }
      
      #if !defined(TESTING)
        #pragma omp critical
        {
          if(g.num_vertices()<smallest_vertices_after_tdp) {
            smallest_vertices_after_tdp = g.num_vertices();
          }
          if(g.num_true_edges()<smallest_edges_after_tdp) {
            smallest_edges_after_tdp = g.num_true_edges();
          }

          if(g.num_vertices()>largest_vertices_after_tdp) {
            largest_vertices_after_tdp = g.num_vertices();
          }
          if(g.num_true_edges()>largest_edges_after_tdp) {
            largest_edges_after_tdp = g.num_true_edges();
          }

          avg_vertices_after_tdp = avg_vertices_after_tdp + g.num_vertices();
          avg_edges_after_tdp = avg_edges_after_tdp + g.num_true_edges();
        }
      #endif
      
      #if !defined(TESTING)
        uint32_t num_of_vertices_before_lcs = g.num_vertices();
        assert(num_of_vertices_before_lcs == num_vertices_before_preserving_original_edges);
      #endif

      vector<uint32_t> kw = keywords.get_keywords();
      #if !defined(TESTING)
        t1 = utils::get_timestamp();
        SmallGraph pruned_graph = level_cover_strategy(g, root, kw, lcs_get_min_kw, lcs_lcs, lcs_shortest_path);
        t2 = utils::get_timestamp();
        lcs_time[omp_get_thread_num()] = lcs_time[omp_get_thread_num()] + (t2-t1);
      #else
        SmallGraph pruned_graph = level_cover_strategy(g, root, kw);
      #endif

      if(vertex_induced && pruned_graph.num_vertices() > 1)
      {
          #if !defined(TESTING)
            auto t12 = utils::get_timestamp();
            preserve_original_edges_from_SmallGraph(g,pruned_graph);
            auto t22 = utils::get_timestamp();
            lcs_preserve_original_edges[omp_get_thread_num()] = lcs_preserve_original_edges[omp_get_thread_num()] + (t22-t12);
          #else
            preserve_original_edges_from_SmallGraph(g,pruned_graph);
          #endif
      }

      #if !defined(TESTING)
        #pragma omp critical
        {
          average_vertices.push_back(pruned_graph.num_vertices()); //average the total number of vertices and edges after all runs if the program is run with multiple iterations to prevent rounding error
          average_edges.push_back(pruned_graph.num_true_edges());

          if(pruned_graph.num_vertices()<smallest_vertices_after_lcs) {
            smallest_vertices_after_lcs = pruned_graph.num_vertices();
          }
          if(pruned_graph.num_true_edges()<smallest_edges_after_lcs) {
            smallest_edges_after_lcs = pruned_graph.num_true_edges();
          }

          if(pruned_graph.num_vertices()>largest_vertices_after_lcs) {
            largest_vertices_after_lcs = pruned_graph.num_vertices();
          }
          if(pruned_graph.num_true_edges()>largest_edges_after_lcs) {
            largest_edges_after_lcs = pruned_graph.num_true_edges();
          }

          avg_vertices_after_lcs = avg_vertices_after_lcs + pruned_graph.num_vertices();
          avg_edges_after_lcs = avg_edges_after_lcs + pruned_graph.num_true_edges();
        }
      #endif

      #if !defined(TESTING)
        t1 = utils::get_timestamp();
        insert_top_k_into_heap(top_k_scores, weights, heap, heap_size, pruned_graph, max_depth, root);  //if graph is top-k, insert into heap
        t2 = utils::get_timestamp();
        heap_insert_time[omp_get_thread_num()] = heap_insert_time[omp_get_thread_num()] + (t2-t1);
      #else
        insert_top_k_into_heap(top_k_scores, weights, heap, heap_size, pruned_graph, max_depth, root);
      #endif
    }
  }

  vector<KWSGraph> true_heap;
  for(uint32_t i=0;i<heap_size;i++) {
    if(heap[i].get_num_vertices()==0) {
      break;
    }
    true_heap.push_back(heap[i]);
  }
  
  #if !defined(TESTING)
    double tdp_avg;
    double preserve_edges_avg;
    double lcs_avg;
    double heap_ins_avg;

    double preserve_edge_insert_avg;

    double lcs_get_min_kw_avg;
    double lcs_lcs_avg;
    double lcs_shortest_path_avg;
    double lcs_preserve_original_edges_avg;

    tdp_avg = 0.0;
    preserve_edges_avg = 0.0;
    lcs_avg = 0.0;
    heap_ins_avg = 0.0;

    preserve_edge_insert_avg = 0.0;

    lcs_get_min_kw_avg = 0.0;
    lcs_lcs_avg = 0.0;
    lcs_shortest_path_avg = 0.0;
    lcs_preserve_original_edges_avg = 0.0;
    
    utils::Log{} << "---------\n";
    for(uint32_t i=0; i<num_threads; i++) {
      // utils::Log{} << "thread " << i << " time for tdp (alg in paper): " << tdp_algorithm_time[i]/1e6 << "\n";
      // utils::Log{} << "thread " << i << " time for preserve original edges : " << preserve_edges_time[i]/1e6 << "\n";
      // utils::Log{} << "thread " << i << " time for preserve original edges insertion : " << preserve_edge_insert_time[i]/1e6 << "\n";
      // utils::Log{} << "thread " << i << " time for lcs : " << lcs_time[i]/1e6 << "\n";
      // utils::Log{} << "thread " << i << " time for heap insertion : " << heap_insert_time[i]/1e6 << "\n";
      // utils::Log{} << "---------\n";
      
      // utils::Log{} << "thread " << i << " time for get min kw (lcs): " << lcs_get_min_kw[i]/1e6 << "\n";
      // utils::Log{} << "thread " << i << " time for bfs (lcs) : " << lcs_lcs[i]/1e6 << "\n";
      // utils::Log{} << "thread " << i << " time for shortest path (lcs)  : " << lcs_shortest_path[i]/1e6 << "\n";
      // utils::Log{} << "thread " << i << " time for preserve original edges (lcs) : " << lcs_preserve_original_edges[i]/1e6 << "\n";
      // utils::Log{} << "---------\n";

      
      tdp_avg = tdp_avg + tdp_algorithm_time[i];
      preserve_edges_avg = preserve_edges_avg + preserve_edges_time[i];
      preserve_edge_insert_avg = preserve_edge_insert_avg + preserve_edge_insert_time[i];
      lcs_avg = lcs_avg + lcs_time[i];
      heap_ins_avg = heap_ins_avg + heap_insert_time[i];

      lcs_get_min_kw_avg = lcs_get_min_kw_avg + lcs_get_min_kw[i];
      lcs_lcs_avg = lcs_lcs_avg + lcs_lcs[i];
      lcs_shortest_path_avg = lcs_shortest_path_avg + lcs_shortest_path[i];
      lcs_preserve_original_edges_avg = lcs_preserve_original_edges_avg + lcs_preserve_original_edges[i];
      
    }

    tdp_avg = tdp_avg/(num_threads*1e6);
    preserve_edges_avg = preserve_edges_avg/(num_threads*1e6);
    lcs_avg = lcs_avg/(num_threads*1e6);
    heap_ins_avg = heap_ins_avg/(num_threads*1e6);

    preserve_edge_insert_avg = preserve_edge_insert_avg/(num_threads*1e6);

    lcs_get_min_kw_avg = lcs_get_min_kw_avg/(num_threads*1e6);
    lcs_lcs_avg = lcs_lcs_avg/(num_threads*1e6);
    lcs_shortest_path_avg = lcs_shortest_path_avg/(num_threads*1e6);
    lcs_preserve_original_edges_avg = lcs_preserve_original_edges_avg/(num_threads*1e6);


    utils::Log{} << "Top down processing steps overview (average of all " << num_threads << " " << "threads): \n";
    utils::Log{} << "avg of top down processing algogrithm from the paper " << tdp_avg << "\n";
    utils::Log{} << "avg of preserve original edges from DataGraph to SmallGraph " << preserve_edges_avg << "\n";
    utils::Log{} << "avg of pres edge time from Datagraph to SmallGraph (edge insertion) " << preserve_edge_insert_avg << "\n";
    utils::Log{} << "avg of lcs (Level Cover Strategy) before preserving edges " << lcs_avg << "\n";
    utils::Log{} << "avg of heap insertion " << heap_ins_avg << "\n";

    utils::Log{} << "---------\nLevel Cover Strategy details (average of all " << num_threads << " threads):\n";

    utils::Log{} << "avg of get min keywords (lcs) " << lcs_get_min_kw_avg << "\n";
    utils::Log{} << "avg of lcs algorithm (bfs traversal outer loop) " << lcs_lcs_avg - lcs_shortest_path_avg << "\n";
    utils::Log{} << "avg of lcs algorithm (get shortest path of added node) " << lcs_shortest_path_avg  << "\n";
    utils::Log{} << "avg of preserving original edges (lcs) " << lcs_preserve_original_edges_avg << "\n";
    

    if(num_of_central_nodes>0) {
      avg_vertices_after_tdp = avg_vertices_after_tdp/num_of_central_nodes;
      avg_edges_after_tdp = avg_edges_after_tdp/num_of_central_nodes;
      avg_vertices_after_lcs = avg_vertices_after_lcs/num_of_central_nodes;
      avg_edges_after_lcs = avg_edges_after_lcs/num_of_central_nodes;

      utils::Log{} << "---------\n";
      utils::Log{} << "largest num vertices after top down and preserving original edges if applicable " << largest_vertices_after_tdp << "\n";
      utils::Log{} << "largest num edges after top down and preserving original edges if applicable " << largest_edges_after_tdp << "\n";
      utils::Log{} << "---------\n";
      utils::Log{} << "avg num vertices after top down and preserving original edges if applicable " << avg_vertices_after_tdp << "\n";
      utils::Log{} << "avg num edges after top down and preserving original edges if applicable " << avg_edges_after_tdp << "\n";
      utils::Log{} << "---------\n";
      utils::Log{} << "smallest num vertices after top down and preserving original edges if applicable " << smallest_vertices_after_tdp << "\n";
      utils::Log{} << "smallest num edges after top down and preserving original edges if applicable " << smallest_edges_after_tdp << "\n";
      utils::Log{} << "---------\n";

      utils::Log{} << "largest num vertices after lcs " << largest_vertices_after_lcs << "\n";
      utils::Log{} << "largest num edges after lcs " << largest_edges_after_lcs << "\n";
      utils::Log{} << "---------\n";
      utils::Log{} << "avg num vertices after lcs " << avg_vertices_after_lcs << "\n";
      utils::Log{} << "avg num edges after lcs " << avg_edges_after_lcs << "\n";
      utils::Log{} << "---------\n";
      utils::Log{} << "smallest num vertices after lcs " << smallest_vertices_after_lcs << "\n";
      utils::Log{} << "smallest num edges after lcs " << smallest_edges_after_lcs << "\n";

      max_vertices.push_back(largest_vertices_after_lcs);
      max_edges.push_back(largest_edges_after_lcs);

      min_vertices.push_back(smallest_vertices_after_lcs);
      min_edges.push_back(smallest_edges_after_lcs);
    }
  #endif


  #if !defined(TESTING)
    delete [] tdp_algorithm_time;
    delete [] preserve_edges_time;
    delete [] lcs_time;
    delete [] heap_insert_time;
    delete [] preserve_edge_insert_time; //time for inserting original edges of graph before pruning into pruned graph


    delete [] lcs_get_min_kw;
    delete [] lcs_lcs;
    delete [] lcs_shortest_path;
    delete [] lcs_preserve_original_edges;
  #endif


  delete [] top_k_scores;
  return true_heap;
}

#endif