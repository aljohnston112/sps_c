#ifndef STOCK_H
#define STOCK_H

#include <string>

class stock {
public:
    std::string file_name;
    double actual;
    double max;
    double prediction;

    stock(){

    }

    stock(std::string file_name,
          double actual,
          double max,
          double prediction
          ){
        this->file_name = file_name;
        this->actual = actual;
        this->max = max;
        this->prediction = prediction;
    }
};

#endif // STOCK_H
