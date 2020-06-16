#include <cmath>
#include <iostream>
#include "set-test-main.h"
#include "IVector.h"


bool get1(ILogger *logger){
    const size_t dim = 2;
    double eps = 1e-3;
    double a1[dim]={1.0, 2.0};
    double a2[dim]={1.01, 2.01};
    bool res = true;
    std::string msg = "get-1: passed";

    IVector *va1 = IVector::createVector(dim, a1, logger);
    IVector *va2 = IVector::createVector(dim, a2, logger);
    ISet *s = ISet::createSet(logger);

    s->insert(va1, IVector::NORM::NORM_2, eps);
    s->insert(va2, IVector::NORM::NORM_2, eps);

    IVector *va4 = nullptr;
    IVector *va5 = nullptr;
    IVector *va6 = nullptr;

    RESULT_CODE r1 = s->get(va4, 0);
    RESULT_CODE r2 = s->get(va5, 1);
    RESULT_CODE r3 = s->get(va6, 2);

    if (r1 != RESULT_CODE::SUCCESS && r2 != RESULT_CODE::SUCCESS){
        msg = "get-1: could not get the item by index";
        res = false;
    }
    if (r3 == RESULT_CODE::SUCCESS){
        msg = "get-1: took out a nonexistent element";
        res = false;
    }

    delete va1;
    delete va2;
    delete va4;
    delete va5;
    delete va6;
    delete s;

    std::cout << msg << std::endl;

    return res;
}

bool get2(ILogger *logger){
    const size_t dim = 2;
    double eps = 1e-3;
    double a1[dim]={1.0, 2.0};
    double a2[dim]={1.01, 2.01};
    double a3[dim]={5.0001, 2.0001};
    double a4[dim]={4.0001, 1.0001};
    bool res = true;
    std::string msg = "get-2: passed";

    IVector *va1 = IVector::createVector(dim, a1, logger);
    IVector *va2 = IVector::createVector(dim, a2, logger);
    IVector *va3 = IVector::createVector(dim, a3, logger);
    IVector *va_fake = IVector::createVector(dim, a4, logger);
    ISet *s = ISet::createSet(logger);

    s->insert(va1, IVector::NORM::NORM_2, eps);
    s->insert(va2, IVector::NORM::NORM_2, eps);
    s->insert(va3, IVector::NORM::NORM_2, eps);

    IVector *va4 = nullptr;

    s->get(va4, va_fake,IVector::NORM::NORM_2, eps);

    if (va4){
        msg = "get-2: nonexistent element found";
        res = false;
    }

    delete va1;
    delete va2;
    delete va3;
    delete va4;
    delete s;

    std::cout << msg << std::endl;

    return res;
}

bool get3(ILogger *logger){
    const size_t dim = 2;
    double eps = 1e-3;
    double a1[dim]={1.0, 2.0};
    double a2[dim]={1.01, 2.01};
    double a3[dim]={1.0001, 2.0001};
    bool res = true;
    std::string msg = "get-3: passed";

    IVector *va1 = IVector::createVector(dim, a1, logger);
    IVector *va2 = IVector::createVector(dim, a2, logger);
    IVector *va3 = IVector::createVector(dim, a3, logger);
    ISet *s = ISet::createSet(logger);

    s->insert(va1, IVector::NORM::NORM_2, eps);
    s->insert(va2, IVector::NORM::NORM_2, eps);
    s->insert(va3, IVector::NORM::NORM_2, eps);

    IVector *va4 = nullptr;
    IVector *va5 = nullptr;
    IVector *va6 = nullptr;

    s->get(va4, va1,IVector::NORM::NORM_2, eps);
    s->get(va5, va2,IVector::NORM::NORM_2, eps);
    s->get(va6, va3,IVector::NORM::NORM_2, eps);

    if ((va4->getCoord(0) - va1->getCoord(0) > eps) ||
        (va4->getCoord(1) - va1->getCoord(1) > eps) ||
        (va5->getCoord(0) - va2->getCoord(0) > eps) ||
        (va5->getCoord(1) - va2->getCoord(1) > eps) ||
        (va5->getCoord(0) - va2->getCoord(0) > eps) ||
        (va6->getCoord(1) - va3->getCoord(1) > eps)){
        msg = "get-3: the coordinates of the inserted vectors are not equal to the coordinates of the resulting vectors";
        res = false;
    }

    delete va1;
    delete va2;
    delete va3;
    delete va4;
    delete va5;
    delete va6;
    delete s;

    std::cout << msg << std::endl;

    return res;
}