#include <iostream>
#include <cmath>
#include "vector-test-main.h"


bool mulvector1(){
    const size_t dim = 2;
    const double eps = 1e-7;
    double a[dim] = {1, 2};
    double b[dim] = {3, 4};

    IVector *va = IVector::createVector(dim, a, nullptr);
    IVector *vb = IVector::createVector(dim, b, nullptr);
    double res = IVector::mul(va, vb, nullptr);
    if (fabs(res - 11) > eps){
        std::cout << "mul-vector-1: incorrect result of multiply\n";
        delete va;
        delete vb;
        return false;
    }
    std::cout << "mul-vector-1: passed\n";
    delete va;
    delete vb;
    return true;
}

bool mulvector2(){
    const size_t dim = 2;
    const size_t dim1 = 3;
    const double eps = 1e-7;
    double a[dim] = {1, 2};
    double b[dim1] = {3, 4, 5};

    IVector *va = IVector::createVector(dim, a, nullptr);
    IVector *vb = IVector::createVector(dim1, b, nullptr);
    double res = IVector::mul(va, vb, nullptr);
    if (!std::isnan(res)){
        std::cout << "mul-vector-2: incorrect result of multiply\n";
        delete va;
        delete vb;
        return false;
    }
    std::cout << "mul-vector-2: passed\n";
    delete va;
    delete vb;
    return true;
}

bool mulvector3(){
    const size_t dim = 2;
    const size_t dim1 = 3;
    const double eps = 1e-7;

    double b[dim1] = {3, 4, 5};

    IVector *vb = IVector::createVector(dim1, b, nullptr);
    double res = IVector::mul(nullptr, vb, nullptr);
    if (!std::isnan(res)){
        std::cout << "mul-vector-3: incorrect result of multiply\n";
        delete vb;
        return false;
    }
    std::cout << "mul-vector-3: passed\n";
    delete vb;
    return true;
}