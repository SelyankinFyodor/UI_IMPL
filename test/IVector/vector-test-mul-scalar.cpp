#include <iostream>
#include <cmath>
#include "vector-test-main.h"


bool mulscalar1(){
    const size_t dim = 2;
    const double eps = 1e-7;
    double a[dim] = {1, 2};

    IVector *va = IVector::createVector(dim, a, nullptr);
    IVector *res = IVector::mul(va, 1123, nullptr);
    if (res->getCoord(0)!= 1123 &&
        res->getCoord(0)!= 2246){
        std::cout << "mul-scalar-1: incorrect result of multiply\n";
        delete va;
        delete res;
        return false;
    }
    std::cout << "mul-scalar-1: passed\n";
    delete va;
    delete res;
    return true;
}

bool mulscalar2(){
    const size_t dim = 2;
    const double eps = 1e-7;

    IVector * res = IVector::mul(nullptr, 12345, nullptr);
    if (res){
        std::cout << "scalar-mul-2: incorrect result of multiply\n";
        return false;
    }
    std::cout << "scalar-mul-2: passed\n";
    return true;
}