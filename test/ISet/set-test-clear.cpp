#include <iostream>
#include "set-test-main.h"
#include "IVector.h"

bool clear1(ILogger *logger){
    const size_t dim = 2;
    double eps = 1e-8;
    double a1[dim]={1.0, 2.0};
    double a2[dim]={2.01, 4.01};
    double a3[dim]={3.0001, 0.0001};
    bool res = true;
    std::string msg = "clear-1: passed";

    IVector *va1 = IVector::createVector(dim, a1, logger);
    IVector *va2 = IVector::createVector(dim, a2, logger);
    IVector *va3 = IVector::createVector(dim, a3, logger);
    ISet *s = ISet::createSet(logger);

    s->insert(va1, IVector::NORM::NORM_2, eps);
    s->insert(va2, IVector::NORM::NORM_2, eps);
    s->insert(va3, IVector::NORM::NORM_2, eps);

    s->clear();
    if (s->getSize() != 0){
        msg = "clear-1: cleaning does not clean";
        res = false;
    }


    delete va1;
    delete va2;
    delete va3;
    delete s;

    std::cout << msg << std::endl;

    return res;
}