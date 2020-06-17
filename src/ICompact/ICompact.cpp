#include <cmath>
#include <new>
#include "ICompact.h"
#include "IVector.h"
#include "ICompact_Impl.cpp"

ICompact *ICompact::createCompact(IVector const *const begin, IVector const *const end, ILogger *logger) {
    if (!begin){
        if(logger){
            logger->log("In ICompact::createCompact - begin vector is nullptr", RESULT_CODE::BAD_REFERENCE);
        }
        return nullptr;
    }

    if (!end){
        if(logger){
            logger->log("In ICompact::createCompact - end vector is nullptr", RESULT_CODE::BAD_REFERENCE);
        }
        return nullptr;
    }

    if(begin->getDim() == 0){
        if(logger){
            logger->log("In ICompact::createCompact - begin vector is zero dimension", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }

    if(end->getDim() == 0){
        if(logger){
            logger->log("In ICompact::createCompact - end vector is zero dimension", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }

    if (begin->getDim() != end->getDim()){
        if(logger){
            logger->log("In ICompact::createCompact - end vector is nullptr", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }

    for (size_t i = 0, dim = begin->getDim(); i < dim; i++){
        if (begin->getCoord(i) > end->getCoord(i)){
            if(logger){
                logger->log("In ICompact::createCompact - one of begin coordinates is greater than or equal to end coordinates", RESULT_CODE::WRONG_ARGUMENT);
            }
            return nullptr;
        }
    }

    IVector* clone_beg = begin->clone();
    IVector* clone_end = end->clone();
    if (!clone_beg || !clone_end){
        if(logger){
            logger->log("In ICompact::createCompact - error while creating compact", RESULT_CODE::OUT_OF_MEMORY);
        }
        return nullptr;
    }

    ICompact *compact = new(std::nothrow) ICompact_Impl(clone_beg, clone_end, begin->getDim(), logger);

    return compact;
} // ICompact::createCompact

ICompact *ICompact::intersection(ICompact const *const left, ICompact const *const right, ILogger *logger) {
    if (!left){
        if(logger){
            logger->log("In ICompact::intersection - left compact is nullptr", RESULT_CODE::BAD_REFERENCE);
        }
        return nullptr;
    }

    if (!right){
        if(logger){
            logger->log("In ICompact::intersection - right compact is nullptr", RESULT_CODE::BAD_REFERENCE);
        }
        return nullptr;
    }

    if (!left->getDim()){
        if(logger){
            logger->log("In ICompact::intersection - left compact has zero dimension", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }

    if (!right->getDim()){
        if(logger){
            logger->log("In ICompact::intersection - right compact has zero dimension", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }

    if (left->getDim() != right->getDim()){
        if(logger){
            logger->log("In ICompact::intersection - mismatch compact dimension", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }

    bool isIntersect = false;
    RESULT_CODE rc = left->isIntersects(right, isIntersect);
    if (rc != RESULT_CODE::SUCCESS){
        if(logger){
            logger->log("In ICompact::intersection - error while checking intersection", rc);
        }
        return nullptr;
    }

    if (!isIntersect){
        return nullptr;
    }

    auto leftBeg = left->getBegin();
    if (!leftBeg){
        if(logger){
            logger->log("In ICompact::intersection - memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        return nullptr;
    }

    auto rightBeg = right->getBegin();
    if (!rightBeg){
        if(logger){
            logger->log("In ICompact::intersection - memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        delete leftBeg;
        leftBeg = nullptr;
        return nullptr;

    }

    auto leftEnd = left->getEnd();
    if (!leftEnd){
        if(logger){
            logger->log("In ICompact::intersection - memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        delete rightBeg;
        delete leftBeg;
        leftBeg = nullptr;
        rightBeg = nullptr;
        return nullptr;
    }

    auto rightEnd = right->getEnd();
    if (!rightEnd){
        if(logger){
            logger->log("In ICompact::intersection - memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        delete rightBeg;
        delete leftBeg;
        delete leftEnd;
        leftBeg = nullptr;
        rightBeg = nullptr;
        leftEnd = nullptr;
        return nullptr;
    }

    size_t dim = left->getDim();
    auto data_beg = new double[dim];
    auto data_end = new double[dim];

    for(size_t i = 0; i < dim; i++){
        data_beg[i] = fmax(leftBeg->getCoord(i), rightBeg->getCoord(i));
        data_end[i] = fmin(leftEnd->getCoord(i), rightEnd->getCoord(i));
    }

    delete leftBeg;
    delete rightBeg;
    delete leftEnd;
    delete rightEnd;
    leftBeg = nullptr;
    rightBeg = nullptr;
    leftEnd = nullptr;
    rightEnd = nullptr;

    IVector* i_beg = IVector::createVector(dim, data_beg, logger);
    if (!i_beg){
        if(logger){
            logger->log("In ICompact::intersection - memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        delete[] data_beg;
        delete[] data_end;
        return nullptr;
    }

    IVector* i_end = IVector::createVector(dim, data_end, logger);
    if (!i_end){
        if(logger){
            logger->log("In ICompact::intersection - memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        delete i_beg;
        delete[] data_beg;
        delete[] data_end;
        return nullptr;
    }

    ICompact* res = createCompact(i_beg, i_end, logger);

    delete i_beg;
    delete i_end;

    return res;
} // ICompact::intersection

ICompact *ICompact::add(ICompact const *const left, ICompact const *const right, ILogger *logger) {
    double tolerance = 1e-5;
    if (!left){
        if(logger){
            logger->log("In ICompact::intersection - left compact is nullptr", RESULT_CODE::BAD_REFERENCE);
        }
        return nullptr;
    }

    if (!right){
        if(logger){
            logger->log("In ICompact::intersection - right compact is nullptr", RESULT_CODE::BAD_REFERENCE);
        }
        return nullptr;
    }

    if (!left->getDim()){
        if(logger){
            logger->log("In ICompact::intersection - left compact has zero dimension", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }

    if (!right->getDim()){
        if(logger){
            logger->log("In ICompact::intersection - right compact has zero dimension", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }

    if (left->getDim() != right->getDim()){
        if(logger){
            logger->log("In ICompact::intersection - mismatch compact dimension", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }

    bool isSub = false;
    RESULT_CODE rc = left->isSubSet(right, isSub);
    if (rc != RESULT_CODE::SUCCESS){
        if(logger){
            logger->log("In ICompact::makeConvex - error checking the possibility of add", rc);
        }
        return nullptr;
    }
    if (isSub){
        return left->clone();
    }

    isSub = false;
    rc = right->isSubSet(left, isSub);
    if (rc != RESULT_CODE::SUCCESS){
        if(logger){
            logger->log("In ICompact::makeConvex - error checking the possibility of add", rc);
        }
        return nullptr;
    }
    if (isSub){
        return right->clone();
    }

    auto leftBeg = left->getBegin();
    if (!leftBeg){
        if(logger){
            logger->log("In ICompact::intersection - memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        return nullptr;
    }

    auto rightBeg = right->getBegin();
    if (!rightBeg){
        if(logger){
            logger->log("In ICompact::intersection - memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        delete leftBeg;
        leftBeg = nullptr;
        return nullptr;

    }

    auto leftEnd = left->getEnd();
    if (!leftEnd){
        if(logger){
            logger->log("In ICompact::intersection - memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        delete rightBeg;
        delete leftBeg;
        leftBeg = nullptr;
        rightBeg = nullptr;
        return nullptr;
    }

    auto rightEnd = right->getEnd();
    if (!rightEnd){
        if(logger){
            logger->log("In ICompact::intersection - memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        delete rightBeg;
        delete leftBeg;
        delete leftEnd;
        leftBeg = nullptr;
        rightBeg = nullptr;
        leftEnd = nullptr;
        return nullptr;
    }

    // check possibility of add with makeConvex
    size_t diff_ind = -1;
    for (size_t i = 0, dim = right->getDim(); i < dim; i++){
        if (fabs(leftBeg->getCoord(i) - rightBeg->getCoord(i)) > tolerance &&
            fabs(leftEnd->getCoord(i) - rightEnd->getCoord(i)) > tolerance){
            if (diff_ind == -1){
                diff_ind = i;
            }
            else {
                if(logger){
                    logger->log("In ICompact::intersection - memory error", RESULT_CODE::OUT_OF_MEMORY);
                }
                delete rightBeg;
                delete leftBeg;
                delete leftEnd;
                delete rightEnd;
                leftBeg = nullptr;
                rightBeg = nullptr;
                leftEnd = nullptr;
                rightEnd = nullptr;
                return nullptr;
            }
        }
    }

    rc = leftBeg->setCoord(diff_ind, fmin(leftBeg->getCoord(diff_ind), rightBeg->getCoord(diff_ind)));
    if (rc != RESULT_CODE::SUCCESS){
        if(logger){
            logger->log("In ICompact::intersection - an error while performing the construction of the compact",rc);
        }
        delete rightBeg;
        delete leftBeg;
        delete leftEnd;
        delete rightEnd;
        leftBeg = nullptr;
        rightBeg = nullptr;
        leftEnd = nullptr;
        rightEnd = nullptr;
        return nullptr;
    }
    rc = leftEnd->setCoord(diff_ind, fmax(leftEnd->getCoord(diff_ind), rightEnd->getCoord(diff_ind)));
    if (rc != RESULT_CODE::SUCCESS){
        if(logger){
            logger->log("In ICompact::intersection - an error while performing the construction of the compact",rc);
        }
        delete rightBeg;
        delete leftBeg;
        delete rightEnd;
        delete leftEnd;
        leftBeg = nullptr;
        rightBeg = nullptr;
        leftEnd = nullptr;
        rightEnd = nullptr;
        return nullptr;
    }

    delete rightBeg;
    delete rightEnd;
    rightBeg = nullptr;
    rightEnd = nullptr;

    ICompact* res = createCompact(leftBeg, leftEnd, logger);

    delete leftBeg;
    delete leftEnd;
    leftBeg = nullptr;
    leftEnd = nullptr;

    return res;
} // ICompact::add

ICompact *ICompact::makeConvex(ICompact const *const left, ICompact const *const right, ILogger *logger) {
    if (!left){
        if(logger){
            logger->log("In ICompact::makeConvex - left compact is nullptr", RESULT_CODE::BAD_REFERENCE);
        }
        return nullptr;
    }

    if (!right){
        if(logger){
            logger->log("In ICompact::makeConvex - right compact is nullptr", RESULT_CODE::BAD_REFERENCE);
        }
        return nullptr;
    }

    if (!left->getDim()){
        if(logger){
            logger->log("In ICompact::makeConvex - left compact has zero dimension", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }

    if (!right->getDim()){
        if(logger){
            logger->log("In ICompact::makeConvex - right compact has zero dimension", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }

    if (left->getDim() != right->getDim()){
        if(logger){
            logger->log("In ICompact::makeConvex - mismatch compact dimension", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }

    auto leftBeg = left->getBegin();
    if (!leftBeg){
        if(logger){
            logger->log("In ICompact::intersection - memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        return nullptr;
    }

    auto rightBeg = right->getBegin();
    if (!rightBeg){
        if(logger){
            logger->log("In ICompact::intersection - memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        delete leftBeg;
        leftBeg = nullptr;
        return nullptr;

    }

    auto leftEnd = left->getEnd();
    if (!leftEnd){
        if(logger){
            logger->log("In ICompact::intersection - memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        delete rightBeg;
        delete leftBeg;
        leftBeg = nullptr;
        rightBeg = nullptr;
        return nullptr;
    }

    auto rightEnd = right->getEnd();
    if (!rightEnd){
        if(logger){
            logger->log("In ICompact::intersection - memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        delete rightBeg;
        delete leftBeg;
        delete leftEnd;
        leftBeg = nullptr;
        rightBeg = nullptr;
        leftEnd = nullptr;
        return nullptr;
    }

    size_t dim = left->getDim();
    auto data_beg = new double[dim];
    auto data_end = new double[dim];

    for(size_t i = 0; i < dim; i++){
        data_beg[i] = fmin(leftBeg->getCoord(i), rightBeg->getCoord(i));
        data_end[i] = fmax(leftEnd->getCoord(i), rightEnd->getCoord(i));
    }

    delete leftBeg;
    delete rightBeg;
    delete leftEnd;
    delete rightEnd;
    leftBeg = nullptr;
    rightBeg = nullptr;
    leftEnd = nullptr;
    rightEnd = nullptr;

    IVector* i_beg = IVector::createVector(dim, data_beg, logger);
    if (!i_beg){
        if(logger){
            logger->log("In ICompact::intersection - memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        delete[] data_beg;
        delete[] data_end;
        return nullptr;
    }

    IVector* i_end = IVector::createVector(dim, data_end, logger);
    if (!i_end){
        if(logger){
            logger->log("In ICompact::intersection - memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        delete i_beg;
        delete[] data_beg;
        delete[] data_end;
        return nullptr;
    }

    ICompact* res = createCompact(i_beg, i_end, logger);

    delete i_beg;
    delete i_end;

    return res;
} // ICompact::makeConvex

ICompact::~ICompact()= default;