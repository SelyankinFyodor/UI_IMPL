#include <cmath>
#include <iostream>
#include "set-test-main.h"
#include "IVector.h"


bool erase1(ILogger *logger){
    const size_t dim = 2;
    double eps = 1e-3;
    double a1[dim]={1.0, 2.0};
    double a2[dim]={1.01, 2.01};
    bool res = true;
    std::string msg = "erase-1: passed";

    IVector *va1 = IVector::createVector(dim, a1, logger);
    IVector *va2 = IVector::createVector(dim, a2, logger);
    ISet *s = ISet::createSet(logger);

    s->insert(va1, IVector::NORM::NORM_2, eps);
    s->insert(va2, IVector::NORM::NORM_2, eps);

    RESULT_CODE r1 = s->erase(0);
    RESULT_CODE r2 = s->erase(0);
    RESULT_CODE r3 = s->erase(0);

    if (r1 != RESULT_CODE::SUCCESS && r2 != RESULT_CODE::SUCCESS){
        msg = "erase-1: could not erase the item by index";
        res = false;
    }
    if (r3 == RESULT_CODE::SUCCESS){
        msg = "erase-1: took out a nonexistent element";
        res = false;
    }

    delete va1;
    delete va2;
    delete s;

    std::cout << msg << std::endl;

    return res;
}

bool erase2(ILogger *logger){
    const size_t dim = 2;
    double eps = 1e-3;
    double a1[dim]={1.0, 2.0};
    double a2[dim]={1.01, 2.01};
    double a3[dim]={5.0001, 2.0001};
    double a4[dim]={4.0001, 1.0001};
    bool res = true;
    std::string msg = "erase-2: passed";

    IVector *va1 = IVector::createVector(dim, a1, logger);
    IVector *va2 = IVector::createVector(dim, a2, logger);
    IVector *va3 = IVector::createVector(dim, a3, logger);
    IVector *va_fake = IVector::createVector(dim, a4, logger);
    ISet *s = ISet::createSet(logger);

    s->insert(va1, IVector::NORM::NORM_2, eps);
    s->insert(va2, IVector::NORM::NORM_2, eps);
    s->insert(va3, IVector::NORM::NORM_2, eps);

    RESULT_CODE r = s->erase(va_fake,IVector::NORM::NORM_2, eps);

    if (s->getSize() != 3) {
        msg = "erase-2: after erasing nonexistent element, the size of the set decrease";
        res = false;
    }

    delete va1;
    delete va2;
    delete va3;
    delete s;

    std::cout << msg << std::endl;

    return res;
}

bool erase3(ILogger *logger){
    const size_t dim = 2;
    double eps = 1e-3;
    double a1[dim]={1.0, 2.0};
    double a2[dim]={1.01, 2.01};
    double a3[dim]={1.0001, 2.0001};
    bool res = true;
    std::string msg = "erase-3: passed";

    IVector *va1 = IVector::createVector(dim, a1, logger);
    IVector *va2 = IVector::createVector(dim, a2, logger);
    IVector *va3 = IVector::createVector(dim, a3, logger);
    ISet *s = ISet::createSet(logger);

    s->insert(va1, IVector::NORM::NORM_2, eps);
    s->insert(va2, IVector::NORM::NORM_2, eps);
    s->insert(va3, IVector::NORM::NORM_2, eps);


    s->erase(va1,IVector::NORM::NORM_2, eps);
    s->erase(va2,IVector::NORM::NORM_2, eps);
    s->erase(va3,IVector::NORM::NORM_2, eps);

    if (s->getDim()){
        msg = "erase-3: there was no zeroing of the dimension with an empty set";
        res = false;
    }

    delete va1;
    delete va2;
    delete va3;
    delete s;

    std::cout << msg << std::endl;

    return res;
}