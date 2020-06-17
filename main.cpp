#include <IVector.h>
#include "ISet.h"
#include <iostream>

int main(){
    auto *a = new double[20]{1};
    a[0]=1;
    IVector*va = IVector::createVector(20, a, nullptr);
//    std::cout << "dim:"<< va->getDim();
    ISet *s = ISet::createSet(nullptr);
    s->insert(va,IVector::NORM::NORM_1, 1);
    IVector *t = nullptr;
    s->get(t, 0);
    std::cout << t->getDim();
    delete va;
}