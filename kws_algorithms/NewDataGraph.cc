#include <iostream>
#include <memory>

#include <string>
#include <cstring>
#include <algorithm>
#include <unordered_set>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <filesystem>

#include "utils.hh"
#include "NewGraph.hh"
#include "NewDataGraph.hh"

#define TRUNCATED 0

#define RANDOMLY_GENERATE_LABELS_FOR_UNLABELLED_GRAPHS 1

#include <stdlib.h>
#include <time.h> 


namespace NewGraph
{
  void DataGraph::from_smallgraph(const SmallGraph &pp)
  {
    SmallGraph p(pp);
    graph_in_memory = std::make_unique<uint32_t[]>(2 * p.num_true_edges());
    data_graph = std::make_unique<adjlist[]>(p.num_vertices()+1);

    uint32_t cursor = 0;
    for (uint32_t v = 1; v <= p.num_vertices(); ++v) {
      std::sort(p.true_adj_list.at(v).begin(), p.true_adj_list.at(v).end());

      std::memcpy(&graph_in_memory[cursor], &p.true_adj_list.at(v)[0], p.true_adj_list.at(v).size() * sizeof(uint32_t));
      data_graph[v-1].ptr = &graph_in_memory[cursor];
      data_graph[v-1].length = p.true_adj_list.at(v).size();
      cursor += p.true_adj_list.at(v).size();
    }

    vertex_count = p.num_vertices();
    edge_count = p.num_true_edges();

    
    if (p.labelling == Graph::LABELLED)
    {
      std::vector<std::vector<uint32_t>> new_labels(p.num_vertices()+1);
      uint32_t min = UINT_MAX;
      uint32_t max = 0;
      labelled_graph = true;
      for (auto labels_in_sg : p.labels)
      {
        for(auto l : labels_in_sg.second) {
          if(l<min) {
            min = l;
          }
          if(l>max){
            max = l;
          }
          new_labels[labels_in_sg.first].push_back(l);
        }
      }
      label_range = std::make_pair(min, max);
      labels = new_labels;
    }


    ids = std::make_unique<uint32_t[]>(vertex_count+1);
    std::iota(&ids[0], &ids[vertex_count+1], 0);
  }

  DataGraph::DataGraph(const SmallGraph &p)
  {
    from_smallgraph(p);
  }

