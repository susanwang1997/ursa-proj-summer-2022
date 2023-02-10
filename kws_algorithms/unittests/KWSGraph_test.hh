#include <iostream>
#include <algorithm>

using namespace std;

SUITE(KWSGraphTests)
{
    TEST(TriangleKWSGraph)
    {
        SmallGraph p;
        p.add_edge(1, 2).add_edge(1, 3).add_edge(2, 3).set_label(1, 100).set_label(2, 200).set_label(3, 300);
        Weights w(p);
        KWSGraph k(p,w.get_weights(),1,1);
        SmallGraph g = k.get_graph();
        vector<uint32_t> nodes = k.get_v_list();
        unordered_map<int,int> indexes = get_indexes(nodes);

        CHECK_EQUAL(k.get_score(), 3);
        CHECK_EQUAL(k.get_depth(), 1);
        CHECK_EQUAL(k.get_root(), 1);
        CHECK_EQUAL(g.num_vertices(), 3);
        vector<uint32_t> l = {100};
        CHECK_EQUAL(g.label(1)[0], l[0]);
        l = {200};
        CHECK_EQUAL(g.label(2)[0], l[0]);
        l = {300};
        CHECK_EQUAL(g.label(3)[0], l[0]);
        CHECK_EQUAL(indexes[1], 0);
        CHECK_EQUAL(indexes[2], 1);
        CHECK_EQUAL(indexes[3], 2);
        CHECK(find(nodes.begin(), nodes.end(), 1) != nodes.end());
        CHECK(find(nodes.begin(), nodes.end(), 2) != nodes.end());
        CHECK(find(nodes.begin(), nodes.end(), 3) != nodes.end());
        CHECK_EQUAL(find(nodes.begin(), nodes.end(), 4) != nodes.end(),false);
    }

    TEST(FiveEdgesKWSGraph)
    {
        SmallGraph p;
        p.add_edge(1, 2).add_edge(1, 3).add_edge(1, 4).add_edge(4, 5).add_edge(5, 3);
        p.set_label(1, 800).set_label(2, 200).set_label(3, 300).set_label(4, 400).set_label(5, 500);
        Weights w(p);
        KWSGraph k(p,w.get_weights(),10,1);
        CHECK_CLOSE(k.get_score(), 4.3662, 0.0001);
    }

    TEST(OneEdgeKWSGraph)
    {
        SmallGraph p;
        p.add_edge(1,2).set_label(1,100).set_label(2,200);
        Weights w(p);
        KWSGraph k(p,w.get_weights(),2,2);
        CHECK_CLOSE(k.get_score(), 2.2974, 0.0001);
    }
}

