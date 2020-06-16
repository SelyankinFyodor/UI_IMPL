#include <iostream>
#include "set-test-main.h"
#include "IVector.h"

bool insert1(ILogger *logger){
    const size_t dim = 2;
    double eps = 1e-3;
    double a1[dim]={1.0, 2.0};
    double a2[dim]={1.01, 2.01};
    double a3[dim]={1.0001, 2.0001};
    bool res = true;
    std::string msg = "insert-1: passed";

    IVector *va1 = IVector::createVector(dim, a1, logger);
    IVector *va2 = IVector::createVector(dim, a2, logger);
    IVector *va3 = IVector::createVector(dim, a3, logger);
    ISet *s = ISet::createSet(logger);

    s->insert(va1, IVector::NORM::NORM_2, eps);
    s->insert(va2, IVector::NORM::NORM_2, eps);
    s->insert(va3, IVector::NORM::NORM_2, eps);

    if (s->getSize() != 2){
        msg = "inset-1: the number of inserts is not equal to the expected number of inserts";
        res = false;
    }

    if (s->getDim() != 2){
        msg = "inset-1: dimension is not equal to expected";
        res = false;
    }

    delete va1;
    delete va2;
    delete va3;
    delete s;

    std::cout << msg << std::endl;

    return res;
}

bool insert2(ILogger *logger){
    const size_t dim = 2;
    const size_t dim1 = 3;
    double eps = 1e-3;
    double a1[dim]={1, 2};
    double a2[dim1]={1.01, 2.01, 3.01};
    double a3[dim]={1.0001, 2.0001};
    bool res = true;
    std::string msg = "insert-2: passed";

    IVector *va1 = IVector::createVector(dim, a1, logger);
    IVector *va2 = IVector::createVector(dim1, a2, logger);
    IVector *va3 = IVector::createVector(dim, a3, logger);
    ISet *s = ISet::createSet(logger);

    s->insert(va2, IVector::NORM::NORM_INF, eps);
    s->insert(va1, IVector::NORM::NORM_INF, eps);
    s->insert(va3, IVector::NORM::NORM_INF, eps);

    if (s->getSize() != 1){
        msg = "inset-2: the number of inserts is not equal to the expected number of inserts";
        res = false;
    }

    if (s->getDim() != 3){
        msg = "inset-2: dimension is not equal to expected";
        res = false;
    }

    delete va1;
    delete va2;
    delete va3;
    delete s;

    std::cout << msg << std::endl;

    return res;
}

bool insert3(ILogger *logger){
    double arr1[2] = {1,2};
    double arr2[2] = {1.00001,2.00001};
    double arr3[2] = {3,2};
    std::string msg = "insert-3: passed";
    IVector *vec1 = IVector::createVector(2, arr1, logger);
    IVector *vec2 = IVector::createVector(2, arr2, logger);
    IVector *vec3 = IVector::createVector(2, arr3, logger);
    IVector *vec4 = IVector::add(vec1, vec3, logger);
    ISet *set1 = ISet::createSet(logger);

    set1->insert(vec1, IVector::NORM::NORM_1, 0.001);
    set1->insert(vec2, IVector::NORM::NORM_1, 0.001);
    set1->insert(vec3, IVector::NORM::NORM_1, 0.001);
    set1->insert(vec4, IVector::NORM::NORM_1, 0.001);

    bool res = set1->getSize() == 3;
    if (!res){
        msg = "insert-3: insert close vectors";
    }

    std::cout << msg << std::endl;
    delete vec1;
    delete vec2;
    delete vec3;
    delete vec4;
    delete set1;
    return res;
}