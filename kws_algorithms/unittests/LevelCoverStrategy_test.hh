#include <iostream>

using namespace std;

void compare_graph(SmallGraph g1, SmallGraph g2, uint32_t depth, uint32_t root, vector<uint32_t> keywords, uint32_t min_preserve_to_compare) {
    DataGraph dg(g1);
    Activation a(dg, 0.5, average_shortest_distance(dg));
    int min_to_preserve = get_min_keywords_to_preserve(g1,root,keywords);
    CHECK_EQUAL(min_to_preserve,min_preserve_to_compare);
    double lcs_get_min_kw[8]={0.0};
    double lcs_lcs[8]={0.0};
    double lcs_shortest_path[8]={0.0};

    SmallGraph sg = level_cover_strategy(g1,root,keywords,
    lcs_get_min_kw,lcs_lcs,lcs_shortest_path);
    if(sg.num_vertices() > 1)
    {
        preserve_original_edges_from_SmallGraph(g1,sg);
    }
    KWSGraph kg1(sg,a.get_weights(),depth,root);

    KWSGraph kg2(g2,a.get_weights(),depth,root);
    CHECK(kg1==kg2);
    kg1.print_graph();
    cout << "\n";
}

SUITE(LevelCoverStrategyTests)
{

    TEST(ThreeEdgesThreeUniqueNumbersLabelPrunedGraph)
    {
        SmallGraph g;
        g.add_edge(1,2).add_edge(1,3).add_edge(1,4);
        vector<uint32_t> l1 = {1};
        vector<uint32_t> l2 = {2,3};
        vector<uint32_t> l3 = {3};
        g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l3);
        vector<uint32_t> keywords = {1,2,3};

        SmallGraph g2;
        g2.add_edge(1,2).set_label(1,1).set_label(2,l2);

        cout << "Results for ThreeEdgesThreeUniqueNumbersLabelPrunedGraph\n";
        compare_graph(g,g2,1,1,keywords,2);
    }

    TEST(ThreeEdgesFourUniqueNumbersLabelPrunedGraph)
    {
        SmallGraph g;
        g.add_edge(1,2).add_edge(1,3).add_edge(1,4);
        vector<uint32_t> l1 = {1,2,32};
        vector<uint32_t> l2 = {2};
        vector<uint32_t> l3 = {3};
        g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l3);
        vector<uint32_t> keywords = {1,2,32};

        SmallGraph g2;
        g2.add_node(1).set_label(1,l1);

        cout << "Results for ThreeEdgesFourUniqueNumbersLabelPrunedGraph\n";
        compare_graph(g,g2,1,1,keywords,3);
    }

    TEST(FourEdgesFiveKWQueryPrunedGraph)
    {
        SmallGraph g;
        g.add_edge(1,2).add_edge(1,3).add_edge(1,4).add_edge(5,4);
        vector<uint32_t> l1 = {1};
        vector<uint32_t> l2 = {2};
        vector<uint32_t> l3 = {3};
        vector<uint32_t> l4 = {1,2,3,4,5};
        g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l3).set_label(5,l4);
        vector<uint32_t> keywords = {1,2,3,4,5};

        SmallGraph g2;
        g2.add_edge(1,4).add_edge(4,5).set_label(1,l1).set_label(4,l3).set_label(5,l4);

        cout << "Results for FourEdgesFiveKWQueryPrunedGraph\n";
        compare_graph(g,g2,2,1,keywords,5);
        
    }

    TEST(ThreeEdgesTwoUniqueNumbersPrunedGraph) {
        SmallGraph g;
        g.add_edge(1,2).add_edge(1,3).add_edge(1,4);
        vector<uint32_t> l1 = {1};
        vector<uint32_t> l2 = {2};
        vector<uint32_t> l3 = {1,2};
        g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l2);
        vector<uint32_t> keywords={1,2};

        SmallGraph g2;
        g2.add_edge(1,3).set_label(1,l1).set_label(3,l3);

        cout << "Results for ThreeEdgesTwoUniqueNumbersPrunedGraph\n";
        compare_graph(g,g2,2,1,keywords,2);
    }

    TEST(SevenEdgesTwoKWQueryPrunedGraph) {
        SmallGraph g;
        g.add_edge(1,2).add_edge(1,3).add_edge(1,4).add_edge(3,5).add_edge(2,6).add_edge(4,6).add_edge(5,6);
        vector<uint32_t> l1 = {0};
        vector<uint32_t> l2 = {1};
        vector<uint32_t> l3 = {2};
        vector<uint32_t> l4 = {3};
        vector<uint32_t> l5 = {4};
        vector<uint32_t> l6 = {2,5};
        g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4).set_label(5,l5).set_label(6,l6);
        vector<uint32_t> keywords = {2,5};
        
        SmallGraph g2;
        g2.add_edge(1,2).add_edge(1,4).add_edge(2,6).add_edge(4,6);
        g2.set_label(1,l1).set_label(2,l2).set_label(4,l4).set_label(6,l6);

        cout << "Results for SevenEdgesTwoKWQueryPrunedGraph\n";
        compare_graph(g,g2,3,1,keywords,2);
    }

    TEST(TenEdgesTwoKWQueryPrunedGraph) {
        SmallGraph g;
        g.add_edge(3,10).add_edge(3,4).add_edge(3,5).add_edge(10,6).add_edge(4,7).add_edge(5,8).add_edge(6,9).add_edge(7,9).add_edge(8,1).add_edge(9,2);
        
        vector<uint32_t> l1 = {6};
        vector<uint32_t> l2 = {5};
        vector<uint32_t> l3 = {4};
        vector<uint32_t> l4 = {3};
        vector<uint32_t> l5 = {2};
        vector<uint32_t> l6 = {9,9,9,5};
        vector<uint32_t> l7 = {1,6,3};
        vector<uint32_t> l8 = {8};
        g.set_label(3,l1).set_label(10,l1).set_label(4,l2).set_label(5,l2).set_label(6,l3).set_label(7,l4).set_label(8,l5).set_label(9,l6).set_label(1,l7).set_label(2,l8);
        vector<uint32_t> k = {1,6,3};

        SmallGraph g2;
        g2.add_edge(3,5).add_edge(5,8).add_edge(8,1).set_label(3,l1).set_label(5,l2).set_label(8,l5).set_label(1,l7);

        cout << "Results for TenEdgesTwoKWQueryPrunedGraph\n";
        compare_graph(g,g2,4,3,k,3);
    }

    TEST(TenEdgesThreeKWQueryPrunedGraph) {
        SmallGraph g;
        g.add_edge(3,10).add_edge(3,4).add_edge(3,5).add_edge(10,6).add_edge(4,7).add_edge(5,8).add_edge(6,9).add_edge(7,9).add_edge(8,1).add_edge(9,2);
        
        vector<uint32_t> l1 = {6};
        vector<uint32_t> l2 = {5};
        vector<uint32_t> l3 = {4};
        vector<uint32_t> l4 = {3};
        vector<uint32_t> l5 = {2};
        vector<uint32_t> l6 = {9,9,9,5};
        vector<uint32_t> l7 = {1,6,3};
        vector<uint32_t> l8 = {9,5};
        g.set_label(3,l1).set_label(10,l1).set_label(4,l2).set_label(5,l2).set_label(6,l3).set_label(7,l4).set_label(8,l5).set_label(9,l6).set_label(1,l7).set_label(2,l8);
        vector<uint32_t> k = {5,9};

        SmallGraph g2;
        g2.add_edge(3,10).add_edge(4,3).add_edge(6,10).add_edge(7,4).add_edge(7,9).add_edge(6,9).add_edge(9,2);
        g2.set_label(3,l1).set_label(10,l1).set_label(4,l2).set_label(6,l3).set_label(7,l4).set_label(9,l6).set_label(2,l8);


        cout << "Results for TenEdgesThreeKWQueryPrunedGraph\n";
        compare_graph(g,g2,4,3,k,2);
    }

    TEST(TwelveEdgesFiveKWQueryPrunedGraph) {
        SmallGraph g;
        g.add_edge(3,10).add_edge(3,4).add_edge(3,5).add_edge(10,6).add_edge(4,7).add_edge(5,8).add_edge(6,9).add_edge(7,9).add_edge(8,1).add_edge(9,2).add_edge(6,7).add_edge(9,3);
        
        vector<uint32_t> l1 = {6};
        vector<uint32_t> l2 = {5};
        vector<uint32_t> l3 = {4};
        vector<uint32_t> l4 = {3};
        vector<uint32_t> l5 = {9,9,9,5};
        vector<uint32_t> l6 = {1};
        vector<uint32_t> l7 = {8};
        
        vector<uint32_t> k = {3,4,5,6,9};
        g.set_label(3,l1).set_label(10,l1).set_label(4,l2).set_label(5,l6).set_label(6,l3).set_label(7,l4).set_label(8,l6).set_label(9,l5).set_label(1,l6).set_label(2,l7);
        SmallGraph g2;
        g2.add_edge(3,10).add_edge(4,3).add_edge(6,10).add_edge(7,4).add_edge(7,9).add_edge(6,9).add_edge(6,7).add_edge(9,3);
        g2.set_label(3,l1).set_label(10,l1).set_label(4,l2).set_label(6,l3).set_label(7,l4).set_label(9,l5);
        

        cout << "Results for TwelveEdgesFiveKWQueryPrunedGraph\n";
        compare_graph(g,g2,4,3,k,1);
    }

    TEST(ElevenEdgesFourKWQueryPrunedGraph) {
        SmallGraph g;
        g.add_edge(3,10).add_edge(3,4).add_edge(3,5).add_edge(10,6).add_edge(4,7).add_edge(5,8).add_edge(6,9).add_edge(7,9).add_edge(8,1).add_edge(9,2).add_edge(6,7);
        vector<uint32_t> l1 = {6};
        vector<uint32_t> l2 = {5};
        vector<uint32_t> l3 = {3};
        vector<uint32_t> l4 = {2};
        vector<uint32_t> l5 = {1,6,3};
        vector<uint32_t> l6 = {1};
        vector<uint32_t> l7 = {0};
        vector<uint32_t> l8 = {4};
        g.set_label(3,l1).set_label(10,l1).set_label(4,l2).set_label(5,l2).set_label(6,l8).set_label(7,l3).set_label(8,l4).set_label(9,l6).set_label(1,l5).set_label(2,l7);
        vector<uint32_t> k = {3,4,5,6};

        SmallGraph g2;
        g2.add_edge(3,10).add_edge(4,3).add_edge(6,10).add_edge(7,4).add_edge(6,7).add_edge(3,5).add_edge(5,8).add_edge(8,1);
        g2.set_label(3,l1).set_label(10,l1).set_label(4,l2).set_label(6,l8).set_label(7,l3).set_label(5,l2).set_label(8,l4).set_label(1,l5);

        cout << "Results for ElevenEdgesFourKWQueryPrunedGraph\n";
        compare_graph(g,g2,4,3,k,1);
    }


    TEST(SixEdgesOneKWQueryPrunedGraph) {
        SmallGraph g;
        SmallGraph other_g;
        vector<uint32_t> l1 = {1,0};
        vector<uint32_t> l2 = {2,0};
        vector<uint32_t> l3 = {3,0};
        g.add_edge(1,2).add_edge(2,3).add_edge(3,1).set_label(1,l1).set_label(2,l2).set_label(3,l3);
        other_g.add_edge(1,2).set_label(1,l1).set_label(2,l2);
        vector<uint32_t> k  = {1};

        SmallGraph g2;
        g2.add_node(1).set_label(1,l1);

        cout << "Results for SixEdgesOneKWQueryPrunedGraph (1)\n";
        compare_graph(g,g2,1,1,k,1);
        cout << "Results for SixEdgesOneKWQueryPrunedGraph (2)\n";
        compare_graph(other_g,g2,1,1,k,1);

        vector<uint32_t> k2  = {2};

        SmallGraph g3;
        g3.add_edge(1,2).set_label(1,l1).set_label(2,l2);
        cout << "Results for SixEdgesOneKWQueryPrunedGraph (3)\n";
        compare_graph(g,g3,1,1,k2,1);
        cout << "Results for SixEdgesOneKWQueryPrunedGraph (4)\n";
        compare_graph(other_g,g3,1,1,k2,1);
    }

    TEST(SingleNodePrunedGraph) {
        SmallGraph g;
        vector<uint32_t> l = {1};
        g.add_node(1).set_label(1,l);
        vector<uint32_t> k  = {1};

        SmallGraph g2;
        g2.add_node(1).set_label(1,l);

        cout << "Results for SingleNodePrunedGraph\n";
        compare_graph(g,g2,1,1,k,1);
    }

}