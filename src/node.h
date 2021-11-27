#ifndef NODE_H
#define NODE_H

#include <memory>
#include <unordered_map>
#include <map>
#include <vector>
#include <boost/serialization/unordered_map.hpp>


using namespace std;

template <class T>
class Node{
    friend class boost::serialization::access;
public:
    Node(){}

    Node<T>* add(std::vector<T>& seq){
        auto nodes = std::vector<Node<T>*>();
        auto it = seq.begin();
        Node<T>* node = this;
        while(it != seq.end()){
            T s = *it;
            auto f = node->__prob_map__.find(s);
            if(f == node->__prob_map__.end()){
                ++it;
                if(it == seq.end()){
                    Node<T>* n = this;
                    if(node != this){
                        n = node;
                        nodes.push_back(n);
                    }
                    n->__prob_map__[s] = new Node();
                    nodes.push_back(n->__prob_map__[s]);
                    if(n->__prob_map__.size() == 1){
                        n->__count__++;
                        auto n2 = nodes.rbegin();
                        int c = 0;
                        while(n2 != nodes.rend()){
                            if((*n2)->__depth__ < c){
                                (*n2)->__depth__ = c;
                            }
                            c++;
                            n2++;
                        }
                    }
                    return n->__prob_map__[s];
                }
            } else {
                nodes.push_back(node);
                node = (*f).second;
                ++it;
            }
        }
        return NULL;
    }

    void predict(std::vector<T>& seq, std::map<T, double>& probabilities){
        auto it = seq.begin();
        Node<T>* node = this;
        Node<T>* actual_node;
        while(it != seq.end()){
            T s = *it;
            if(node->__prob_map__.find(s) == node->__prob_map__.end()){
                return;
            } else {
                actual_node = node;
                node = node->__prob_map__[s];
                ++it;
            }
        }
        double sum = 0;
        for(auto& e : actual_node->__prob_map__){
            sum += e.second->__count__;
        }
        for(auto& e : actual_node->__prob_map__){
            if(probabilities.find(e.first) == probabilities.end()){
                probabilities[e.first] =((double)e.second->__count__)/sum;
            } else{
                probabilities[e.first]
                    = (((double)(e.second->__count__)/sum)
                       + (double)probabilities[e.first])/2.0;
            }
        }
    }

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version){
        ar.template register_type<Node<T>>();
        ar & __prob_map__;
        ar & __depth__;
        ar & __count__;
    }

    int __count__{0};
    std::unordered_map<T, Node<T>*> __prob_map__;
    int __depth__ = 0;
};

#endif // NODE_H
