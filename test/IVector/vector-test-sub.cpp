#include <iostream>
#include <cmath>
#include "vector-test-main.h"

bool sub1(ILogger *logger){

    const size_t dim = 2;
    const double eps = 1e-7;
    double a[dim] = {1, 2};
    double b[dim] = {3, 4};

    IVector *va = IVector::createVector(dim, a, logger);
    IVector *vb = IVector::createVector(dim, b, logger);
    IVector *sub = IVector::sub(va, vb, logger);
    if (sub->getDim() != dim){
        std::cout << "add-1: result dimension error\n";
        delete va;
        delete vb;
        delete sub;
        return false;
    }
    if (fabs(sub->getCoord(0) - (a[0]-b[0])) > eps &&
        fabs(sub->getCoord(1) - (a[1]-b[1])) > eps){
        std::cout << "sub-1: result coordinates error\n";
        delete va;
        delete vb;
        delete sub;
        return false;
    }
    std::cout << "sub-1: passed\n";
    delete va;
    delete vb;
    delete sub;
    return true;
}

bool sub2(ILogger *logger) {
    const size_t dim1 = 2;
    const size_t dim2 = 3;
    double a[dim1] = {1, 2};
    double b[dim2] = {3, 4, 6};

    IVector *va = IVector::createVector(dim1, a, logger);
    IVector *vb = IVector::createVector(dim2, b, logger);
    IVector *sub = IVector::sub(va, vb, logger);
    if (sub != nullptr){
        std::cout << "sub-2: mismatch dimensional\n";
        delete va;
        delete vb;
        delete sub;
        return false;
    }
    std::cout << "sub-2: passed\n";
    delete va;
    delete vb;
    delete sub;
    return true;
}

bool sub3(ILogger *logger){
    const size_t dim = 3;
    double a[dim] = {1, 2, 3};

    IVector *va = IVector::createVector(dim, a, logger);
    IVector *vb = nullptr;
    IVector *sum = IVector::add(va, nullptr, logger);
    if (sum != nullptr){
        std::cout << "sub-3: sum mismatch dimensional vectors\n";
        delete va;
        delete sum;
        return false;
    }
    std::cout << "sub-3: passed\n";
    delete va;
    delete sum;
    return true;
}