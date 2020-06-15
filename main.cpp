//
// Created by user on 12.06.2020.
//

#include <IVector.h>
#include <iostream>

int main(){
    auto *a = new double[1];
    std::cout << "dim 2";
    a[0]=1;
    IVector*va = IVector::createVector(1, a, nullptr);
    std::cout << "dim"<< va->getDim();
    delete va;
}