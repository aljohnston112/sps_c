#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestSPS
#include <boost/test/unit_test.hpp>

#include <chrono>
#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include "../src/probtree.hpp"
#include "../src/file_util.h"
#include "../src/stock.h"

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
    string file_name = "a";
    std::vector<int> seq = std::vector<int>();
    seq.push_back(1);
    seq.push_back(2);
    seq.push_back(1);
    seq.push_back(1);
    seq.push_back(1);
    auto pt = ProbTree<int>(seq);
    write<int>(file_name, pt);
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
    auto pre = std::map<int, double>();
    pt.predict(seq2, pre);
    double roundingError = 0.000000000000001;
    BOOST_CHECK(abs((pre[1] - (11.0/13.0))) < roundingError);
    BOOST_CHECK(abs((pre[2] - (2.0/13.0))) < roundingError);
    seq2.push_back(3);
    seq2.push_back(1);
    seq2.push_back(2);
    seq2.push_back(1);
    pre = std::map<int, double>();
    pt.predict(seq2, pre);
}

BOOST_AUTO_TEST_CASE(TestRealData){
    ProbTree<double> pt = ProbTree<double>();
    string pt_file("./Programming/C++/sps/test/predict/pt");
    string ext = ".csv";
    if(!filesystem::exists(pt_file)){
        for(auto& p : filesystem::recursive_directory_iterator(filesystem::path("./Programming/C++/sps/test/data/"))){
            if(p.path().extension() == ext){
                std::vector<double> data{};
                get_price_data(p.path(), data);
                std::cout << p.path() << std::endl;
                pt.process(data);
            }
        }
        write(pt_file, pt);
    } else{
        read(pt_file, pt);
    }
    std::map<double, stock, std::greater<double>> all{};
    for(auto& p : filesystem::recursive_directory_iterator(filesystem::path("./Programming/C++/sps/test/predict/"))){
        if(p.path().extension() == ext){
            double max = get_max(p.path().string() + ".scale");
            std::vector<double> data{};
            get_price_data(p.path(), data);
            auto actual = data.back();
            data.pop_back();
            std::map<double, double> probs{};
            pt.predict(data, probs);
            double prediction = 0;
            for(auto& e: probs){
                prediction += e.first * e.second;
            }
            // std::string file_name; double actual; double max; double prediction;
            all[prediction*max] = stock(p.path(), actual, max, prediction);
            if(std::isnan(prediction)){
                std::cout << p.path() << " has nan" << std::endl;
            }
        }
    }
    double actual_money = 1.0;
    double expected_money = 1.0;
    double per = 1.0/10.0;
    double pos_error = 0.0;
    double neg_error = 0.0;
    double sign_error = 0;
    double error = 0.0;
    auto it = all.begin();
    for(int i = 0; i < all.size(); i++){
        auto e = (*it);
        actual_money += (per*e.second.actual*e.second.max);
        expected_money += (per*e.second.prediction*e.second.max);
        std::cout << "Money: " << actual_money << " Actual: " << e.second.actual << " Prediction: " << e.second.prediction <<std::endl;
        if(e.second.actual > 0 && e.second.prediction > 0){
            pos_error += abs(e.second.actual - e.second.prediction);
        } else if(e.second.actual < 0 && e.second.prediction < 0){
            neg_error += abs(e.second.actual - e.second.prediction);
        } else {
            error += abs(e.second.actual - e.second.prediction);
            sign_error += 1;
        }
        it++;
    }
    std::cout << "Actual Money: " << actual_money << " Expected Money: " << expected_money <<
            std::endl << "Sign error: " << sign_error << " out of " << all.size() << std::endl;

}

BOOST_AUTO_TEST_CASE(TestBenchmark){
    std::random_device rd{};
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(1, 21);
    auto pt = ProbTree<int>();
    for (int j = 0;
         j < 100; //j < 7144;
         j++) {
        auto seq = std::vector<int>();
        rng.seed(j);
        for(int i = 0; i < 336 ; i++) {
            seq.push_back(uni(rng));
        }
        auto seq2 = std::vector<int>();
        rng.seed(j);
        for(int i = 0; i < 335 ; i++) {
            seq.push_back(uni(rng));
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        pt.process(seq);
        auto t2 = std::chrono::high_resolution_clock::now();
        auto pre = std::map<int, double>();
        pt.predict(seq2, pre);
        auto t3 = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> ms = t2 - t1;
        std::chrono::duration<double, std::milli> ms2 = t3 - t2;
        std::cout << ms.count() << "ms, " << pt.nodes.size() << " nodes, and " << ms2.count() << "ms prediction \n";
    }

}

BOOST_AUTO_TEST_SUITE_END()
