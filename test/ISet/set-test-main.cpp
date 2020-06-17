#include "set-test-main.h"
#include <iostream>

bool clone1(ILogger *logger){
    const size_t dim = 2;
    double eps = 1e-8;
    double a1[dim]={1.0, 2.0};
    double a2[dim]={2.01, 4.01};
    double a3[dim]={3.0001, 0.0001};
    bool res = true;

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
        std::cout << "clone-1: clone not work" << std::endl;
        res = false;
    }
    if (s1) {
        if (s1->getDim()!= s->getDim()){
            std::cout << "clone-1: dimension mismatch" << std::endl;
            res = false;
        }
        if (s1->getSize() != s->getSize()){
            std::cout << "clone-1: does not match the number of elements" << std::endl;
            res = false;
        }
        else {
            IVector *va4 = nullptr;
            IVector *va5 = nullptr;
            IVector *va6 = nullptr;

            RESULT_CODE r1 = s1->get(va4, va1,IVector::NORM::NORM_2, eps);
            RESULT_CODE r2 = s1->get(va5, va2,IVector::NORM::NORM_2, eps);
            RESULT_CODE r3 = s1->get(va6, va3,IVector::NORM::NORM_2, eps);
            if (r1 != RESULT_CODE::SUCCESS || r2 != RESULT_CODE::SUCCESS || r3 != RESULT_CODE::SUCCESS){
                std::cout << "clone-1: source elements not found" << std::endl;
                res = false;
            }

            if (res){
                if ((va4->getCoord(0) - va1->getCoord(0) > eps) ||
                    (va4->getCoord(1) - va1->getCoord(1) > eps) ||
                    (va5->getCoord(0) - va2->getCoord(0) > eps) ||
                    (va5->getCoord(1) - va2->getCoord(1) > eps) ||
                    (va5->getCoord(0) - va2->getCoord(0) > eps) ||
                    (va6->getCoord(1) - va3->getCoord(1) > eps)){
                    std::cout << "clone-1: source and clone elements do not match" << std::endl;
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
    if (res){
        std::cout << "clone-1: passed" << std::endl;
    }
    return res;
}

bool clear1(ILogger *logger){
    const size_t dim = 2;
    double eps = 1e-8;
    double a1[dim]={1.0, 2.0};
    double a2[dim]={2.01, 4.01};
    double a3[dim]={3.0001, 0.0001};
    bool res = true;

    IVector *va1 = IVector::createVector(dim, a1, logger);
    IVector *va2 = IVector::createVector(dim, a2, logger);
    IVector *va3 = IVector::createVector(dim, a3, logger);
    ISet *s = ISet::createSet(logger);

    s->insert(va1, IVector::NORM::NORM_2, eps);
    s->insert(va2, IVector::NORM::NORM_2, eps);
    s->insert(va3, IVector::NORM::NORM_2, eps);

    s->clear();
    if (s->getSize() != 0){
        std::cout << "clear-1: cleaning does not clean" << std::endl;
        res = false;
    }


    delete va1;
    delete va2;
    delete va3;
    delete s;
    if (res){
        std::cout << "clear-1: passed" << std::endl;
    }

    return res;
}

bool get1(ILogger *logger){
    const size_t dim = 2;
    double eps = 1e-3;
    double a1[dim]={1.0, 2.0};
    double a2[dim]={1.01, 2.01};
    bool res = true;

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
        std::cout << "get-1: could not get the item by index" << std::endl;
        res = false;
    }
    if (r3 == RESULT_CODE::SUCCESS){
        std::cout << "get-1: took out a nonexistent element" << std::endl;
        res = false;
    }

    delete va1;
    delete va2;
    delete va4;
    delete va5;
    delete va6;
    delete s;

    if (res){
        std::cout << "get-1: passed" << std::endl;
    }

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
        std::cout << "get-2: nonexistent element found" << std::endl;
        res = false;
    }

    delete va1;
    delete va2;
    delete va3;
    delete va4;
    delete s;

    if (res){
        std::cout << "get-2: passed" << std::endl;
    }

    return res;
}

bool get3(ILogger *logger){
    const size_t dim = 2;
    double eps = 1e-3;
    double a1[dim]={1.0, 2.0};
    double a2[dim]={1.01, 2.01};
    double a3[dim]={1.0001, 2.0001};
    bool res = true;

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
        std::cout << "get-3: the coordinates of the inserted vectors are not equal to the coordinates of the resulting vectors" << std::endl;
        res = false;
    }

    delete va1;
    delete va2;
    delete va3;
    delete va4;
    delete va5;
    delete va6;
    delete s;

    if (res){
        std::cout << "get-3: passed" << std::endl;
    }

    return res;
}

