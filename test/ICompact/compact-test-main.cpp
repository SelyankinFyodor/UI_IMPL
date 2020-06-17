#include <iostream>

#include "compact-test-main.h"

void create1(ILogger *logger){
    double tolerance = 1e-5;

    double a1[] = { 1, 1 };
    double a2[] = { 2, 2 };

    IVector* v1 = IVector::createVector(2, a1, logger);
    IVector* v2 = IVector::createVector(2, a2, logger);

    ICompact* compact = ICompact::createCompact(v1, v2, logger);

    IVector* vec = compact->getBegin();
    bool true_beg;
    RESULT_CODE rc1 = IVector::equals(v1, vec, IVector::NORM::NORM_1, tolerance, &true_beg, logger);
    delete vec;
    vec = nullptr;

    vec = compact->getEnd();
    bool true_end;
    RESULT_CODE rc2 = IVector::equals(v2, vec, IVector::NORM::NORM_1, tolerance, &true_end, logger);
    delete vec;
    vec = nullptr;

    if (rc1 == RESULT_CODE::SUCCESS && rc2 == RESULT_CODE::SUCCESS && true_beg && true_end) {
        std::cout << "create1: passed" << std::endl;
    }
    else {
        std::cout << "create1: fail" << std::endl;
    }

    delete v1;
    delete v2;
    delete compact;
}

void create2(ILogger *logger){
    double tolerance = 1e-5;

    double a1[] = { 1, 1 };
    double a2[] = { 2, 2, 2};

    IVector* v1 = IVector::createVector(2, a1, logger);
    IVector* v2 = IVector::createVector(3, a2, logger);

    ICompact* compact = ICompact::createCompact(v1, v2, logger);

    if (compact == nullptr) {
        std::cout << "create2: passed" << std::endl;
    }
    else {
        std::cout << "create2: fail" << std::endl;
    }

    delete v1;
    delete v2;
}

void create3(ILogger *logger){
    ICompact* compact = ICompact::createCompact(nullptr, nullptr, logger);

    if (compact == nullptr) {
        std::cout << "create3: passed" << std::endl;
    }
    else {
        std::cout << "create3: fail" << std::endl;
    }
}

void isSubSet1(ILogger *logger){
    double a1[] = { 1, 1 };
    double a2[] = { 2, 2 };
    double a3[] = { 3, 3 };
    double a4[] = { 4, 4 };

    IVector* v1 = IVector::createVector(2, a1, logger);
    IVector* v2 = IVector::createVector(2, a2, logger);
    IVector* v3 = IVector::createVector(2, a3, logger);
    IVector* v4 = IVector::createVector(2, a4, logger);

    ICompact* c1 = ICompact::createCompact(v1, v4, logger);
    ICompact* c2 = ICompact::createCompact(v2, v3, logger);

    bool res = false;
    RESULT_CODE r = c1->isSubSet(c2,res);

    if (r == RESULT_CODE::SUCCESS && res) {
        std::cout << "isSubSet1: passed" << std::endl;
    }
    else {
        std::cout << "isSubSet1: fail" << std::endl;
    }

    delete v1;
    delete v2;
    delete v3;
    delete v4;
    delete c1;
    delete c2;
}

void isSubSet2(ILogger *logger){
    double a1[] = { 1, 1 };
    double a2[] = { 2, 2 };
    double a3[] = { 3, 3 };
    double a4[] = { 4, 4 };

    IVector* v1 = IVector::createVector(2, a1, logger);
    IVector* v2 = IVector::createVector(2, a2, logger);
    IVector* v3 = IVector::createVector(2, a3, logger);
    IVector* v4 = IVector::createVector(2, a4, logger);

    ICompact* c1 = ICompact::createCompact(v1, v2, logger);
    ICompact* c2 = ICompact::createCompact(v3, v4, logger);

    bool res = false;
    RESULT_CODE r = c1->isSubSet(c2,res);

    if (r == RESULT_CODE::SUCCESS && !res) {
        std::cout << "isSubSet2: passed" << std::endl;
    }
    else {
        std::cout << "isSubSet2: fail" << std::endl;
    }

    delete v1;
    delete v2;
    delete v3;
    delete v4;
    delete c1;
    delete c2;
}

