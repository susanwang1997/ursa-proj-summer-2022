#include <iostream>

using namespace std;

SUITE(TwoStageParallelTest) {
    TEST(FiveEdgesTreeTwoKWQueryTSPTest) {
        cout << "\nResults for FiveEdgesTreeTwoKWQueryTSPTest: \n";
        SmallGraph g;
        g.add_edge(1,2).add_edge(2,4).add_edge(1,6).add_edge(1,3).add_edge(3,5);
        vector<uint32_t> l1 = {1};
        vector<uint32_t> l2 = {2};
        vector<uint32_t> l3 = {3};
        vector<uint32_t> l4 = {2,1};
        g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4).set_label(5,l3).set_label(6,l2);
        DataGraph dg(g);
        Activation a(dg, 0.5, average_shortest_distance(dg));
        vector<uint32_t> k = {1,2};
        vector<KWSGraph> heap = get_heap(dg, k, 0.5, 5, 4);

        SmallGraph sg1;
        sg1.add_edge(1,6);
        sg1.set_label(1,l1).set_label(6,l2);
        KWSGraph kg1(sg1,a.get_weights(),1,1);
        CHECK(heap[0]==kg1);

        SmallGraph sg2;
        sg2.add_node(4).set_label(4,l4);
        KWSGraph kg2(sg2,a.get_weights(),0,4);
        CHECK(heap[1]==kg2);

        CHECK(heap.size()==2);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(FiveEdgesTreeOneKWQueryNoResultsTSPTest) {
        cout << "\nResults for FiveEdgesTreeOneKWQueryNoResultsTSPTest: \n";
        SmallGraph g;
        vector<uint32_t> l1 = {1};
        vector<uint32_t> l2 = {2};
        vector<uint32_t> l3 = {3};
        vector<uint32_t> l4 = {2,1};
        g.add_edge(1,2).add_edge(2,4).add_edge(1,6).add_edge(1,3).add_edge(3,5);
        g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4).set_label(5,l3).set_label(6,l2);
        DataGraph dg(g);
        vector<uint32_t> k = {8};
        vector<KWSGraph> heap = get_heap(dg, k, 0.5, 5, 4);
        CHECK(heap.size()==0);
        print_heap(heap);
    }

    TEST(SixEdgesTwoKWQueryTSPTest) {
        cout << "\nResults for SixEdgesTwoKWQueryTSPTest: \n";
        SmallGraph g;
        g.add_edge(1,2).add_edge(2,4).add_edge(1,6).add_edge(1,3).add_edge(3,5).add_edge(2,6);
        vector<uint32_t> l1 = {1};
        vector<uint32_t> l2 = {2};
        vector<uint32_t> l3 = {3};
        vector<uint32_t> l4 = {2,1};
        g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4).set_label(5,l3).set_label(6,l2);
        DataGraph dg(g);
        Activation a(dg, 0.5, average_shortest_distance(dg));
        vector<uint32_t> k = {1,2};
        vector<KWSGraph> heap = get_heap(dg, k, 0.5, 5, 4);

        SmallGraph sg1;
        sg1.add_edge(1,6);
        sg1.set_label(1,l1).set_label(6,l2);
        KWSGraph kg1(sg1,a.get_weights(),3,1);
        CHECK(heap[0]==kg1);

        SmallGraph sg2;
        sg2.add_node(4).set_label(4,l4);
        KWSGraph kg2(sg2,a.get_weights(),0,4);
        CHECK(heap[1]==kg2);

        CHECK(heap.size()==2);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(FiveEdgesTreeOneKWQueryTSPTest) {
        cout << "\nResults for FiveEdgesTreeOneKWQueryTSPTest: \n";
        SmallGraph g;
        g.add_edge(1,2).add_edge(2,4).add_edge(1,6).add_edge(1,3).add_edge(3,5);
        vector<uint32_t> l1 = {1};
        vector<uint32_t> l2 = {2};
        vector<uint32_t> l3 = {3};
        vector<uint32_t> l4 = {2,1};
        g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4).set_label(5,l3).set_label(6,l2);
        DataGraph dg(g);
        Activation a(dg, 0.5, average_shortest_distance(dg));
        vector<uint32_t> k = {3};
        vector<KWSGraph> heap = get_heap(dg, k, 0.5, 5, 4);

        SmallGraph sg1;
        sg1.add_node(3).set_label(3,l3);
        KWSGraph kg1(sg1,a.get_weights(),0,3);

        SmallGraph sg2;
        sg2.add_node(5).set_label(5,l3);
        KWSGraph kg2(sg2,a.get_weights(),0,5);

        CHECK(heap[0].get_v_list().size() == 1);
        CHECK(heap[1].get_v_list().size() == 1);
        CHECK(heap[0].get_graph().label(heap[0].get_v_list()[0])[0]==3);
        CHECK(heap[1].get_graph().label(heap[1].get_v_list()[0])[0]==3);
        CHECK(heap[0].get_v_list()[0]==5);
        CHECK(heap[1].get_v_list()[0]==3);
        CHECK(heap.size()==2);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(FiveEdgesTreeThreeKWQueryTSPTest) {
        cout << "\nResults for FiveEdgesTreeThreeKWQueryTSPTest: \n";
        SmallGraph g;
        g.add_edge(1,2).add_edge(2,4).add_edge(1,6).add_edge(1,3).add_edge(3,5);
        vector<uint32_t> l1 = {1};
        vector<uint32_t> l2 = {2};
        vector<uint32_t> l3 = {3};
        vector<uint32_t> l4 = {2,1};
        g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4).set_label(5,l3).set_label(6,l2);
        DataGraph dg(g);
        Activation a(dg, 0.5, average_shortest_distance(dg));
        vector<uint32_t> k = {1,2,3};
        vector<KWSGraph> heap = get_heap(dg, k, 0.5, 5, 4);

        SmallGraph sg1;
        sg1.add_edge(1,6).add_edge(1,3);
        sg1.set_label(1,l1).set_label(3,l3).set_label(6,l2);
        KWSGraph kg1(sg1,a.get_weights(),3,1);
        CHECK(heap[0]==kg1);
        CHECK(heap.size()==1);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(SixEdgesFourKWQueryTSPTest) {
        cout << "\nResults for SixEdgesFourKWQueryTSPTest: \n";
        SmallGraph g;
        g.add_edge(1,2).add_edge(2,3).add_edge(3,4).add_edge(1,4).add_edge(1,3).add_edge(2,4);
        vector<uint32_t> l1 = {1};
        vector<uint32_t> l2 = {2};
        vector<uint32_t> l3 = {3};
        vector<uint32_t> l4 = {4};
        g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4);
        DataGraph dg(g);
        vector<uint32_t> k = {1,2,3,4};
        vector<KWSGraph> heap = get_heap(dg, k, 0.5, 5, 6);
        CHECK(heap.size()==4);

        SmallGraph sg1;
        sg1.add_edge(1,2).add_edge(2,3).add_edge(3,4).add_edge(1,4).add_edge(1,3).add_edge(2,4);
        sg1.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4);
        CHECK(heap[0].get_graph()==sg1);
        CHECK(heap[1].get_graph()==sg1);
        CHECK(heap[2].get_graph()==sg1);
        CHECK(heap[3].get_graph()==sg1);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(SquareAllKWQueryTSPTest) {
        cout << "\nResults for SquareAllKWQueryTSPTest: \n";
        SmallGraph g;
        g.add_edge(1,2).add_edge(2,3).add_edge(3,4).add_edge(1,4);
        vector<uint32_t> l1 = {1};
        vector<uint32_t> l2 = {2};
        vector<uint32_t> l3 = {3};
        vector<uint32_t> l4 = {4};
        g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4);
        DataGraph dg(g);
        vector<uint32_t> k = {1,2,3,4};
        vector<KWSGraph> heap = get_heap(dg, k, 0.5, 5, 6);
        CHECK(heap.size()==4);

        SmallGraph sg1;
        sg1.add_edge(1,2).add_edge(2,3).add_edge(3,4).add_edge(1,4);
        sg1.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4);
        CHECK(heap[0].get_graph()==sg1);
        CHECK(heap[1].get_graph()==sg1);
        CHECK(heap[2].get_graph()==sg1);
        CHECK(heap[3].get_graph()==sg1);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(FourEdgeLineAllKWQueryTSPTest) {
        cout << "\nResults for FourEdgeLineAllKWQueryTSPTest: \n";
        SmallGraph g;
        g.add_edge(1,2).add_edge(2,3).add_edge(3,4).add_edge(4,5);
        vector<uint32_t> l1 = {1};
        vector<uint32_t> l2 = {2};
        vector<uint32_t> l3 = {3};
        vector<uint32_t> l4 = {4};
        vector<uint32_t> l5 = {5};
        g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4).set_label(5,l5);
        DataGraph dg(g);
        vector<uint32_t> k = {1,2,3,4,5};
        vector<KWSGraph> heap = get_heap(dg, k, 0.5, 5, 6);
        
        SmallGraph g2;
        g2.add_edge(1,2).add_edge(2,3).add_edge(3,4).add_edge(4,5);
        g2.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4).set_label(5,l5);
        CHECK(heap[0].get_graph()==g2);
        CHECK(heap.size()==1);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(FourEdgeLineTwoKWQueryTSPTest) {
        cout << "\nResults for FourEdgeLineTwoKWQueryTSPTest: \n";
        SmallGraph g;
        g.add_edge(1,2).add_edge(2,3).add_edge(3,4).add_edge(4,5);
        vector<uint32_t> l1 = {1};
        vector<uint32_t> l2 = {2};
        vector<uint32_t> l3 = {3};
        vector<uint32_t> l4 = {4};
        vector<uint32_t> l5 = {5};
        g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4).set_label(5,l5);
        DataGraph dg(g);
        Activation a(dg, 0.5, average_shortest_distance(dg));
        vector<uint32_t> k = {2,3};
        vector<KWSGraph> heap = get_heap(dg, k, 0.5, 5, 6);

        SmallGraph sg1;
        sg1.add_edge(2,3).set_label(2,l2).set_label(3,l3);
        SmallGraph sg2;
        sg2.add_edge(2,3).set_label(2,l2).set_label(3,l3);
        KWSGraph kg1(sg1,a.get_weights(),5,3);
        KWSGraph kg2(sg2,a.get_weights(),5,2);

        CHECK(heap[0]==kg1);
        CHECK(heap[1]==kg2);
        CHECK(heap.size()==2);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(SevenEdgesFourKWQueryTSPTest) {
        cout << "\nResults for SevenEdgesFourKWQueryTSPTest: \n";
        SmallGraph g;
        g.add_edge(1,2).add_edge(1,3).add_edge(1,4).add_edge(2,3).add_edge(2,5).add_edge(2,6).add_edge(3,7);
        vector<uint32_t> l1 = {1};
        vector<uint32_t> l2 = {2};
        vector<uint32_t> l3 = {3};
        vector<uint32_t> l4 = {4};
        vector<uint32_t> l5 = {5};
        vector<uint32_t> l6 = {6};
        vector<uint32_t> l7 = {7};
        g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4).set_label(5,l5).set_label(6,l6).set_label(7,l7);
        DataGraph dg(g);
        Activation a(dg, 0.5, average_shortest_distance(dg));
        vector<uint32_t> k1 = {1,2,3,7};
        vector<uint32_t> k2 = {7,3,2,1};
        vector<KWSGraph> heap = get_heap(dg,k1, 0.5, 5, 4);
        vector<KWSGraph> heap2= get_heap(dg,k2, 0.5, 5, 4);
        CHECK(heap[0]==heap2[0]);
        CHECK(heap.size()==1 && heap2.size()==1);
        SmallGraph g2;
        g2.add_edge(1,2).add_edge(2,3).add_edge(1,3).add_edge(3,7);
        g2.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(7,l7);
        KWSGraph kg1(g2,a.get_weights(),4,2);
        CHECK(kg1==heap[0]);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(SixEdgesFourKWQueryMultLabelsTSPTest) {
        cout << "\nResults for SixEdgesFourKWQueryMultLabelsTSPTest: \n";
        SmallGraph g;
        g.add_edge(1,2).add_edge(1,3).add_edge(1,4).add_edge(1,5).add_edge(5,6).add_edge(3,6);
        vector<uint32_t> l1 = {12};
        vector<uint32_t> l2 = {31,13};
        vector<uint32_t> l3 = {24};
        vector<uint32_t> l4 = {35};
        vector<uint32_t> l5 = {57};
        vector<uint32_t> l6 = {31,6,9};
        g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4).set_label(5,l5).set_label(6,l6);
        DataGraph dg(g);
        Activation a(dg, 0.5, average_shortest_distance(dg));
        vector<uint32_t> k = {12,31,6,9};
        vector<KWSGraph> heap = get_heap(dg,k, 0.5, 5, 4);
        SmallGraph g2;
        g2.add_edge(1,3).add_edge(1,5).add_edge(3,6).add_edge(5,6);
        g2.set_label(1,l1).set_label(3,l3).set_label(5,l5).set_label(6,l6);
        KWSGraph kg(g2, a.get_weights(), 4, 1);
        CHECK(kg==heap[0]);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(SixEdgesFourKWQueryMultLabelsNoResultsTSPTest) {
        cout << "\nResults for SixEdgesFourKWQueryMultLabelsNoResultsTSPTest: \n";
        SmallGraph g;
        g.add_edge(1,2).add_edge(1,3).add_edge(1,4).add_edge(1,5).add_edge(5,6).add_edge(3,6);
        vector<uint32_t> l1 = {12};
        vector<uint32_t> l2 = {31,13};
        vector<uint32_t> l3 = {24};
        vector<uint32_t> l4 = {35};
        vector<uint32_t> l5 = {57};
        vector<uint32_t> l6 = {99, 66, 0, 4};
        g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4).set_label(5,l5).set_label(6,l6);
        DataGraph dg(g);
        vector<uint32_t> k = {12,31,6,9};
        vector<KWSGraph> heap = get_heap(dg,k, 0.5, 5, 10);
        CHECK(heap.size()==0);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(FourteenEdgesCitrusTwoKWQueryTSPTest) { //Called citrus because shape is like the top of a citrus fruit cut in half
        cout << "\nResults for FourteenEdgesCitrusTwoKWQueryTSPTest: \n";
        SmallGraph g;
        g.add_edge(1,2).add_edge(1,3).add_edge(1,4).add_edge(1,5).add_edge(1,6).add_edge(1,7).add_edge(1,8);
        g.add_edge(2,3).add_edge(3,4).add_edge(4,5).add_edge(5,6).add_edge(6,7).add_edge(7,8).add_edge(8,2);
        vector<uint32_t> l1={1};
        vector<uint32_t> l2={2};
        vector<uint32_t> l3={3};
        vector<uint32_t> l4={4};
        vector<uint32_t> l5={5};
        vector<uint32_t> l6={6};
        vector<uint32_t> l7={7};
        vector<uint32_t> l8={8,22};
        g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4).set_label(5,l5).set_label(6,l6).set_label(7,l7).set_label(8,l8);
        DataGraph dg(g);
        Activation a(dg, 0.5, average_shortest_distance(dg));
        vector<uint32_t> k = {1,3};
        vector<KWSGraph> heap = get_heap(dg,k,0.2,5,2);

        SmallGraph g2;
        g2.add_edge(1,3).set_label(1,l1).set_label(3,l3);
        KWSGraph kg1(g2,a.get_weights(),1,1);

        CHECK(heap[0]==kg1);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(SevenEdgesUrchinTwoKWQueryTSPTest) { //Called urchin because shape is like a sea urchin
        cout << "\nResults for SevenEdgesUrchinTwoKWQueryTSPTest: \n";
        SmallGraph g;
        g.add_edge(1,2).add_edge(1,3).add_edge(1,4).add_edge(1,5).add_edge(1,6).add_edge(1,7).add_edge(1,8);
        vector<uint32_t> l1={1};
        vector<uint32_t> l2={2};
        vector<uint32_t> l3={3};
        vector<uint32_t> l4={4};
        vector<uint32_t> l5={5};
        vector<uint32_t> l6={6};
        vector<uint32_t> l7={7};
        vector<uint32_t> l8={8,22};
        g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4).set_label(5,l5).set_label(6,l6).set_label(7,l7).set_label(8,l8);
        DataGraph dg(g);
        Activation a(dg, 0.5, average_shortest_distance(dg));
        vector<uint32_t> k = {1,3};
        vector<KWSGraph> heap = get_heap(dg,k,0.2,5,2);

        SmallGraph g2;
        g2.add_edge(1,3).set_label(1,l1).set_label(3,l3);
        KWSGraph kg1(g2,a.get_weights(),1,1);

        CHECK(heap[0]==kg1);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(FourteenEdgesCitrusThreeKWQueryTSPTest) {
        cout << "\nResults for FourteenEdgesCitrusThreeKWQueryTSPTest: \n";
        SmallGraph g;
        g.add_edge(1,2).add_edge(1,3).add_edge(1,4).add_edge(1,5).add_edge(1,6).add_edge(1,7).add_edge(1,8);
        g.add_edge(2,3).add_edge(3,4).add_edge(4,5).add_edge(5,6).add_edge(6,7).add_edge(7,8).add_edge(8,2);
        vector<uint32_t> l1={1,9};
        vector<uint32_t> l2={2};
        vector<uint32_t> l3={3};
        vector<uint32_t> l4={4};
        vector<uint32_t> l5={5};
        vector<uint32_t> l6={6};
        vector<uint32_t> l7={7};
        vector<uint32_t> l8={8};
        g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4).set_label(5,l5).set_label(6,l6).set_label(7,l7).set_label(8,l8);
        DataGraph dg(g);
        Activation a(dg, 0.5, average_shortest_distance(dg));
        vector<uint32_t> k = {1,2,3};
        vector<KWSGraph> heap = get_heap(dg,k,0.2,5,2);

        SmallGraph g2;
        g2.add_edge(1,2).add_edge(1,3).add_edge(2,3).set_label(1,l1).set_label(2,l2).set_label(3,l3);
        KWSGraph kg1(g2,a.get_weights(),1,1);

        CHECK(heap[0]==kg1);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(SevenEdgesUrchinThreeKWQueryTSPTest) {
        cout << "\nResults for SevenEdgesUrchinThreeKWQueryTSPTest: \n";
        SmallGraph g;
        g.add_edge(1,2).add_edge(1,3).add_edge(1,4).add_edge(1,5).add_edge(1,6).add_edge(1,7).add_edge(1,8);
        vector<uint32_t> l1={1,9};
        vector<uint32_t> l2={2};
        vector<uint32_t> l3={3};
        vector<uint32_t> l4={4};
        vector<uint32_t> l5={5};
        vector<uint32_t> l6={6};
        vector<uint32_t> l7={7};
        vector<uint32_t> l8={8};
        g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4).set_label(5,l5).set_label(6,l6).set_label(7,l7).set_label(8,l8);
        DataGraph dg(g);
        Activation a(dg, 0.5, average_shortest_distance(dg));
        vector<uint32_t> k = {1,2,3};
        vector<KWSGraph> heap = get_heap(dg,k,0.2,5,2);

        SmallGraph g2;
        g2.add_edge(1,2).add_edge(1,3).set_label(1,l1).set_label(2,l2).set_label(3,l3);
        KWSGraph kg1(g2,a.get_weights(),1,1);

        CHECK(heap[0]==kg1);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(CircleWithRadiusThreeKWQueryTSPTest) {
        cout << "\nResults for CircleWithRadiusThreeKWQueryTSPTest: \n";
        SmallGraph g;
        g.add_edge(1,2);
        g.add_edge(2,3).add_edge(3,4).add_edge(4,5).add_edge(5,6).add_edge(6,7).add_edge(7,8).add_edge(8,2);
        vector<uint32_t> l1={1,9};
        vector<uint32_t> l2={2};
        vector<uint32_t> l3={3};
        vector<uint32_t> l4={4};
        vector<uint32_t> l5={5};
        vector<uint32_t> l6={6};
        vector<uint32_t> l7={7};
        vector<uint32_t> l8={8};
        g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4).set_label(5,l5).set_label(6,l6).set_label(7,l7).set_label(8,l8);
        DataGraph dg(g);
        Activation a(dg, 0.5, average_shortest_distance(dg));
        vector<uint32_t> k = {1,2,3};
        vector<KWSGraph> heap = get_heap(dg,k,0.2,5,4);

        SmallGraph g2;
        g2.add_edge(1,2).add_edge(2,3).set_label(1,l1).set_label(2,l2).set_label(3,l3);
        KWSGraph kg1(g2,a.get_weights(),4,2);

        CHECK(heap[0]==kg1);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(EightNodesCompleteGraphThreeKWQueryTSPTest) {
        cout << "\nResults for EightNodesCompleteGraphThreeKWQueryTSPTest: \n";
        SmallGraph g;
        g.add_edge(1,2).add_edge(1,3).add_edge(1,4).add_edge(1,5).add_edge(1,6).add_edge(1,7).add_edge(1,8);
        g.add_edge(2,3).add_edge(2,4).add_edge(2,5).add_edge(2,6).add_edge(2,7).add_edge(2,8);
        g.add_edge(3,4).add_edge(3,5).add_edge(3,6).add_edge(3,7).add_edge(3,8);
        g.add_edge(4,5).add_edge(4,6).add_edge(4,7).add_edge(4,8);
        g.add_edge(5,6).add_edge(5,7).add_edge(5,8);
        g.add_edge(6,7).add_edge(6,8);
        g.add_edge(7,8);
        vector<uint32_t> l1={1,9};
        vector<uint32_t> l2={2};
        vector<uint32_t> l3={3};
        vector<uint32_t> l4={4};
        vector<uint32_t> l5={5};
        vector<uint32_t> l6={6};
        vector<uint32_t> l7={7};
        vector<uint32_t> l8={8};
        g.set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4).set_label(5,l5).set_label(6,l6).set_label(7,l7).set_label(8,l8);
        DataGraph dg(g);
        Activation a(dg, 0.5, average_shortest_distance(dg));
        vector<uint32_t> k = {1,2,3};
        vector<KWSGraph> heap = get_heap(dg,k,0.9,8,3);

        SmallGraph g2;
        g2.add_edge(1,8).add_edge(2,8).add_edge(3,8).add_edge(1,2).add_edge(1,3).add_edge(2,3).set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(8,l8);
        KWSGraph kg1(g2,a.get_weights(),3,8);

        SmallGraph g3;
        g3.add_edge(1,2).add_edge(2,3).add_edge(1,3).set_label(1,l1).set_label(2,l2).set_label(3,l3);
        KWSGraph kg2(g3,a.get_weights(),3,1);

        CHECK(heap[0]==kg1);
        CHECK(heap[7]==kg2);
        CHECK(heap.size()==8);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(SingleNodeOneKWQueryTSPTest) {
        cout << "\nResults for SingleNodeOneKWQueryTSPTest: \n";
        SmallGraph g;
        vector<uint32_t> l1 = {1,2,3,4};
        g.add_node(1).set_label(1,l1);

        DataGraph dg(g);
        Activation a(dg, 0.5, average_shortest_distance(dg));

        vector<uint32_t> k = {1};
        vector<KWSGraph> heap = get_heap(dg,k,0.2,5,2);

        SmallGraph g2;
        g2.add_node(1).set_label(1,l1);
        KWSGraph kg1(g2,a.get_weights(),0,1);
        CHECK(heap[0]==kg1);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(ThreeSingleNodesTSPTest) {
        cout << "\nResults for ThreeSingleNodesTSPTest: (1)\n";
        SmallGraph g;
        vector<uint32_t> l1 = {1,7};
        vector<uint32_t> l2 = {2};
        vector<uint32_t> l3 = {3,7};
        g.add_node(1).set_label(1,l1).add_node(2).set_label(2,l2).add_node(3).set_label(3,l3);
        DataGraph dg(g);
        Activation a(dg, 0.5, average_shortest_distance(dg));
        vector<uint32_t> k = {2};
        vector<KWSGraph> heap = get_heap(dg,k,0.2,5,2);

        SmallGraph g2;
        g2.add_node(2).set_label(2,l2);
        KWSGraph kg1(g2,a.get_weights(),0,2);
        CHECK(heap[0]==kg1);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);

        cout << "\nResults for ThreeSingleNodesTSPTest: (2)\n";
        vector<uint32_t> k2 = {7};
        vector<KWSGraph> heap2 = get_heap(dg,k2,0.2,5,2);
        SmallGraph g3;
        g3.add_node(3).set_label(3,l3);
        KWSGraph kg2(g3,a.get_weights(),0,3);
        SmallGraph g4;
        g4.add_node(1).set_label(1,l1);
        KWSGraph kg3(g4,a.get_weights(),0,1);
        CHECK(heap2[0]==kg2);
        CHECK(heap2[1]==kg3);
        CHECK(heap2.size()==2);
        CHECK(heap_score_is_desc(heap2));
        print_heap(heap2);

        cout << "\nResults for ThreeSingleNodesTSPTest: (3)\n";
        vector<uint32_t> k3 = {1,3};
        vector<KWSGraph> heap3 = get_heap(dg,k3,0.2,5,2);
        CHECK(heap3.size()==0);
        CHECK(heap_score_is_desc(heap2));
        print_heap(heap3);
    }

    TEST(ThreeSingleNodesTwoDisconnectedEdgesTSPTest) {
        cout << "\nResults for ThreeSingleNodesTwoDisconnectedEdgesTSPTest: \n";
        SmallGraph g;
        vector<uint32_t> l1 = {1};
        vector<uint32_t> l2 = {2,6,9};
        vector<uint32_t> l3 = {3};
        g.add_node(1).set_label(1,l1).add_node(2).set_label(2,l2).add_node(3).add_edge(4,5).add_edge(6,7).set_label(4,l3).set_label(5,l3).set_label(6,l3);
        DataGraph dg(g);
        Activation a(dg, 0.5, average_shortest_distance(dg));
        vector<uint32_t> k = {2,9};
        vector<KWSGraph> heap = get_heap(dg,k,0.2,5,2);

        SmallGraph g2;
        g2.add_node(2).set_label(2,l2);
        KWSGraph kg1(g2,a.get_weights(),0,2);
        CHECK(heap[0]==kg1);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(TwoDisconnectedEdgesOneGraphTSPTest) {
        cout << "\nResults for TwoDisconnectedEdgesOneGraphTSPTest: \n";
        SmallGraph g;
        vector<uint32_t> l1 = {1};
        vector<uint32_t> l2 = {2};
        vector<uint32_t> l3 = {3};
        vector<uint32_t> l4 = {4};
        g.add_edge(1,2).set_label(1,l1).set_label(2,l2).add_edge(3,4).set_label(3,l3).set_label(4,l4).add_edge(5,6).add_edge(5,7).add_edge(5,8).add_edge(6,7);
        DataGraph dg(g);
        Activation a(dg, 0.5, average_shortest_distance(dg));
        vector<uint32_t> k = {1,2};
        vector<KWSGraph> heap = get_heap(dg,k,0.2,5,3);

        SmallGraph g2;
        g2.add_edge(1,2).set_label(1,l1).set_label(2,l2);
        KWSGraph kg1(g2,a.get_weights(),1,2);
        KWSGraph kg2(g2,a.get_weights(),1,1);
        CHECK(heap[0]==kg1);
        CHECK(heap[1]==kg2);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(ThreeSingleNodesOneDisconnectedEdgeOneKWQueryTSPTest) {
        cout << "\nResults for ThreeSingleNodesOneDisconnectedEdgeOneKWQueryTSPTest: \n";
        SmallGraph g;
        vector<uint32_t> l1 = {1,2,3,4};
        g.add_node(1).set_label(1,l1).add_node(2).add_node(3).add_edge(4,5);

        DataGraph dg(g);
        Activation a(dg, 0.5, average_shortest_distance(dg));

        vector<uint32_t> k = {1};
        vector<KWSGraph> heap = get_heap(dg,k,0.2,5,2,false);

        SmallGraph g2;
        g2.add_node(1).set_label(1,l1);
        KWSGraph kg1(g2,a.get_weights(),0,1);
        CHECK(heap[0]==kg1);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(ThreeSingleNodesOneDisconnectedEdgeNoResultsTSPTest) {
        cout << "\nResults for ThreeSingleNodesOneDisconnectedEdgeNoResultsTSPTest: \n";
        SmallGraph g;
        vector<uint32_t> l1 = {1,2,3,4};
        g.add_node(1).set_label(1,l1).add_node(2).add_node(3).add_edge(4,5);

        DataGraph dg(g);

        vector<uint32_t> k = {8,7,6,5,10};
        vector<KWSGraph> heap = get_heap(dg,k,0.2,5,2,false);

        CHECK(heap.size()==0);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(SquareAllKeywordsQueryTSPTest) {
        cout << "\nResults for SquareAllKeywordsQueryTSPTest: \n";
        SmallGraph g;
        vector<uint32_t> l1 = {1};
        vector<uint32_t> l2 = {2};
        vector<uint32_t> l3 = {3};
        vector<uint32_t> l4 = {4};
        g.add_edge(1,2).add_edge(2,3).add_edge(3,4).add_edge(4,1).set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4);

        DataGraph dg(g);
        Activation a(dg, 0.5, average_shortest_distance(dg));

        vector<uint32_t> k = {1,2,3,4};
        vector<KWSGraph> heap = get_heap(dg,k,0.9,10,5,false);

        SmallGraph g2;
        g2.add_edge(1,4).add_edge(3,4).add_edge(2,1).set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4);
        KWSGraph kg1(g2,a.get_weights(),5,4);

        CHECK(heap[0]==kg1);
        uint32_t counter = 4;
        for(auto h: heap) {
            CHECK(h.get_num_vertices()==4);
            CHECK(h.get_num_edges()==3);
            CHECK(h.get_root()==counter);
            CHECK((round( h.get_score() * 100000.0 ) / 100000.0) == 5.51892);
            counter--;
        }
        CHECK(heap.size()==4);
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(FourEdgeLineAllKeywordsQueryTSPTest) {
        cout << "\nResults for FourEdgeLineAllKeywordsQueryTSPTest: \n";
        SmallGraph g;
        vector<uint32_t> l1 = {1};
        vector<uint32_t> l2 = {2};
        vector<uint32_t> l3 = {3};
        vector<uint32_t> l4 = {4};
        g.add_edge(1,2).add_edge(2,3).add_edge(3,4).set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4);

        DataGraph dg(g);
        Activation a(dg, 0.5, average_shortest_distance(dg));

        vector<uint32_t> k = {1,2,3,4};
        vector<KWSGraph> heap = get_heap(dg,k,0.9,10,5,false);

        SmallGraph g2;
        g2.add_edge(1,2).add_edge(2,3).add_edge(3,4).set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4);
        KWSGraph kg1(g2,a.get_weights(),4,3);
        KWSGraph kg2(g2,a.get_weights(),4,2);

        CHECK(heap[0]==kg1);
        CHECK(heap[1]==kg2);
        CHECK(heap.size()==2);

        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(FourEdgeLineTwoKeywordsMiddleQueryTSPTest) {
        cout << "\nResults for FourEdgeLineTwoKeywordsMiddleQueryTSPTest: \n";
        SmallGraph g;
        vector<uint32_t> l1 = {1};
        vector<uint32_t> l2 = {2};
        vector<uint32_t> l3 = {3};
        vector<uint32_t> l4 = {4};
        g.add_edge(1,2).add_edge(2,3).add_edge(3,4).set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4);

        DataGraph dg(g);
        Activation a(dg, 0.5, average_shortest_distance(dg));

        vector<uint32_t> k = {2,3};
        vector<KWSGraph> heap = get_heap(dg,k,0.9,10,5,false);

        SmallGraph g2;
        g2.add_edge(2,3).set_label(2,l2).set_label(3,l3);
        KWSGraph kg1(g2,a.get_weights(),4,3);
        KWSGraph kg2(g2,a.get_weights(),4,2);

        CHECK(heap[0]==kg1);
        CHECK(heap[1]==kg2);
        CHECK(heap.size()==2);

        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(FourEdgeLineTwoKeywordsEndpointQueryTSPTest) {
        cout << "\nResults for FourEdgeLineTwoKeywordsEndpointQueryTSPTest: \n";
        SmallGraph g;
        vector<uint32_t> l1 = {1};
        vector<uint32_t> l2 = {2};
        vector<uint32_t> l3 = {3};
        vector<uint32_t> l4 = {4};
        g.add_edge(1,2).add_edge(2,3).add_edge(3,4).set_label(1,l1).set_label(2,l2).set_label(3,l3).set_label(4,l4);

        DataGraph dg(g);
        Activation a(dg, 0.5, average_shortest_distance(dg));

        vector<uint32_t> k = {3,4};
        vector<KWSGraph> heap = get_heap(dg,k,0.9,10,5,false);

        SmallGraph g2;
        g2.add_edge(3,4).set_label(3,l3).set_label(4,l4);
        KWSGraph kg1(g2,a.get_weights(),1,3);

        CHECK(heap[0]==kg1);
        CHECK(heap.size()==1);

        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(FiveSingleNodesOneKWQueryTSPTest) {
        cout << "\nResults for FiveSingleNodesOneKWQueryTSPTest: \n";
        SmallGraph g;
        vector<uint32_t> l1 = {1};
        g.add_node(1).add_node(2).add_node(3).add_node(4).add_node(5);
        g.set_label(1,l1).set_label(2,l1).set_label(3,l1).set_label(4,l1).set_label(5,l1);

        DataGraph dg(g);

        vector<uint32_t> k = {1};
        vector<KWSGraph> heap = get_heap(dg,k,0.9,3,5,true);

        CHECK(heap.size()==3);
        CHECK(heap[0].get_root()==5);
        CHECK(heap[1].get_root()==4);
        CHECK(heap[2].get_root()==3);
        CHECK(heap[0].get_score()==0);
        CHECK(heap[1].get_score()==0);
        CHECK(heap[2].get_score()==0);
        
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    TEST(FiveSingleNodesOneEdgeOneGraphTwoKWQueryTSPTest) {
        cout << "\nResults for FiveSingleNodesOneEdgeOneGraphTwoKWQueryTSPTest: \n";
        SmallGraph g;
        vector<uint32_t> l1 = {2};
        vector<uint32_t> l2 = {2};
        vector<uint32_t> l3 = {3};
        g.add_node(1).add_node(2).add_node(3).add_node(4).add_node(5).add_edge(6,7).add_edge(8,9).add_edge(10,9);
        g.set_label(1,l1).set_label(2,l1).set_label(3,l1).set_label(4,l1).set_label(5,l1).set_label(6,l2).set_label(7,l3).set_label(8,l2).set_label(9,l3).set_label(10,l2);

        DataGraph dg(g);

        vector<uint32_t> k = {2,3};
        vector<KWSGraph> heap = get_heap(dg,k,0.9,4,5,true);

        CHECK(heap.size()==3);
        CHECK(heap[0].get_root()==9);
        CHECK(heap[1].get_root()==7);
        CHECK(heap[2].get_root()==6);
        CHECK((round( heap[0].get_score() * 100000.0 ) / 100000.0) == 2.26186);
        CHECK((round( heap[1].get_score() * 100000.0 ) / 100000.0) == 1.26186);
        CHECK((round( heap[2].get_score() * 100000.0 ) / 100000.0) == 1.26186);
        
        CHECK(heap_score_is_desc(heap));
        print_heap(heap);
    }

    // TODO: Need more tests where DataGraph is generated from a SmallGraph that was imported from a textfile
    TEST(ImportSmallGraphFromTextfile) {
        cout << "\nResults for ImportSmallGraphFromTextfile: \n";
        SmallGraph g("kws_algorithms/unittests/Textfiles/test.txt");
        DataGraph dg(g); //If you generate a SmallGraph from a text file and export it to a DataGraph, the text file must be a fully labelled graph 
        vector<uint32_t> k = {100,101};
        vector<KWSGraph> heap = get_heap(dg,k,0.5,5,4,true);

        SmallGraph g2;
        g2.add_edge(1,2).set_label(1,100).set_label(2,101);
        CHECK(heap[0].get_graph()==g);
        CHECK(heap[1].get_graph()==g2);
        CHECK(heap[2].get_graph()==g2);
        CHECK(heap[0].get_root()==3);
        CHECK(heap[1].get_root()==2);
        CHECK(heap[2].get_root()==1);
        CHECK(heap.size()==3);
        print_heap(heap);
    }

    // TODO: Need more tests where DataGraph is imported from a bin file
    TEST(ImportDataGraphFromBinFile) {
        cout << "\nResults for ImportDataGraphFromBinFile: \n";
        DataGraph dg("data/my-graph"); //bin file should not be partially labelled
        vector<uint32_t> k = {100,200,300,400};
        vector<KWSGraph> heap = get_heap(dg,k,0.5,5,5,true);

        SmallGraph g2;
        g2.add_edge(1,2).add_edge(2,3).add_edge(3,4).add_edge(4,1).set_label(1,100).set_label(2,200).set_label(3,300).set_label(4,400);
        CHECK(heap[0].get_graph()==g2);
        CHECK(heap[1].get_graph()==g2);
        CHECK(heap[2].get_graph()==g2);
        CHECK(heap[3].get_graph()==g2);
        CHECK(heap[0].get_root()==4);
        CHECK(heap[1].get_root()==3);
        CHECK(heap[2].get_root()==2);
        CHECK(heap[3].get_root()==1);
        CHECK(heap.size()==4);
        print_heap(heap);
    }

    // TODO: Need more tests on unlabelled graphs and partially labelled graphs
    TEST(UnlabelledGraphTest) {
        cout << "\nResults for UnlabelledGraphTest: \n";
        SmallGraph g;
        g.add_edge(1,2).add_edge(3,4);
        vector<uint32_t> k = {100,200,300,400};
        DataGraph dg(g);
        try {
            vector<KWSGraph> heap = get_heap(dg,k,0.5,5,5,true);
        }
        catch(const invalid_argument& ia) {
            CHECK(string(ia.what()) == "Graph is not labelled\n");
            cout << "No labels found\n";
        }
    }

}