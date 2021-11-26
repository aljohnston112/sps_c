#ifndef FILE_UTIL_H
#define FILE_UTIL_H

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <fstream>

#include "node.h"
#include "probtree.h"



using namespace boost::archive;

template <class T>
void write(char* file_name, ProbTree<T>* map){
    std::ofstream ofs;
    ofs.open(file_name, std::ios::binary);
    binary_oarchive oa(ofs);
    oa << *map;
}

template <class T>
void read(char* file_name, ProbTree<T>& map){
    std::ifstream ofs;
    ofs.open(file_name, std::ios::binary);
    binary_iarchive oa(ofs);
    oa >> map;
}

#endif // FILE_UTIL_H
