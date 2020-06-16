#include <iostream>
#include "set-test-main.h"
#include "IVector.h"

bool clone1(ILogger *logger){
    const size_t dim = 2;
    double eps = 1e-8;
    double a1[dim]={1.0, 2.0};
    double a2[dim]={2.01, 4.01};
    double a3[dim]={3.0001, 0.0001};
    bool res = true;
    std::string msg = "clone-1: passed";

    IVector *va1 = IVector::createVector(dim, a1, logger);
    IVector *va2 = IVector::createVector(dim, a2, logger);
    IVector *va3 = IVector::createVector(dim, a3, logger);
    ISet *s = ISet::createSet(logger);

    s->insert(va1, IVector::NORM::NORM_2, eps);
    s->insert(va2, IVector::NORM::NORM_2, eps);
    s->insert(va3, IVector::NORM::NORM_2, eps);

    ISet *s1 = nullptr;
    s1 = s->clone();
    if (!s1){
        msg = "clone-1: clone not work";
        res = false;
    }
    if (s1) {
        if (s1->getDim()!= s->getDim()){
            msg = "clone-1: dimension mismatch";
            res = false;
        }
        if (s1->getSize() != s->getSize()){
            msg = "clone-1: does not match the number of elements";
            res = false;
        }
        else {
            size_t dim = s1->getDim();
            size_t size = s1->getSize();

            IVector *va4 = nullptr;
            IVector *va5 = nullptr;
            IVector *va6 = nullptr;

            RESULT_CODE r1 = s1->get(va4, va1,IVector::NORM::NORM_2, eps);
            RESULT_CODE r2 = s1->get(va5, va2,IVector::NORM::NORM_2, eps);
            RESULT_CODE r3 = s1->get(va6, va3,IVector::NORM::NORM_2, eps);
            if (r1 != RESULT_CODE::SUCCESS || r2 != RESULT_CODE::SUCCESS || r3 != RESULT_CODE::SUCCESS){
                msg = "clone-1: source elements not found";
                res = false;
            }

            if (res){
                if ((va4->getCoord(0) - va1->getCoord(0) > eps) ||
                    (va4->getCoord(1) - va1->getCoord(1) > eps) ||
                    (va5->getCoord(0) - va2->getCoord(0) > eps) ||
                    (va5->getCoord(1) - va2->getCoord(1) > eps) ||
                    (va5->getCoord(0) - va2->getCoord(0) > eps) ||
                    (va6->getCoord(1) - va3->getCoord(1) > eps)){
                    msg = "clone-1: source and clone elements do not match";
                    res = false;
                }
            }
        }


    }

    delete va1;
    delete va2;
    delete va3;
    delete s;
    delete s1;

    std::cout << msg << std::endl;

    return res;
}
