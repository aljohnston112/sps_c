#ifndef PROBTREE_H
#define PROBTREE_H

#include "node.h"
#include "vector_util.h"
#include <vector>
#include <boost/serialization/vector.hpp>
#include <thread>
#include <execution>
#include <map>


template <class T>
void increment_counts(std::vector<Node<T>*>& nodes, std::vector<T>& seq){
    for(auto current_node: nodes){
        auto node_path = std::vector<Node<T>*>();
        auto it = seq.begin();
        Node<T>* node = current_node;
        while(it != seq.end()){
            T s = *it;
            auto f = node->__prob_map__.find(s);
            if(f == node->__prob_map__.end()){
                ++it;
                if(it == seq.end()){
                    if(node != current_node && node->__count__ != 0){
                        for(auto n: node_path){
                            n->__count__++;
                        }
                    }
                }
                return;
            } else {
                node = (*f).second;
                node_path.push_back(node);
                ++it;
            }
        }
        if(node != current_node && node->__count__ != 0){
            for(auto n: node_path){
                n->__count__++;
            }
        }
    }
}

template <class T>
class ProbTree{
friend class boost::serialization::access;
public:
    ProbTree(){
        nodes.reserve(60000);
        nodes.push_back(&__root__);
    }

    ProbTree(std::vector<T>& seq){
        nodes.reserve(60000);
        nodes.push_back(&__root__);
        process(seq);
    }

    ~ProbTree(){

    }

    void process(std::vector<T>& seq){
        auto current = std::vector<T>();
        for(auto it = seq.begin(); it != seq.end(); ++it){
            auto current_seq = *it;
            current.push_back(current_seq);
            auto s = std::vector<T>(current);
            while(s.size() != 0){
                auto node = __root__.add(s);
                if(node != NULL){
                    nodes.push_back(node);
                }
                increment_counts<T>(nodes, s);
                s.erase(s.begin());
            }
        }
    }

    void predict(std::vector<T>& seq, std::map<T, double>& probabilities){
        auto s = std::vector<T>(seq);
        while(s.size() != 0){
            __root__.predict(s, probabilities);
            s.erase(s.begin());
        }
        double sum = 0;
        for(auto e: probabilities){
            sum += e.second;
        }
        auto scale = 1.0/sum;
        for(auto e: probabilities){
            probabilities[e.first] = e.second * scale;
        }
    }

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version){
        ar.template register_type<ProbTree<T>>();
        ar & __root__;
        ar & nodes;
    }

    Node<T> __root__ = Node<T>();
    std::vector<Node<T>*> nodes = std::vector<Node<T>*>();
};

int add(int i, int j) {
    return i + j;
}

#endif // PROBTREE_H
