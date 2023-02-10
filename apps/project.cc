#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>

#include "Peregrine.hh"

using namespace Peregrine;
using namespace std;

int main() {
  /*DataGraph g("data/my-graph");*/
  SmallGraph p;

 /*
  p.add_edge(1, 2)
  .add_edge(2, 3)
  .add_edge(3, 1)
  .set_label(1, 100)
  .set_label(2, 200)
  .set_label(3, 300);
  */
  p.add_edge(1, 2)
  .add_edge(2, 3)
  .add_edge(3, 4)
  .add_edge(4, 1)
  .set_label(1, 100)
  .set_label(2, 200)
  .set_label(3, 300)
  .set_label(4, 400);
  DataGraph g(p);
  uint32_t num_of_v = g.get_vertex_count();
  for(uint32_t i=1;i<=num_of_v;i++) {
    vector<uint32_t> neigbours = p.get_neighbours(i);
    cout << "neighbours[" << i << "] = [ ";
    for (uint32_t j=0;j<neigbours.size();j++){
      cout << neigbours[j] << " ";
    }
    cout << "] \n";

    cout << "ptr[" << i << "] = " << &(g.get_adj(i)) << "\n";

    cout << "adjlist[" << i << "] = [ ";
    for(uint32_t j=0 ; j<g.get_adj(i).length ; j++) {
      cout << *(g.get_adj(i).ptr + j) << " ";
    }
    cout << "] \n";


    cout << "label[" << i << "] = " << g.label(i) << "\n";
    cout << "-----\n";
  }
  cout << "number of vertices " << num_of_v << "\n";
  cout << "number of edges " << g.get_edge_count() << "\n";
  cout << endl;


  return 0;
}
