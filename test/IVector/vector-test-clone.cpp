#include <iostream>
#include <cmath>
#include "vector-test-main.h"

bool clone1(ILogger *logger) {
    const size_t dim = 2;
    const double eps = 1e-7;
    double a[dim] = {1, 2};
    double b[dim] = {3, 4};

    IVector *va = IVector::createVector(dim, a, logger);
    IVector *vb = va->clone();
    if (va->getDim() != vb->getDim()){
        std::cout << "clone-1: mismatch dimensions\n";
        return false;
    }
    if (fabs(va->getCoord(0) - vb->getCoord(0))>eps &&
        fabs(va->getCoord(1) - vb->getCoord(1))>eps){
        std::cout << "clone-1: mismatch coordinates\n";
        return false;
    }

    std::cout << "clone-1: passed\n";
    delete va;
    delete vb;
    return true;
}