bool erase1(ILogger *logger){
    const size_t dim = 2;
    double eps = 1e-3;
    double a1[dim]={1.0, 2.0};
    double a2[dim]={1.01, 2.01};
    bool res = true;

    IVector *va1 = IVector::createVector(dim, a1, logger);
    IVector *va2 = IVector::createVector(dim, a2, logger);
    ISet *s = ISet::createSet(logger);

    s->insert(va1, IVector::NORM::NORM_2, eps);
    s->insert(va2, IVector::NORM::NORM_2, eps);

    RESULT_CODE r1 = s->erase(0);
    RESULT_CODE r2 = s->erase(0);
    RESULT_CODE r3 = s->erase(0);

    if (r1 != RESULT_CODE::SUCCESS && r2 != RESULT_CODE::SUCCESS){
        std::cout << "erase-1: could not erase the item by index" << std::endl;
        res = false;
    }
    if (r3 == RESULT_CODE::SUCCESS){
        std::cout << "erase-1: took out a nonexistent element" << std::endl;
        res = false;
    }

    delete va1;
    delete va2;
    delete s;

    if (res){
        std::cout << "erase-1: passed" << std::endl;
    }

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
        std::cout << "erase-2: after erasing nonexistent element, the size of the set decrease" << std::endl;
        res = false;
    }

    delete va1;
    delete va2;
    delete va3;
    delete s;

    if (res){
        std::cout << "erase-2: passed" << std::endl;
    }

    return res;
}

bool erase3(ILogger *logger){
    const size_t dim = 2;
    double eps = 1e-3;
    double a1[dim]={1.0, 2.0};
    double a2[dim]={1.01, 2.01};
    double a3[dim]={1.0001, 2.0001};
    bool res = true;

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
        std::cout << "erase-3: there was no zeroing of the dimension with an empty set" << std::endl;
        res = false;
    }

    delete va1;
    delete va2;
    delete va3;
    delete s;

    if (res){
        std::cout << "erase-3: passed" << std::endl;
    }

    return res;
}

bool insert1(ILogger *logger){
    const size_t dim = 2;
    double eps = 1e-3;
    double a1[dim]={1.0, 2.0};
    double a2[dim]={1.01, 2.01};
    double a3[dim]={1.0001, 2.0001};
    bool res = true;

    IVector *va1 = IVector::createVector(dim, a1, logger);
    IVector *va2 = IVector::createVector(dim, a2, logger);
    IVector *va3 = IVector::createVector(dim, a3, logger);
    ISet *s = ISet::createSet(logger);

    s->insert(va1, IVector::NORM::NORM_2, eps);
    s->insert(va2, IVector::NORM::NORM_2, eps);
    s->insert(va3, IVector::NORM::NORM_2, eps);

    if (s->getSize() != 2){
        std::cout << "inset-1: the number of inserts is not equal to the expected number of inserts" << std::endl;
        res = false;
    }

    if (s->getDim() != 2){
        std::cout << "inset-1: dimension is not equal to expected" << std::endl;
        res = false;
    }

    delete va1;
    delete va2;
    delete va3;
    delete s;

    if (res){
        std::cout <<  "insert-1: passed" << std::endl;
    }

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

    IVector *va1 = IVector::createVector(dim, a1, logger);
    IVector *va2 = IVector::createVector(dim1, a2, logger);
    IVector *va3 = IVector::createVector(dim, a3, logger);
    ISet *s = ISet::createSet(logger);

    s->insert(va2, IVector::NORM::NORM_INF, eps);
    s->insert(va1, IVector::NORM::NORM_INF, eps);
    s->insert(va3, IVector::NORM::NORM_INF, eps);

    if (s->getSize() != 1){
        std::cout << "inset-2: the number of inserts is not equal to the expected number of inserts" << std::endl;
        res = false;
    }

    if (s->getDim() != 3){
        std::cout << "inset-2: dimension is not equal to expected" << std::endl;
        res = false;
    }

    delete va1;
    delete va2;
    delete va3;
    delete s;

    if (res){
        std::cout << "insert-2: passed" << std::endl;
    }

    return res;
}

bool insert3(ILogger *logger){
    double arr1[2] = {1,2};
    double arr2[2] = {1.00001,2.00001};
    double arr3[2] = {3,2};

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
        std::cout << "insert-3: insert close vectors" << std::endl;
    }

    if (res){
        std::cout << "insert-3: passed" << std::endl;
    }

    delete vec1;
    delete vec2;
    delete vec3;
    delete vec4;
    delete set1;
    return res;
}

int main(){
    ILogger *l = ILogger::createLogger(nullptr);
    insert1(l);
    insert2(l);
    insert3(l);

    get1(l);
    get2(l);
    get3(l);

    clear1(l);

    clone1(l);

    erase1(l);
    erase2(l);
    erase3(l);

    l->destroyLogger(nullptr);
    return 0;
}