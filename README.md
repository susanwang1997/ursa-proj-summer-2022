# USRA Project Summer 2022

# 1. Keyword search on Knowledge Graphs
This project is an implementation of algorithms from a research paper[[1]](#4-references) about parallel keyword search algorithms.

The algorithm looks at a graph and gets a query with a list of keywords (positive integers) as input. It finds subgraphs of the graph containing labels that match the query or similar results and return them in a heap of size k defined by the user.

Each subgraph has a score which is calculated using a scoring function. Details about the scoring function can be found in the paper. The k subgraphs with the top-k scores are returned in the heap.

Presentation slides: Inside directory `kws/Peregrine/Slides`  

Paper: https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=8731500&tag=1

Directory for project: /kws

## 1.1 Quick start
This project has incorporated the Peregrine framework. Here are the instructions to set up Peregrine (from: https://github.com/pdclab/peregrine)

Peregrine has been tested on Ubuntu 18.04 and Arch Linux but should work on any
POSIX-y OS. It requires C++20 support (GCC version >= 10.1). Additionally, the
tests require [UnitTest++](https://github.com/unittest-cpp/unittest-cpp).

Ubuntu 18.04 prerequisites:
```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt install g++-10 libunittest++-dev
```

## 1.2 How to run the code:

### 1.2.1 Standard way
The following are the steps to run the code in the standard way:

1. Clone the code from this repo
2. Go to the directory `<path where you store the folder>/KeywordSearch/kws/Peregrine`
3. In the command line, type `source tbb2020/bin/tbbvars.sh intel64`
4. In the command line, run `make kws`
5. Run `bin/kws <graph path> \"[query]\" (<alpha>) (<heap size>) (<depth>) (<num threads>)`

The graph path is a path to a folder with binary files that represent graph information. 

Setting the alpha value, heap size, depth, and number of threads is optional. 

By default, alpha is set to 0.1, heap size is 20, depth is 10. The default number of threads being run will be the max number of threads supported by your machine.

Alpha must be a double in between (but not including) 0 and 1. The keywords in the query, heap size, depth, and number of threads must be a positive integer.

#### 1.2.1.1 Example of how to run the code (Standard way)
Here is an example of how to run the code in the standard way. Let's say that you want to search from the graph located in `data/my-graph4` and search for nodes containing labels 1 and 2. 

If you want to use the default values for alpha, heap size, depth, and num threads, you can just type `bin/kws data/my-graph4 "[1,2]"`. 

Let's say that you also want alpha to be set to 0.4, the heap size set to 15, the max depth set to 5, and the number of threads set to 4. In that case, you type `bin/kws data/my-graph4 "[1,2]" 0.4 15 5 4` in the command line. 

#### 1.2.1.2 Query syntax
The query can be specified using the syntax `"[#,#,# ... ]"` where `#` represents a number. The query must be in quotations and the list of keywords queried must be inside square brackets. Numbers must be separated by commas.

Spaces are allowed between commas and numbers (e.g. can type something like `"[1, 2, 3]"`). Note that if you separate two numbers by a space without commas, it will be interpreted as one number (e.g. `"[1, 2, 3 4]"` will be interpreted as `"[1, 2, 34]"` since there was no comma between 3 and 4). 

The `...` represents an unspecified length of the query but it does not actually belong in the syntax. For example, if you want to query nodes with labels 24, 6, and 12, you write the query as `"[24,6,12]"`.

#### 1.2.1.3 Prompt from the program during runtime
When you run the program, the program will print some information of each graph from the top-k heap . The program will ask you if you want to print the edges of the graph. Type `y` if you want to print the edges, otherwise type `n`. 

Note that if you compile the program by setting a flag called `TEST`, you won't get this prompt. See the [subsection about additional flags](#13-additional-flags-you-can-set) for more details.

#### 1.2.1.4 Checking race conditions or errors
You can also test whether the program has a race condition by running the code an infinite amount of times until the heap produces an incorrect output by running `bin/kws <path to graph> "[query]" <alpha> <heap_size> <depth> <num_threads> -rc`. The heaps generated for each run will be compared to the heap generated from a run that was performed sequentially (single threaded) and there will be a check to see if the heaps are identical. If the heaps aren't identical, it is assumed that there is a race condition.

Setting alpha, heap_size, depth, and threads are not optional for this case. To exit the program with the race condition check, press CTRL+C.

### 1.2.2 Performance evaluation
There is a way to do performance evaluation in the program. The performance evaluation checks the average time between different steps in the two stage parallel algorithm by running the keyword search algorithm for 50 iterations and recording the average time for all iterations. The evaluation also reports the max, min, and average number of vertices and edges for every step after top down processing and for every graph in the heap for all iterations.

How it works is that the program will randomly generate *knum* keywords in a query in an uniform distribution, pass the query through the keyword search algorithm, and repeat this for 50 runs. Afterwards, the program averages the time between different steps in two stage parallel for all runs and reports the statistics for the max/min/average number of vertices/edges.

The following are the steps to run the performance evalution:

1. Clone the code from this repo
2. Go to the directory `<path where you store the folder>/KeywordSearch/kws/Peregrine`
3. In the command line, type `source tbb2020/bin/tbbvars.sh intel64`
4. In the command line, run `make kws`
5. Run `bin/kws <graph path>`

The graph path is a path to a folder with binary files that represent graph information. 

By default, knum is set to 6, alpha is set to 0.1, heap size is 20, depth is 10. The number of threads being run will be the max number of threads supported by your machine by default.

If you want to change the default knum, alpha, heap size, or number of threads during compilation, see section [1.3](#13-additional-flags-you-can-set).

## 1.3 Additional flags you can set
This program produces a vertex induced solution by default. But if you want an edge induced solution, you must set a flag called `EDGE_INDUCED`. You can compile the program as `make EDGE_INDUCED=1 kws` to make the program generate the edge induced solution.

This program prints the runtimes of different sections of the program and each graph in the heap by default. If you do not wish to print the runtime or the results of the heap, you can set a flag called `TEST` and compile the program as `make TEST=1 kws` to get rid of these print statements from the program (note that if you do performance evaluation, the times for the two stage parallel will still get printed, but results for the max/min/average vertices and edges will be omitted).

You can change the default knum, the heap size, the alpha value, and number of threads being run by setting the `KNUM`, `HEAP_SIZE`, `ALPHA`, `MAX_DEPTH`, and `THREADS` flag when compiling the program with the syntax `make PARAMS="-DKNUM=<knum> -DHEAP_SIZE=<heap size> -DALPHA=<alpha> -DTHREADS=<num threads> -DMAX_DEPTH=<depth>" kws`. E.g. `make PARAMS="-DKNUM=3 -DHEAP_SIZE=10 -DALPHA=0.5 -DTHREADS=4 -DMAX_DEPTH=5" kws` to change the knum, heap size, alpha, number of threads, and max depth. You can also choose to change some of the parameters only (e.g. `make PARAMS=-DALPHA=0.2 kws` if you just want to change alpha to 0.2).

You can also set multiple variables at the same time (e.g. `make TEST=1 EDGE_INDUCED=1 PARAMS="-DKNUM=3 -DHEAP_SIZE=10 -DALPHA=0.5 -DTHREADS=4" kws`).

Alpha must be a double in between (but not including) 0 and 1. The knum, heap size, number of threads, and max depth must be a positive integer.

## 1.4 Small graphs which can be used for manual testing
In the `/data` folder, there are graphs called my-graph# where # represents a number. They are small graphs that can be used for manual testing. 

To look at the edges and labels that correspond to those graphs, go to `data/txt/`. There are folders called g# where each number # corresponds to the graph in `data/my-graph#`. 

These folders contain text files called edges.txt and labels.txt that show the edges and labels for the graph. For example, if you want to find the labels and edges for `data/my-graph4`, go to `data/txt/g4` to find the text files that correspond to the labels and edges.

## 1.5 Unit tests
The unit tests can be found in the unittests folder inside the kws_algorithms folder. The code which runs all tests are in tests.cc inside the kws_algorithms folder. To build and run the unit tests, call `make kws_tests` and `bin/kws_tests`. 

You can also test whether the program has a race condition by running the test cases an infinite amount of times until at least one of the test cases fail by running `bin/kws_tests -rc`. To exit the program with the race condition check, press CTRL+C.

# 2. About the code for this project
In this repository, all files for the implementation of the keyword search algorithm can be found in the directory `kws/Peregrine/kws_algorithms`, but the executable for the program will be located in `kws/Peregrine/bin` where the name of the executable will be `kws` when compiled.

Here is what each file for the project is used for:
- main.cc - The main program which calls an algorithm (top down processing) that returns a heap with the graph results of the query

- Matrix.hh - Class which initalizes a node-keyword matrix (2d array) where for each element M(i,j) (ith row/jth column), M(i,j)=0 if the ith vertex contains the jth keyword, otherwise M(i,J)=infinity. It will be used to record the hitting level of a vertex in a graph.

- activation.hh - Class which intializes an array of the activation levels for all vertices in a graph (at which level can a node expand and look at neighbours)

- weights.hh - Class which initalizes and normalizes weights for all vertices of the graph based on the number of edges for each vertex

- AverageShortestDistance.hh - Computes the average shortest distance for all pairs of vertices in a graph using the Floyd-Warshall algorithm

- keywords.hh - Class which stores each keyword from the query defined by the user. Also has a function called `contains_keywords` to check if a label has numbers contained in the query. 

- TwoStageParallel.hh - Code for the two stage parallel algorithm that was defined in the paper. Two stage parallel is ran in a function called `get_heap` which returns the heap with the graph results.

- Expansion.hh - Code for the expansion algorithm that was defined in the paper

- TopDownProcessing.hh - Code for the top down processing algorithm defined in the paper

- LevelCoverStrategy.hh - An implementation of the level cover strategy algorithm that was proposed by the paper

- PreserveOriginalEdges.hh - Functions which take a graph (original_graph) and the subgraph of the graph (sub_graph) and puts all edges from original_graph into sub_graph for all the vertices contained in sub_graph. This is intended to transform sub_graph into a vertex induced subgraph of original_graph. sub_graph must be a SmallGraph, but there are 2 different functions which account for an original_graph that is either a DataGraph or another SmallGraph.  

- KWSGraph.hh - (KWSGraph = Keyword Search Graph) A class that stores a graph, a vector with list of vertex ids (called v_list), the central node/root of the graph, a score of the graph based off a scoring function defined in the paper, and the depth that was used to calculate the score of a graph. The graphs that were part of the results of the heap gets stored in this type of data structure. 

- NewDataGraph.cc/hh - The class for the data structure DataGraph

- NewGraph.hh - The class for the data structure SmallGraph

- VecOps.cc/hh - Functions that have various vector operations. (Converting an unsigned int vector into a string or vice versa, getting the average/min/max of a type of vector)

- tests.cc - Runs all unit tests from the folder kws_algorithms/unittests/*

- All .hh files from kws_algorithms/unittests/* are unit tests for the files I listed above

## 2.1 How to construct graphs through the libraries of this project
From Peregrine, there were files called Graph.hh, DataGraph.hh, and DataGraph.cc which allowed a user to construct a graph and define it with vertex ids, edges, and labels. 

For this project, a copy of Graph.hh was created and named NewGraph.hh. Copies of DataGraph.cc and DataGraph.hh was created and named NewDataGraph.cc and NewDataGraph.hh respectively. These files can be found in `kws/Peregrine/kws_algorithms`. The classes in these files are in a namespace called `NewGraph`.

Like with Peregrine, the libraries assume that the graphs are undirected.

Graphs can constructed using the `SmallGraph` and `DataGraph` data structure.

### 2.1.1 Dynamically, using the builder methods (SmallGraph construction)

Like with Peregrine, you can create an empty graph and add edges one by one. The vertex ids must be positive integers and start at 1.

Here is an example of how to generate a SmallGraph through code:

```
SmallGraph p;
p.add_edge(1, 2).add_edge(1, 3).set_label(1, 100).set_label(2, 101);
```
**NEW: Multiple label support**
Unlike Peregine, the code for this program allows the creation of graphs with more than one label. The multiple labels must be in a vector of the form `vector<uint32_t>`. An example of this can be shown below:

```
SmallGraph g;
g.add_edge(1,2).add_edge(1,3).add_edge(1,4).add_edge(1,5).add_edge(5,6).add_edge(3,6);
vector<uint32_t> l1 = {12};
vector<uint32_t> l2 = {31,13};
vector<uint32_t> l3 = {24};
vector<uint32_t> l4 = {35};
vector<uint32_t> l5 = {57};
vector<uint32_t> l6 = {99, 66, 0, 4};
g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4).set_label(5,l5).set_label(6,l6);
```

**NEW: Single vertex support**
In this code, you can define a SmallGraph with a single vertex or multiple single vertices by calling `add_node(uint32_t vertex_id)` Examples of this can be shown below:

```
SmallGraph g;
vector<uint32_t> l1 = {1,2,3,4};
g.add_node(1).set_label(1,l1);

SmallGraph g2;
vector<uint32_t> l1_2 = {1,7};
vector<uint32_t> l2_2 = {2};
vector<uint32_t> l3_2 = {3};
g2.add_node(1).set_label(1,l1_2).add_node(2).set_label(2,l2_2).add_node(3).set_label(3,l3_2);

```

### 2.1.2 Importing text files of graphs (SmallGraph construction)
Another way of creating a SmallGraph is to load a text file of the graph into SmallGraph. It is done in the same way as Peregrine. i.e. (`SmallGraph p("pattern_graph.txt");`) Details about how to create the text file can be found in the README from this page https://csil-git1.cs.surrey.sfu.ca/pdcl/summer22-usra-susan/-/tree/master/kws/Peregrine#2-writing-your-own-programs **(The page directs you to a new link)**. 

Do note that the code currently does not support importing text files with vertices containing more than one label. So if you use this method, you can only define one label for a vertex.

If a text file contains a partially labelled graph and gets exported to SmallGraph and that SmallGraph gets exported to DataGraph, the DataGraph will not contain the labels from the original graph. 

### 2.1.3 Convert SmallGraph into DataGraph 
SmallGraphs can be converted to DataGraphs as follows:
```
DataGraph data_graph(p); //where p is a SmallGraph
```

### 2.1.4 Importing binary files of graphs (DataGraph construction)
You can create a graph with the `DataGraph` data structure by importing bin files. 

You must have a file named data.bin and ids.bin to represent the edges and vertex ids of the graph. If the graph is labelled, then it must be stored as a file named labels.bin. But having a labels file is optional since unlabelled graphs can be imported.

For example, if you have bin files in a folder called preprocessed_data in the current path, you can consturct a DataGraph like this: 

```
DataGraph dg("preprocessed_data/");
```

You can create a text file to represent graphs and convert it into a binary file. Instructions on how to convert text files to binary files can be found in the README from this page https://csil-git1.cs.surrey.sfu.ca/pdcl/summer22-usra-susan/-/tree/master/kws/Peregrine#3-data-graphs **(The page directs you to a new link)**. 

Do note that the code currently does not support importing binary files with vertices containing more than one label. So if you use this method, you can only define one label for a vertex.

##### 2.1.4.1 Note for bin files of unlabelled graphs
If the binary files contains no labels (i.e. the graph folder does not have a file for labels called labels.bin), then the program randomly generates labels from 0 to 99 for each vertex, where higher numbers have a higher chance of occurence in the graph. Each vertex is randomly given 1 to 5 labels.

# 3. Important information about average shortest distance
The keyword search project contains an algorithm which calculates the average shortest distance for all pairs of vertices in a graph 
using the Floyd-Warshall algorithm by randomly sampling approximately ~10,000 vertices. The algorithm is time consuming and takes about ~600-800s. 

However, when the algorithm finishes, it creates a folder called `asd` and stores the result in a text file inside the folder. The `asd` folder gets stored inside the folder or path of the graph. 

This way, the next time the program runs on the same graph, it can load the file to speed up the time. 

If you want to re-run the average shortest distance algorithm, you would have to remove the `asd` folder from the graph path.

# 4. References
[1] Y. Yang, D. Agrawal, H. V. Jagadish, A. K. H. Tung and S. Wu, "An Efficient Parallel Keyword Search Engine on Knowledge Graphs," 2019 IEEE 35th International Conference on Data Engineering (ICDE), 2019, pp. 338-349, doi: 10.1109/ICDE.2019.00038. 


# Contact
Any questions about the code can be directed to the creator of this codebase (Susan Wang) at jyw30@sfu.ca or susanatbc@gmail.com.
