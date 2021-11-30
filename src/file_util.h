#ifndef FILE_UTIL_H
#define FILE_UTIL_H

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "rapidcsv.h"
#include <fstream>
#include <filesystem>

#include "node.hpp"
#include "probtree.hpp"



using namespace boost::archive;

template <class T>
void write(string file_name, ProbTree<T>& map){
    std::ofstream ofs;
    ofs.open(file_name, std::ios::binary);
    binary_oarchive oa(ofs);
    oa << map;
}

template <class T>
void read(string file_name, ProbTree<T>& map){
    std::ifstream ofs;
    ofs.open(file_name, std::ios::binary);
    binary_iarchive oa(ofs);
    oa >> map;
}

void process_csv(string folder, ProbTree<float>& pt){
    string ext = ".csv";
    for(auto& p : filesystem::recursive_directory_iterator(filesystem::path(folder))){
        if(p.path().extension() == ext){
            rapidcsv::Document doc(p.path());
            std::cout << p.path() << std::endl;
            std::vector<float> low = doc.GetColumn<float>("Low");
            pt.process(low);
        }
    }
}

void predict_csv(string folder, ProbTree<float>& pt, std::map<float, double> probs){
    string ext = ".csv";
    for(auto& p : filesystem::recursive_directory_iterator(filesystem::path(folder))){
        if(p.path().extension() == ext){
            rapidcsv::Document doc(p.path());
            std::cout << p.path() << std::endl;
            std::vector<float> low = doc.GetColumn<float>("Low");
            pt.predict(low, probs);
        }
    }
}

void get_price_data(string file, std::vector<double>& v){
    fstream fin;
    fin.open(file, ios::in);
    string temp;
    string line;
    string word;
    std::vector<string> row{};
    if(fin >> temp){
        stringstream s(temp);
        while(getline(s, word, ',')){
            row.push_back(word);
        }
        assert(row[1] == "Low");
        row.clear();
    }
    while(fin >> temp){
        stringstream s(temp);
        while(getline(s, word, ',')){
            row.push_back(word);
        }
        v.push_back(std::stod(row[1]));
        row.clear();
    }
}

double get_max(string file){
    fstream fin;
    fin.open(file, ios::in);
    string temp;
    if(fin >> temp){
        assert(temp == "max");
    }
    if(fin >> temp){
        stringstream s(temp);
        return std::stod(temp);
    }
    return -1;
}

#endif // FILE_UTIL_H
