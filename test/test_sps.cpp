#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestSPS
#include <boost/test/unit_test.hpp>

#include <chrono>
#include <iostream>
#include <random>
#include <vector>
#include "../src/probtree.h"
#include "../src/file_util.h"

BOOST_AUTO_TEST_SUITE(SuiteSPS)

BOOST_AUTO_TEST_CASE(TestSingleNode){
    const int v = 1;
    std::vector<int> seq = std::vector<int>();
    seq.push_back(v);
    auto pt = ProbTree<int>(seq);
    auto& root = pt.__root__;
    BOOST_CHECK(root.__count__ == v);
    auto pm = root.__prob_map__;
    BOOST_CHECK(pm.find(v) != pm.end());
    auto v0 = pm[v];
    BOOST_CHECK(v0->__count__ == 0);
    BOOST_CHECK(v0->__prob_map__.size() == 0);
}

BOOST_AUTO_TEST_CASE(TestSingleBranch){
    std::vector<int> seq = std::vector<int>();
    seq.push_back(1);
    seq.push_back(2);
    auto pt = ProbTree<int>(seq);

    auto& n0 = pt.__root__;
    BOOST_CHECK(n0.__count__ == 1);
    auto pm = n0.__prob_map__;
    for(auto s: seq){
        BOOST_CHECK(pm.find(s) != pm.end());
    }
    auto n1 = pm[seq[0]];
    BOOST_CHECK(n1->__count__ == 1);
    BOOST_CHECK(n1->__prob_map__.size() == 1);

    auto n2 = n1->__prob_map__[seq[1]];
    BOOST_CHECK(n2->__count__ == 0);
    BOOST_CHECK(n2->__prob_map__.size() == 0);

    auto n3 = pm[seq[1]];
    BOOST_CHECK(n3->__count__ == 0);
    BOOST_CHECK(n3->__prob_map__.size() == 0);

}

BOOST_AUTO_TEST_CASE(TestCounts){
    std::vector<int> seq = std::vector<int>();
    seq.push_back(1);
    seq.push_back(2);
    seq.push_back(1);
    seq.push_back(1);
    seq.push_back(1);
    auto pt = ProbTree<int>(seq);


    auto& n0 = pt.__root__;
    BOOST_CHECK(n0.__count__ == 1);
    BOOST_CHECK(n0.__depth__ == 5);

    auto pm = n0.__prob_map__;
    for(auto s: seq){
        BOOST_CHECK(pm.find(s) != pm.end());
    }

    auto n1 = pm[seq[0]];
    BOOST_CHECK(n1->__count__ == 5);
    BOOST_CHECK(n1->__prob_map__.size() == 2);
    BOOST_CHECK(n1->__depth__ == 4);

    auto n2 = n1->__prob_map__[2];
    BOOST_CHECK(n2->__count__ == 1);
    BOOST_CHECK(n2->__prob_map__.size() == 1);
    BOOST_CHECK(n2->__depth__ == 3);

    auto n3 = n1->__prob_map__[1];
    BOOST_CHECK(n3->__count__ == 3);
    BOOST_CHECK(n3->__prob_map__.size() == 1);
    BOOST_CHECK(n3->__depth__ == 1);

    auto n4 = n1->__prob_map__[2];
    BOOST_CHECK(n4->__count__ == 1);
    BOOST_CHECK(n4->__prob_map__.size() == 1);
    BOOST_CHECK(n4->__depth__ == 3);

    auto n5 = n2->__prob_map__[1];
    BOOST_CHECK(n5->__count__ == 4);
    BOOST_CHECK(n5->__prob_map__.size() == 1);

    auto n6 = n3->__prob_map__[1];
    BOOST_CHECK(n6->__count__ == 0);
    BOOST_CHECK(n6->__prob_map__.size() == 0);
    BOOST_CHECK(n6->__depth__ == 0);

    auto n7 = n4->__prob_map__[1];
    BOOST_CHECK(n7->__count__ == 4);
    BOOST_CHECK(n7->__prob_map__.size() == 1);
    BOOST_CHECK(n7->__depth__ == 2);

    auto n8 = n5->__prob_map__[1];
    BOOST_CHECK(n8->__count__ == 3);
    BOOST_CHECK(n8->__prob_map__.size() == 1);
    BOOST_CHECK(n8->__depth__ == 1);

    auto n9 = n7->__prob_map__[1];
    BOOST_CHECK(n9->__count__ == 3);
    BOOST_CHECK(n9->__prob_map__.size() == 1);
    BOOST_CHECK(n9->__depth__ == 1);

    auto n10 = n8->__prob_map__[1];
    BOOST_CHECK(n10->__count__ == 0);
    BOOST_CHECK(n10->__prob_map__.size() == 0);
    BOOST_CHECK(n10->__depth__ == 0);

    auto n11 = n9->__prob_map__[1];
    BOOST_CHECK(n11->__count__ == 0);
    BOOST_CHECK(n11->__prob_map__.size() == 0);
    BOOST_CHECK(n11->__depth__ == 0);

}