void isIntersect1(ILogger *logger){
    double a1[] = { 1, 1 };
    double a2[] = { 2, 2 };
    double a3[] = { 3, 3 };
    double a4[] = { 4, 4 };

    IVector* v1 = IVector::createVector(2, a1, logger);
    IVector* v2 = IVector::createVector(2, a2, logger);
    IVector* v3 = IVector::createVector(2, a3, logger);
    IVector* v4 = IVector::createVector(2, a4, logger);

    ICompact* c1 = ICompact::createCompact(v1, v3, logger);
    ICompact* c2 = ICompact::createCompact(v2, v4, logger);

    bool res = false;
    RESULT_CODE r = c1->isIntersects(c2,res);

    if (r == RESULT_CODE::SUCCESS && res) {
        std::cout << "IsIntersect1: passed" << std::endl;
    }
    else {
        std::cout << "IsIntersect1: fail" << std::endl;
    }

    delete v1;
    delete v2;
    delete v3;
    delete v4;
    delete c1;
    delete c2;
}

void isIntersect2(ILogger *logger){
    double a1[] = { 1, 1 };
    double a2[] = { 2, 2 };
    double a3[] = { 3, 3 };
    double a4[] = { 4, 4 };

    IVector* v1 = IVector::createVector(2, a1, logger);
    IVector* v2 = IVector::createVector(2, a2, logger);
    IVector* v3 = IVector::createVector(2, a3, logger);
    IVector* v4 = IVector::createVector(2, a4, logger);

    ICompact* c1 = ICompact::createCompact(v1, v2, logger);
    ICompact* c2 = ICompact::createCompact(v3, v4, logger);

    bool res = false;
    RESULT_CODE r = c1->isIntersects(c2,res);

    if (r == RESULT_CODE::SUCCESS && !res) {
        std::cout << "IsIntersect2: passed" << std::endl;
    }
    else {
        std::cout << "IsIntersect2: fail" << std::endl;
    }

    delete v1;
    delete v2;
    delete v3;
    delete v4;
    delete c1;
    delete c2;
}

void isIntersect3(ILogger *logger){
    double a1[] = { 1, 1 };
    double a2[] = { 2, 2 };
    double a3[] = { 3, 3, 3};
    double a4[] = { 4, 4, 4};

    IVector* v1 = IVector::createVector(2, a1, logger);
    IVector* v2 = IVector::createVector(2, a2, logger);
    IVector* v3 = IVector::createVector(3, a3, logger);
    IVector* v4 = IVector::createVector(3, a4, logger);

    ICompact* c1 = ICompact::createCompact(v1, v2, logger);
    ICompact* c2 = ICompact::createCompact(v3, v4, logger);

    bool res = false;
    RESULT_CODE r = c1->isIntersects(c2,res);

    if (r != RESULT_CODE::SUCCESS && !res) {
        std::cout << "IsIntersect3: passed" << std::endl;
    }
    else {
        std::cout << "IsIntersect3: fail" << std::endl;
    }

    delete v1;
    delete v2;
    delete v3;
    delete v4;
    delete c1;
    delete c2;
}

void intersection1(ILogger *logger){
    double a1[] = { 1, 1 };
    double a2[] = { 2, 2 };
    double a3[] = { 3, 3, 3};
    double a4[] = { 4, 4, 4};

    IVector* v1 = IVector::createVector(2, a1, logger);
    IVector* v2 = IVector::createVector(2, a2, logger);
    IVector* v3 = IVector::createVector(3, a3, logger);
    IVector* v4 = IVector::createVector(3, a4, logger);

    ICompact* c1 = ICompact::createCompact(v1, v2, logger);
    ICompact* c2 = ICompact::createCompact(v3, v4, logger);

    ICompact* r = ICompact::intersection(c1, c2, logger);

    if (r == nullptr) {
        std::cout << "intersection1: passed" << std::endl;
    }
    else {
        std::cout << "intersection1: fail" << std::endl;
        delete r;
    }

    delete v1;
    delete v2;
    delete v3;
    delete v4;
    delete c1;
    delete c2;
}

