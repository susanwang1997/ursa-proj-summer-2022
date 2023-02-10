#include <iostream>
#include <limits>

using namespace std;

SUITE(MatrixTests) {
    TEST(OneEdgeOneKWQueryMatrix) {
        SmallGraph g;
        vector<uint32_t> l1 = {1};
        vector<uint32_t> l2 = {2};
        g.add_edge(1,2).set_label(1,l1).set_label(2,l2);
        DataGraph G(g);
        vector<uint32_t> k = {2};
        uint32_t l = 1;
        Matrix m(G,k);
        CHECK_EQUAL(m.get_num_queries(),l);
        CHECK_EQUAL(m.get_num_vertices(),G.get_vertex_count());
        uint32_t** matrix = m.get_matrix();
        CHECK_EQUAL(matrix[0][0],numeric_limits<uint32_t>::max());
        CHECK_EQUAL(matrix[1][0],0);
    }

    TEST(OneEdgeTwoKWQueryMatrix) {
        SmallGraph g;
        vector<uint32_t> l1 = {1};
        vector<uint32_t> l2 = {2};
        g.add_edge(1,2).set_label(1,l1).set_label(2,l2);
        DataGraph G(g);
        vector<uint32_t> k = {1,2};
        uint32_t l = 2;
        Matrix m(G,k);
        CHECK_EQUAL(m.get_num_queries(),l);
        CHECK_EQUAL(m.get_num_vertices(),G.get_vertex_count());
        uint32_t** matrix = m.get_matrix();
        CHECK_EQUAL(matrix[0][0],0);
        CHECK_EQUAL(matrix[1][0],numeric_limits<uint32_t>::max());
        CHECK_EQUAL(matrix[0][1],numeric_limits<uint32_t>::max());
        CHECK_EQUAL(matrix[1][1],0);
    }

    TEST(OneEdgeThreeKWQueryMatrix) {
        SmallGraph g;
        vector<uint32_t> l1 = {1};
        vector<uint32_t> l2 = {2};
        g.add_edge(1,2).set_label(1,l1).set_label(2,l2);
        DataGraph G(g);
        vector<uint32_t> k = {1,2,3};
        uint32_t l = 3;
        Matrix m(G,k);
        CHECK_EQUAL(m.get_num_queries(),l);
        CHECK_EQUAL(m.get_num_vertices(),G.get_vertex_count());
        uint32_t** matrix = m.get_matrix();
        CHECK_EQUAL(matrix[0][0],0);
        CHECK_EQUAL(matrix[1][0],numeric_limits<uint32_t>::max());
        
        CHECK_EQUAL(matrix[0][1],numeric_limits<uint32_t>::max());
        CHECK_EQUAL(matrix[1][1],0);

        CHECK_EQUAL(matrix[0][2],numeric_limits<uint32_t>::max());
        CHECK_EQUAL(matrix[1][2],numeric_limits<uint32_t>::max());
    }

    TEST(OneEdgeOneKWQueryNoMatchesMatrix) {
        SmallGraph g;
        vector<uint32_t> l1 = {1};
        vector<uint32_t> l2 = {2};
        g.add_edge(1,2).set_label(1,l1).set_label(2,l2);
        DataGraph G(g);
        vector<uint32_t> k = {6};
        uint32_t l = 1;
        Matrix m(G,k);
        CHECK_EQUAL(m.get_num_queries(),l);
        CHECK_EQUAL(m.get_num_vertices(),G.get_vertex_count());
        uint32_t** matrix = m.get_matrix();
        CHECK_EQUAL(matrix[0][0],numeric_limits<uint32_t>::max());
        CHECK_EQUAL(matrix[1][0],numeric_limits<uint32_t>::max());
    }
}