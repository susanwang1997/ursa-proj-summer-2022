#include <iostream>

using namespace std;

SUITE(ASDTest) {
    TEST(TriangleASD) {
        SmallGraph g;
        SmallGraph g2;
        g.add_edge(1,2).add_edge(2,3).add_edge(3,1);
        g2.add_edge(1,2);
        DataGraph G(g);
        DataGraph G2(g2);
        CHECK_EQUAL(average_shortest_distance(G),1);
        CHECK_EQUAL(average_shortest_distance(G2),1);
    }

    TEST(SquareASD) {
        SmallGraph g;
        g.add_edge(1,2).add_edge(2,3).add_edge(3,4).add_edge(4,1);
        DataGraph G(g);
        CHECK_CLOSE(average_shortest_distance(G),1.3333,0.0001);
    }

    TEST(EightEdgesASD) {
        SmallGraph g;
        g.add_edge(1,2).add_edge(1,3).add_edge(1,4).add_edge(4,5).add_edge(3,6).add_edge(5,7).add_edge(6,7).add_edge(5,6);
        DataGraph G(g);
        CHECK_CLOSE(average_shortest_distance(G),1.8571,0.0001);
    }

    TEST(FiveEdgesTreeASD) {
        SmallGraph g;
        g.add_edge(1,2).add_edge(2,4).add_edge(1,6).add_edge(1,3).add_edge(3,5);
        DataGraph G(g);
        CHECK_CLOSE(average_shortest_distance(G),2.06667,0.0001);
    }
}