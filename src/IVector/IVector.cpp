//
// Created by user on 12.06.2020.
//
#define _EXPORTING
#include <cstring>
#include "../../include/IVector.h"
#include "IVector_Impl.cpp"

IVector * IVector::createVector(size_t dim, double *pData, ILogger *pLogger) {
    if (!pData){
        if (pLogger){
            pLogger->log("In IVector::createVector - get nullptr as pData", RESULT_CODE::WRONG_ARGUMENT);
        }
        return nullptr;
    }

    if (!dim){
        if (pLogger){
            pLogger->log("In IVector::createVector - get 0 as dim", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }

    for (size_t i = 0; i < dim; i++){
        if (std::isnan(pData[i])){
            if (pLogger){
                pLogger->log("In IVector::createVector - one of coordinate is nan", RESULT_CODE::WRONG_ARGUMENT);
            }
            return nullptr;
        }
    }

    auto *data = new (std::nothrow)double[dim];

    if(!data){
        if (pLogger){
            pLogger->log("In IVector::createVector - memory allocation problems", RESULT_CODE::OUT_OF_MEMORY);
        }
        return nullptr;
    }

    memcpy(data, pData, dim * sizeof(double));

    IVector* result = new (std::nothrow)IVector_Impl(dim, data);
    if (!result){
        if (pLogger){
            pLogger->log("In IVector::createVector - memory allocation problems", RESULT_CODE::OUT_OF_MEMORY);
        }
        return nullptr;
    }

    return result;
} // IVector::createVector

IVector *IVector::add(IVector const *pOperand1, IVector const *pOperand2, ILogger *pLogger) {
    if (!pOperand1){
        if (pLogger){
            pLogger->log("In IVector::add - first operand is nullptr", RESULT_CODE::WRONG_ARGUMENT);
        }
        return nullptr;
    }

    if (!pOperand2) {
        if (pLogger) {
            pLogger->log("In IVector::add - second operand is nullptr", RESULT_CODE::WRONG_ARGUMENT);
        }
        return nullptr;
    }

    if (pOperand1->getDim() != pOperand2->getDim()){
        if (pLogger) {
            pLogger->log("In IVector::add - mismatch of operand size", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }

    size_t dim = pOperand1->getDim();
    auto *data = new (std::nothrow)double[dim];

    if (!data){
        if (pLogger) {
            pLogger->log("In IVector::add - memory allocation problems", RESULT_CODE::OUT_OF_MEMORY);
        }
        return nullptr;
    }

    for (size_t i = 0; i < dim; i++){
        data[i] = pOperand1->getCoord(i) + pOperand2->getCoord(i);
    }

    IVector* result = new (std::nothrow)IVector_Impl(dim, data);

    if (!result){
        if (pLogger){
            pLogger->log("In IVector::add - memory allocation problems", RESULT_CODE::OUT_OF_MEMORY);
        }
        return nullptr;
    }

    return result;
} // IVector::add

IVector *IVector::sub(IVector const *pOperand1, IVector const *pOperand2, ILogger *pLogger) {
    if (!pOperand1){
        if (pLogger){
            pLogger->log("In IVector::sub - first operand is nullptr", RESULT_CODE::WRONG_ARGUMENT);
        }
        return nullptr;
    }

    if (!pOperand2) {
        if (pLogger) {
            pLogger->log("In IVector::sub - second operand is nullptr", RESULT_CODE::WRONG_ARGUMENT);
        }
        return nullptr;
    }

    if (pOperand1->getDim() != pOperand2->getDim()){
        if (pLogger) {
            pLogger->log("In IVector::sub - mismatch of operand dimension", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }

    size_t dim = pOperand1->getDim();
    auto *data = new (std::nothrow)double[dim];

    if (!data){
        if (pLogger) {
            pLogger->log("In IVector::sub - memory allocation problems", RESULT_CODE::OUT_OF_MEMORY);
        }
        return nullptr;
    }

    for (size_t i = 0; i < dim; i++){
        data[i] = pOperand1->getCoord(i) - pOperand2->getCoord(i);
    }

    IVector* result = new (std::nothrow)IVector_Impl(dim, data);

    if (!result){
        if (pLogger){
            pLogger->log("In IVector::sub - memory allocation problems", RESULT_CODE::OUT_OF_MEMORY);
        }
        return nullptr;
    }

    return result;
} // IVector::sub

IVector *IVector::mul(IVector const *pOperand1, double scaleParam, ILogger *pLogger) {
    if (!pOperand1){
        if (pLogger){
            pLogger->log("In IVector::mul scalar - first operand is nullptr", RESULT_CODE::WRONG_ARGUMENT);
        }
        return nullptr;
    }

    size_t dim = pOperand1->getDim();
    auto *data = new (std::nothrow)double[dim];

    if (!data){
        if (pLogger) {
            pLogger->log("In IVector::mull scalar - memory allocation problems", RESULT_CODE::OUT_OF_MEMORY);
        }
        return nullptr;
    }

    if (std::isnan(scaleParam)){
        if (pLogger) {
            pLogger->log("In IVector::mull scalar - scale param is nan", RESULT_CODE::WRONG_ARGUMENT);
        }
        return nullptr;
    }

    for (size_t i = 0; i < dim; i++){
        data[i] = pOperand1->getCoord(i) * scaleParam;
    }

    IVector* result = new (std::nothrow)IVector_Impl(dim, data);

    if (!result){
        if (pLogger){
            pLogger->log("In IVector::mull scalar - memory allocation problems", RESULT_CODE::OUT_OF_MEMORY);
        }
        return nullptr;
    }

    return result;
} // IVector::mul

double IVector::mul(IVector const *pOperand1, IVector const *pOperand2, ILogger *pLogger) {
    if (!pOperand1){
        if (pLogger){
            pLogger->log("In IVector::mull vector - first operand is nullptr", RESULT_CODE::WRONG_ARGUMENT);
        }
        return NAN;
    }

    if (!pOperand2) {
        if (pLogger) {
            pLogger->log("In IVector::mull vector - second operand is nullptr", RESULT_CODE::WRONG_ARGUMENT);
        }
        return NAN;
    }

    if (pOperand1->getDim() != pOperand2->getDim()){
        if (pLogger) {
            pLogger->log("In IVector::mull vector - mismatch of operand size", RESULT_CODE::WRONG_DIM);
        }
        return NAN;
    }

    double result = 0;
    for (size_t i = 0, dim = pOperand1->getDim(); i < dim; i++){
        result += pOperand1->getCoord(i) * pOperand2->getCoord(i);
    }

    return result;
} // IVector::mul

RESULT_CODE
IVector::equals(IVector const *pOperand1, IVector const *pOperand2, IVector::NORM norm, double tolerance, bool *result,
                ILogger *pLogger) {
    if (!pOperand1){
        if (pLogger){
            pLogger->log("In IVector::equals - first operand is nullptr", RESULT_CODE::WRONG_ARGUMENT);
        }

        return RESULT_CODE::WRONG_ARGUMENT;
    }

    if (!pOperand2){
        if (pLogger){
            pLogger->log("In IVector::equals - second operand is nullptr", RESULT_CODE::WRONG_ARGUMENT);
        }

        return RESULT_CODE::WRONG_ARGUMENT;
    }

    if (tolerance < 0){
        return RESULT_CODE::WRONG_ARGUMENT;
    }

    if (pOperand1->getDim() != pOperand2->getDim()){
        if (pLogger){
            pLogger->log("In IVector::equals - mismatch of operand dimensions", RESULT_CODE::WRONG_DIM);
        }

        return RESULT_CODE::WRONG_ARGUMENT;
    }

    IVector *diff = IVector::sub(pOperand1, pOperand2, pLogger);
    if (!diff){
        if (pLogger){
            pLogger->log("In IVector::equals - memory allocation problems", RESULT_CODE::OUT_OF_MEMORY);
        }

        return RESULT_CODE::OUT_OF_MEMORY;
    }

    double diff_norm = fabs(diff->norm(norm));
    if (std::isnan(diff_norm)){
        delete diff;
        if (pLogger){
            pLogger->log("In IVector::equals - error while calculating vector's norm", RESULT_CODE::CALCULATION_ERROR);
        }

        return RESULT_CODE::CALCULATION_ERROR;
    }

    *result = diff_norm < tolerance;
    delete diff;

    return RESULT_CODE::SUCCESS;
} // IVector::equals

IVector::~IVector()
= default;