void intersection2(ILogger *logger){
    double tolerance = 1e-5;

    double a1[] = { 1, 1 };
    double a2[] = { 2, 2 };
    double a3[] = { 3, 3};
    double a4[] = { 4, 4};

    IVector* v1 = IVector::createVector(2, a1, logger);
    IVector* v2 = IVector::createVector(2, a2, logger);
    IVector* v3 = IVector::createVector(2, a3, logger);
    IVector* v4 = IVector::createVector(2, a4, logger);

    ICompact* c1 = ICompact::createCompact(v1, v4, logger);
    ICompact* c2 = ICompact::createCompact(v3, v4, logger);

    ICompact* r = ICompact::intersection(c1, c2, logger);


    if (r != nullptr) {
        IVector* v5 = r->getBegin();
        IVector* v6 = r->getEnd();
        bool r1, r2;
        IVector::equals(v3, v5, IVector::NORM::NORM_2, tolerance, &r1, logger);
        IVector::equals(v4, v6, IVector::NORM::NORM_2, tolerance, &r2, logger);
        if (r1 && r2){
            std::cout << "intersection1: passed" << std::endl;
        }
        else{
            std::cout << "intersection1: fail - the constructed compact does not meet the expected" << std::endl;
        }

        delete v5;
        delete v6;
    }
    else {
        std::cout << "intersection1: fail - compact was not created" << std::endl;
        delete r;
    }

    delete v1;
    delete v2;
    delete v3;
    delete v4;
    delete c1;
    delete c2;
}

void add1(ILogger *logger){
    double tolerance = 1e-5;

    double a1[] = { 1, 1 };
    double a2[] = { 2, 2 };
    double a3[] = { 1.5, 1};
    double a4[] = { 40, 2};

    IVector* v1 = IVector::createVector(2, a1, logger);
    IVector* v2 = IVector::createVector(2, a2, logger);
    IVector* v3 = IVector::createVector(2, a3, logger);
    IVector* v4 = IVector::createVector(2, a4, logger);

    ICompact* c1 = ICompact::createCompact(v1, v2, logger);
    ICompact* c2 = ICompact::createCompact(v3, v4, logger);

    ICompact* r = ICompact::add(c1, c2, logger);


    if (r != nullptr) {
        IVector* v5 = r->getBegin();
        IVector* v6 = r->getEnd();
        bool r1, r2;
        IVector::equals(v1, v5, IVector::NORM::NORM_2, tolerance, &r1, logger);
        IVector::equals(v4, v6, IVector::NORM::NORM_2, tolerance, &r2, logger);
        if (r1 && r2){
            std::cout << "add1: passed" << std::endl;
        }
        else{
            std::cout << "add1: fail - the constructed compact does not meet the expected" << std::endl;
        }

        delete v5;
        delete v6;
    }
    else {
        std::cout << "add1: fail - compact was not created" << std::endl;
        delete r;
    }

    delete v1;
    delete v2;
    delete v3;
    delete v4;
    delete c1;
    delete c2;
}

void add2(ILogger *logger){
    double tolerance = 1e-5;

    double a1[] = { 1, 1 };
    double a2[] = { 2, 2 };
    double a3[] = { 3, 3};
    double a4[] = { 4, 5};

    IVector* v1 = IVector::createVector(2, a1, logger);
    IVector* v2 = IVector::createVector(2, a2, logger);
    IVector* v3 = IVector::createVector(2, a3, logger);
    IVector* v4 = IVector::createVector(2, a4, logger);

    ICompact* c1 = ICompact::createCompact(v1, v2, logger);
    ICompact* c2 = ICompact::createCompact(v3, v4, logger);

    ICompact* r = ICompact::add(c1, c2, logger);


    if (r == nullptr) {
        std::cout << "add2: passed" << std::endl;
    }
    else {
        std::cout << "add2: fail - compact should not created" << std::endl;
        delete r;
    }

    delete v1;
    delete v2;
    delete v3;
    delete v4;
    delete c1;
    delete c2;
}

int main(){
    ILogger *l = ILogger::createLogger(nullptr);
    create1(l);
    create2(l);
    create3(l);
    isSubSet1(l);
    isSubSet2(l);
    isIntersect1(l);
    isIntersect2(l);
    isIntersect3(l);
    intersection1(l);
    intersection2(l);
    add1(l);
    add2(l);

    l->destroyLogger(nullptr);
}
