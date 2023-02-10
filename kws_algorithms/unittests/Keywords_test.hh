#include <iostream>

SUITE(KeywordsTests) {
    TEST(OneKeywordQuery) {
        vector<uint32_t> query = {1};
        Keywords keywords(query);
        CHECK_EQUAL(keywords.get_length(),1);
        vector<uint32_t> l = {1};
        CHECK(keywords.contains_keyword(l));
        l = {0};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {2};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {3};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {4};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {5};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {6};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {7};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {8};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {9};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
    }

    TEST(TwoKeywordsQuery) {
        vector<uint32_t> query = {1,2};
        Keywords keywords(query);
        CHECK_EQUAL(keywords.get_keywords()[0],1);
        CHECK_EQUAL(keywords.get_keywords()[1],2);
        CHECK_EQUAL(keywords.get_length(),2);
        vector<uint32_t> l = {1};
        CHECK(keywords.contains_keyword(l));
        l = {2};
        CHECK(keywords.contains_keyword(l));
        l = {0};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {3};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {4};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {5};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {6};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {7};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {8};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {9};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
    }

    TEST(ThreeKeywordsQuery) {
        vector<uint32_t> query = {1,2,3};
        Keywords keywords(query);
        CHECK_EQUAL(keywords.get_keywords()[0],1);
        CHECK_EQUAL(keywords.get_keywords()[1],2);
        CHECK_EQUAL(keywords.get_keywords()[2],3);
        CHECK_EQUAL(keywords.get_length(),3);
        vector<uint32_t> l = {1};
        CHECK(keywords.contains_keyword(l));
        l = {2};
        CHECK(keywords.contains_keyword(l));
        l = {3};
        CHECK(keywords.contains_keyword(l));
        l = {0};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {4};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {5};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {6};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {7};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {8};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {9};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {1,2,3};
        CHECK_EQUAL(keywords.contains_keyword(l),true);
        l = {3,2,1};
        CHECK_EQUAL(keywords.contains_keyword(l),true);
        l = {3,6,6};
        CHECK_EQUAL(keywords.contains_keyword(l),true);
        l = {9,6,7};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
    }

    TEST(SixNineOneKeywordsQuery) {
        vector<uint32_t> query = {6,9,1};
        Keywords keywords(query);
        CHECK_EQUAL(keywords.get_keywords()[0],6);
        CHECK_EQUAL(keywords.get_keywords()[1],9);
        CHECK_EQUAL(keywords.get_keywords()[2],1);
        CHECK_EQUAL(keywords.get_length(),3);
        vector<uint32_t> l = {6};
        CHECK(keywords.contains_keyword(l));
        l = {9};
        CHECK(keywords.contains_keyword(l));
        l = {1};
        CHECK(keywords.contains_keyword(l));
        l = {0};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {2};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {3};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {4};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {5};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {7};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {8};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
        l = {6,9,1};
        CHECK_EQUAL(keywords.contains_keyword(l),true);
        l = {1,6,9};
        CHECK_EQUAL(keywords.contains_keyword(l),true);
        l = {3,8,1};
        CHECK_EQUAL(keywords.contains_keyword(l),true);
        l = {8,7,0};
        CHECK_EQUAL(keywords.contains_keyword(l),false);
    }
}