BOOST_AUTO_TEST_CASE(Test_file_util){
    char* file_name = "a";
    std::vector<int> seq = std::vector<int>();
    seq.push_back(1);
    seq.push_back(2);
    seq.push_back(1);
    seq.push_back(1);
    seq.push_back(1);
    auto pt = ProbTree<int>(seq);
    write<int>(file_name, &pt);
    ProbTree<int> pt2;
    read<int>(file_name, pt2);

    auto& n0 = pt2.__root__;
    BOOST_CHECK(n0.__count__ == 1);
    auto pm = n0.__prob_map__;
    for(auto s: seq){
        BOOST_CHECK(pm.find(s) != pm.end());
    }

    auto n1 = pm[seq[0]];
    BOOST_CHECK(n1->__count__ == 5);
    BOOST_CHECK(n1->__prob_map__.size() == 2);

    auto n2 = n1->__prob_map__[2];
    BOOST_CHECK(n2->__count__ == 1);
    BOOST_CHECK(n2->__prob_map__.size() == 1);

    auto n3 = n1->__prob_map__[1];
    BOOST_CHECK(n3->__count__ == 3);
    BOOST_CHECK(n3->__prob_map__.size() == 1);

    auto n4 = n1->__prob_map__[2];
    BOOST_CHECK(n4->__count__ == 1);
    BOOST_CHECK(n4->__prob_map__.size() == 1);

    auto n5 = n2->__prob_map__[1];
    BOOST_CHECK(n5->__count__ == 4);
    BOOST_CHECK(n5->__prob_map__.size() == 1);

    auto n6 = n3->__prob_map__[1];
    BOOST_CHECK(n6->__count__ == 0);
    BOOST_CHECK(n6->__prob_map__.size() == 0);

    auto n7 = n4->__prob_map__[1];
    BOOST_CHECK(n7->__count__ == 4);
    BOOST_CHECK(n7->__prob_map__.size() == 1);

    auto n8 = n5->__prob_map__[1];
    BOOST_CHECK(n8->__count__ == 3);
    BOOST_CHECK(n8->__prob_map__.size() == 1);

    auto n9 = n7->__prob_map__[1];
    BOOST_CHECK(n9->__count__ == 3);
    BOOST_CHECK(n9->__prob_map__.size() == 1);

    auto n10 = n8->__prob_map__[1];
    BOOST_CHECK(n10->__count__ == 0);
    BOOST_CHECK(n10->__prob_map__.size() == 0);

    auto n11 = n9->__prob_map__[1];
    BOOST_CHECK(n11->__count__ == 0);
    BOOST_CHECK(n11->__prob_map__.size() == 0);

}

BOOST_AUTO_TEST_CASE(TestPrediction){
    std::vector<int> seq = std::vector<int>();
    seq.push_back(1);
    seq.push_back(2);
    seq.push_back(1);
    seq.push_back(1);
    seq.push_back(1);
    auto pt = ProbTree<int>(seq);
    std::vector<int> seq2 = std::vector<int>();
    seq2.push_back(1);
    seq2.push_back(2);
    seq2.push_back(1);
    auto pre = std::unordered_map<int, double>();
    pt.predict(seq2, pre);
    double roundingError = 0.000000000001;
    BOOST_CHECK(abs((pre[1] - (11.0/13.0))) < roundingError);
    BOOST_CHECK(abs((pre[2] - (2.0/13.0))) < roundingError);
}

BOOST_AUTO_TEST_CASE(TestBenchmark){
    std::random_device rd{};
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(1, 21);
    auto pt = ProbTree<int>();
    for (int j = 0; j < 1000 ; j++) {
        auto seq = std::vector<int>();
        rng.seed(j);
        for (int i = 0; i < 336 ; i++) {
            seq.push_back(uni(rng));
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        pt.process(seq);
        auto t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> ms = t2 - t1;
        std::cout << ms.count() << "ms and " << pt.nodes.size() << "\n";
    }

}

BOOST_AUTO_TEST_SUITE_END()
