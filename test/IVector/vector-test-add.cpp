#include <iostream>
#include <cmath>
#include "vector-test-main.h"

bool add1() {
    const size_t dim = 2;
    const double eps = 1e-7;
    double a[dim] = {1, 2};
    double b[dim] = {3, 4};

    IVector *va = IVector::createVector(dim, a, nullptr);
    IVector *vb = IVector::createVector(dim, b, nullptr);
    IVector *sum = IVector::add(va, vb, nullptr);
    if (sum->getDim() != dim){
        std::cout << "add-1: result dimension error\n";
        delete va;
        delete vb;
        delete sum;
        return false;
    }
    if (fabs(sum->getCoord(0) - (a[0]+b[0])) > eps &&
        fabs(sum->getCoord(1) - (a[1]+b[1])) > eps){
        std::cout << "add-1: result coordinates error\n";
        delete va;
        delete vb;
        delete sum;
        return false;
    }
    std::cout << "add-1: passed\n";
    delete va;
    delete vb;
    delete sum;
    return true;
}

bool add2() {
    const size_t dim1 = 2;
    const size_t dim2 = 3;
    double a[dim1] = {1, 2};
    double b[dim2] = {3, 4, 6};

    IVector *va = IVector::createVector(dim1, a, nullptr);
    IVector *vb = IVector::createVector(dim2, b, nullptr);
    IVector *sum = IVector::add(va, vb, nullptr);
    if (sum != nullptr){
        std::cout << "add-2: mismatch dimensional vectors\n";
        delete va;
        delete vb;
        delete sum;
        return false;
    }
    std::cout << "add-2: passed\n";
    delete va;
    delete vb;
    delete sum;
    return true;
}

bool add3(){
    const size_t dim = 3;
    double a[dim] = {1, 2, 3};

    IVector *va = IVector::createVector(dim, a, nullptr);
    IVector *vb = nullptr;
    IVector *sum = IVector::add(va, nullptr, nullptr);
    if (sum != nullptr){
        std::cout << "add-3: sum mismatch dimensional vectors\n";
        delete va;
        delete sum;
        return false;
    }
    std::cout << "add-3: passed\n";
    delete va;
    delete sum;
    return true;
}