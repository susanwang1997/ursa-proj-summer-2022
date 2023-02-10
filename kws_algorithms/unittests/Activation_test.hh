#include <iostream>

using namespace std;

SUITE(ActivationTests)
{
    TEST(OneEdgeActivation) 
    {
        SmallGraph g;
        g.add_edge(1,2).set_label(1,1).set_label(2,2);
        DataGraph G(g);
        double alpha = 0.5;
        Activation act(G,alpha,average_shortest_distance(G));
        uint32_t* a = act.get_activation();
        CHECK_EQUAL(act.get_length(),2);
        CHECK_EQUAL(a[0],2);
        CHECK_EQUAL(a[1],2);
    }

    TEST(SquareGraphActivation) {
        SmallGraph g;
        g.add_edge(1,2).add_edge(2,3).add_edge(3,4).add_edge(4,1);
        DataGraph G(g);
        double alpha = 0.1;
        Activation act(G,alpha,average_shortest_distance(G));
        uint32_t* a = act.get_activation();
        CHECK_EQUAL(act.get_length(),4);
        CHECK_EQUAL(a[0],3);
        CHECK_EQUAL(a[1],3);
        CHECK_EQUAL(a[2],3);
        CHECK_EQUAL(a[3],3);
    }

    TEST(EightEdgesActivation) {
        SmallGraph g;
        g.add_edge(1,2).add_edge(1,3).add_edge(1,4).add_edge(4,5).add_edge(3,6).add_edge(5,7).add_edge(6,7).add_edge(5,6);
        DataGraph G(g);
        double alpha = 0.58;
        Activation act(G,alpha,average_shortest_distance(G));
        uint32_t* a = act.get_activation();
        CHECK_EQUAL(act.get_length(),7);
        CHECK_EQUAL(a[0],4);
        CHECK_EQUAL(a[1],0);
        CHECK_EQUAL(a[2],2);
        CHECK_EQUAL(a[3],2);
        CHECK_EQUAL(a[4],4);
        CHECK_EQUAL(a[5],4);
        CHECK_EQUAL(a[6],2);
    }

    TEST(FiveEdgesTreeActivation) {
        SmallGraph g;
        g.add_edge(1,2).add_edge(2,4).add_edge(1,6).add_edge(1,3).add_edge(3,5);
        g.set_label(1,1).set_label(2,2).set_label(3,3).set_label(4,21).set_label(5,3).set_label(6,2);
        DataGraph G(g);
        double alpha = 0.50;
        Activation act(G,alpha,average_shortest_distance(G));
        uint32_t* a = act.get_activation();
        CHECK_EQUAL(act.get_length(),6);
        CHECK_EQUAL(a[0],4);
        CHECK_EQUAL(a[1],2);
        CHECK_EQUAL(a[2],2);
        CHECK_EQUAL(a[3],0);
        CHECK_EQUAL(a[4],0);
        CHECK_EQUAL(a[5],0);
    }


}