#ifndef VECTOR_UTIL_H
#define VECTOR_UTIL_H

#include <vector>

template <typename T>
void chunk_vector(std::vector<T>& source, int chunkSize, std::vector<std::vector<T>>& returnVector){
    if(chunkSize == 0){
        returnVector.push_back(source);
    }else{
        for (int i = 0; i < source.size(); i += chunkSize) {
            auto start = source.begin() + i;
            auto end = start + chunkSize > source.end() ? source.end() : start + chunkSize;
            std::vector<T> tempVector(start, end);
            returnVector.push_back(tempVector);
        }
    }
}

#endif // VECTOR_UTIL_H