  DataGraph::DataGraph(const std::string &path_str)
  {
    std::filesystem::path data_graph_path(path_str);
    if (std::filesystem::is_directory(data_graph_path))
    {
      std::filesystem::path data_path(data_graph_path / "data.bin");
      if (!std::filesystem::exists(data_path))
      {
        std::cerr << "ERROR: Data graph could not be opened." << std::endl;
        exit(1);
      }

      std::ifstream input_graph(data_path, std::ios::binary);
      input_graph.read(reinterpret_cast<char *>(&vertex_count), sizeof(vertex_count));
      input_graph.read(reinterpret_cast<char *>(&edge_count), sizeof(edge_count));

      uint64_t file_size = std::filesystem::file_size(data_path);
      assert(file_size % 4 == 0);
      uint64_t data_count = file_size / 4;
      // don't count the header (num vertices - 32-bit integer, num edges - 64-bit integer)
      uint64_t body_count = data_count-3;
      graph_in_memory = std::make_unique<uint32_t[]>(body_count);

      uint64_t curr_read_offset = 0;
      uint64_t read_batch_size = 2147479552;
      while (curr_read_offset/4 <= body_count)
      {
        input_graph.read(reinterpret_cast<char *>(graph_in_memory.get() + curr_read_offset/4), std::min(read_batch_size, (body_count)*4-curr_read_offset));
        curr_read_offset += read_batch_size;
      }

      data_graph = std::make_unique<adjlist[]>(vertex_count);

#ifdef TRUNCATED
    // uint32_t maxVertex = 5'000;
    uint32_t maxVertex = MAX_VERTEX;
    // uint32_t maxVertex = 50'000;
    uint32_t cursor = 0;
    for (uint32_t i = 0; i < vertex_count; i++)
    {
      data_graph[i].length = graph_in_memory[cursor];
      data_graph[i].ptr = &graph_in_memory[++cursor];
      cursor += data_graph[i].length;
      if (i >= maxVertex)
      {
        data_graph[i].length = 0;
      } else
      {
        uint32_t count = 0;
        for (uint32_t j = 0; j < data_graph[i].length; j++)
        {
          if (data_graph[i].ptr[j] >= maxVertex)
          {
            break;
          }
          count++;
        }
        data_graph[i].length = count;
      }
    }
#else
      uint32_t cursor = 0;
      for (uint32_t i = 0; i < vertex_count; i++)
      {
        data_graph[i].length = graph_in_memory[cursor];
        data_graph[i].ptr = &graph_in_memory[++cursor];
        cursor += data_graph[i].length;
      }
#endif
      std::vector<std::vector<uint32_t>> new_labels(vertex_count+1);
      labels = new_labels;
      std::ifstream labels_file(data_graph_path / "labels.bin", std::ios::binary);
      if (labels_file)  // TODO: Need to modify this to support vertices with multiple labels rather than just one
      {
        uint32_t min_label = UINT_MAX;
        uint32_t max_label = 0;
        labelled_graph = true;

        for (uint32_t i = 0; i < vertex_count; ++i)
        {
          uint32_t buf[2];
          labels_file.read(reinterpret_cast<char *>(buf), 2*sizeof(uint32_t));

          uint32_t v = buf[0];
          uint32_t label = buf[1];
          min_label = std::min(min_label, label);
          max_label = std::max(max_label, label);
          labels[v].push_back(label);
        }

        label_range = {min_label, max_label};
      }
      else {
        if(RANDOMLY_GENERATE_LABELS_FOR_UNLABELLED_GRAPHS) { //Source on how to generate a skewed distribution: https://stackoverflow.com/questions/2395012/generating-random-numbers-from-skewed-normal-distribution
          uint32_t min_label = UINT_MAX;
          uint32_t max_label = 0;
          if (!labels_file) {
            std::vector<uint32_t> possible_labels_array;
            uint32_t counter = 1;
            for(uint32_t i=0;i<100;i++) {
              for(uint32_t j=0;j<pow(counter,2);j++) {
                possible_labels_array.push_back(i); 
              }
              counter++;
            }
            // for(uint32_t i = 0; i<100; i+=10) {
            //     uint32_t counts = 0;
            //     for(uint32_t j=0;j<10;j++) {
            //         uint32_t num_to_count = i+j;
            //         counts = counts + count(possible_labels_array.begin(),possible_labels_array.end(),num_to_count);
            //     }
            //     std::cout << "(possible_labels_array) number of labels from " << i << " to " << i+9 << " : " << counts << "\n"; 
            // }

            srand (1);
            for (uint32_t i = 1; i <= vertex_count; i++)
            {
              uint32_t num_of_labels_for_vertex = rand() % 5 + 1;
              for(uint32_t j=0;j<num_of_labels_for_vertex;j++) {
                double p=((double)rand()/(double)RAND_MAX);
                uint32_t l = possible_labels_array[floor(p*possible_labels_array.size())];
                labels[i].push_back(l);
                min_label = std::min(min_label, l);
                max_label = std::max(max_label, l);
              }
            }

            label_range = {min_label, max_label};
            labelled_graph = true;
          }
        }
      }

      {
        std::ifstream ids_file(data_graph_path / "ids.bin", std::ios::binary);

        if (!ids_file)
        {
          std::cerr << "WARNING: Could not open ID file for data graph." << std::endl;
        }
        else
        {
          ids = std::make_unique<uint32_t[]>(vertex_count+1);
          for (uint32_t i = 1; i <= vertex_count; ++i)
          {
            ids_file.read(reinterpret_cast<char *>(&ids[i]), sizeof(uint32_t));
          }
        }
      }
    }
    else if (std::filesystem::is_regular_file(data_graph_path))
    {
      from_smallgraph(path_str);
    }
    else
    {
      std::cerr << "ERROR: Data graph could not be opened." << std::endl;
      exit(1);
    }
  }

  DataGraph::DataGraph(DataGraph &&other)
    : rbi(other.rbi),
      vertex_count(other.vertex_count),
      edge_count(other.edge_count),
      forest_count(other.forest_count),
      labelled_graph(other.labelled_graph),
      labels(std::move(other.labels)),
      label_range(other.label_range),
      ids(std::move(other.ids)),
      data_graph(std::move(other.data_graph)),
      graph_in_memory(std::move(other.graph_in_memory)),
      known_labels(other.known_labels)
  {
    other.vertex_count = 0;
    other.edge_count = 0;
  }

  void DataGraph::set_rbi(const AnalyzedPattern &new_rbi)
  {
    rbi = new_rbi;
    forest_count = rbi.vgs.size();
    if (rbi.labelling_type() == Graph::PARTIALLY_LABELLED)
    {
      //TODO: Need to modify this to support vertices with multiple labels rather than just one
      uint32_t empty = static_cast<uint32_t>(-1);
      std::unordered_map<uint32_t,std::vector<uint32_t>>::const_iterator found = rbi.query_graph.labels.find({empty});
      new_label = std::distance(rbi.query_graph.labels.cbegin(),
            found);
    }
  }

  void DataGraph::set_known_labels(const std::vector<SmallGraph> &patterns)
  {
    for (const auto &p : patterns)
    {
        std::vector<uint32_t> labels_to_insert;
        for(auto labels: p.labels) {
            labels_to_insert.push_back(labels.second[0]); //TODO: Need to modify this to support vertices with multiple labels rather than just one
        }
        sort(labels_to_insert.begin(),labels_to_insert.end());
        known_labels.insert(labels_to_insert.cbegin(), labels_to_insert.cend());
    }
  }

  void DataGraph::set_known_labels(const std::vector<uint32_t> &labels)
  {
    known_labels.insert(labels.cbegin(), labels.cend());
  }

  bool DataGraph::known_label(const uint32_t label) const
  {
    return known_labels.contains(label);
  }

  bool DataGraph::is_labelled() const 
  {
    return labelled_graph;
  }

  const std::vector<uint32_t> &DataGraph::get_upper_bounds(uint32_t v) const
  {
    return rbi.upper_bounds[v];
  }

  const std::vector<uint32_t> &DataGraph::get_lower_bounds(uint32_t v) const
  {
    return rbi.lower_bounds[v];
  }

  const std::pair<uint32_t, uint32_t> &DataGraph::get_label_range() const
  {
    return label_range;
  }

  const adjlist &DataGraph::get_adj(uint32_t v) const
  {
    return data_graph[v-1];
  }

  const SmallGraph &DataGraph::get_vgs(unsigned vgsi) const
  {
    return rbi.vgs[vgsi];
  }

  uint32_t DataGraph::original_id(uint32_t v) const
  {
    return ids[v];
  }

  const SmallGraph &DataGraph::get_pattern() const {
    return rbi.query_graph;
  }

  const std::vector<std::vector<uint32_t>> &DataGraph::get_qs(unsigned vgsi) const
  {
    return rbi.qs[vgsi];
  }

  std::vector<uint32_t> DataGraph::label(uint32_t dv) const
  {
    return labels[dv];
  }

  uint32_t DataGraph::vmap_at(unsigned vgsi, uint32_t v, unsigned qsi) const
  {
    return rbi.vmap[vgsi][v][qsi];
  }

  const std::vector<uint32_t> &DataGraph::get_qo(uint32_t vgsi) const
  {
    return rbi.qo_book[vgsi];
  }

  uint32_t DataGraph::get_vgs_count() const
  {
    return rbi.vgs.size();
  }

  uint32_t DataGraph::get_vertex_count() const
  {
    return vertex_count;
  }

  uint64_t DataGraph::get_edge_count() const
  {
    return edge_count;
  }
}
