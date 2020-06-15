#include <ISet.h>
#include "ISet_Impl.cpp"

ISet *ISet::createSet(ILogger *pLogger) {
    auto new_set = new(std::nothrow)ISet_Impl(pLogger);
    if (!new_set){
        if (pLogger){
            pLogger->log("In ISet::createSet - could not create set",RESULT_CODE::OUT_OF_MEMORY);
        }
    }

    return new_set;
} // ISet::createSet

ISet *ISet::add(ISet const *pOperand1, ISet const *pOperand2, IVector::NORM norm, double tolerance, ILogger *pLogger) {
    if (!pOperand1){
        if (pLogger){
            pLogger->log("In ISet::add - first operand is nullptr", RESULT_CODE::WRONG_ARGUMENT);
        }
        return nullptr;
    }

    if (!pOperand2){
        if (pLogger){
            pLogger->log("In ISet::add - second operand is nullptr", RESULT_CODE::WRONG_ARGUMENT);
        }
        return nullptr;
    }

    if (tolerance < 0){
        if (pLogger){
            pLogger->log("In ISet::add - second operand is nullptr", RESULT_CODE::WRONG_ARGUMENT);
        }
        return nullptr;
    }

    if (!pOperand1->getDim()){
        if (pLogger){
            pLogger->log("In ISet::add - first operand has zero dimension", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }

    if (!pOperand2->getDim()){
        if (pLogger){
            pLogger->log("In ISet::add - second operand has zero dimension", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }

    if (pOperand1->getDim() != pOperand2->getDim()){
        if (pLogger){
            pLogger->log("In ISet::add - the dimensions of the sets do not match", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }

    auto union_set = pOperand1->clone();
    if (!union_set){
        if (pLogger){
            pLogger->log("In ISet::add - could not create union of set", RESULT_CODE::OUT_OF_MEMORY);
        }
        return nullptr;
    }

    IVector *vec = nullptr;
    for (size_t i = 0, size_of_second = pOperand2->getSize(); i < size_of_second; i++){
        switch (pOperand2->get(vec, i)){
            case RESULT_CODE::SUCCESS:
                if (!vec){
                    if(pLogger){
                        pLogger->log("In ISet::add - error while copy second set", RESULT_CODE::OUT_OF_MEMORY);
                    }
                    delete vec;
                    vec = nullptr;
                }
                if ((union_set->insert(vec, norm, tolerance)) == RESULT_CODE::OUT_OF_MEMORY){
                    if(pLogger){
                        pLogger->log("In ISet::add - error while copy second set", RESULT_CODE::OUT_OF_MEMORY);
                    }
                    delete vec;
                    vec = nullptr;
                }
                break;
            case RESULT_CODE::OUT_OF_MEMORY:
                if(pLogger){
                    pLogger->log("In ISet::add - error while copy second set", RESULT_CODE::OUT_OF_MEMORY);
                }
                delete vec;
                vec = nullptr;
                break;
            default:
                break;
        }
    }

    return union_set;
} // ISet::add

ISet *
ISet::intersect(ISet const *pOperand1, ISet const *pOperand2, IVector::NORM norm, double tolerance, ILogger *pLogger) {
    if (!pOperand1){
        if (pLogger){
            pLogger->log("In ISet::intersect - first operand is nullptr", RESULT_CODE::WRONG_ARGUMENT);
        }
        return nullptr;
    }

    if (!pOperand2){
        if (pLogger){
            pLogger->log("In ISet::intersect - second operand is nullptr", RESULT_CODE::WRONG_ARGUMENT);
        }
        return nullptr;
    }

    if (tolerance < 0){
        if (pLogger){
            pLogger->log("In ISet::intersect - second operand is nullptr", RESULT_CODE::WRONG_ARGUMENT);
        }
        return nullptr;
    }

    if (!pOperand1->getDim()){
        if (pLogger){
            pLogger->log("In ISet::intersect - first operand has zero dimension", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }

    if (!pOperand2->getDim()){
        if (pLogger){
            pLogger->log("In ISet::intersect - second operand has zero dimension", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }

    if (pOperand1->getDim() != pOperand2->getDim()){
        if (pLogger){
            pLogger->log("In ISet::intersect - the dimensions of the sets do not match", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }

    auto intersect_set = createSet(pLogger);
    if (!intersect_set){
        if (pLogger){
            pLogger->log("In ISet::add - could not create union of set", RESULT_CODE::OUT_OF_MEMORY);
        }
        return nullptr;
    }

    IVector *vec = nullptr;
    for (size_t i = 0, size_of_second = pOperand2->getSize(); i < size_of_second; i++){
        if (pOperand1->get(vec, i) == RESULT_CODE::SUCCESS) {
            if (!vec){
                if(pLogger){
                    pLogger->log("In ISet::intersect - error while finding intersections", RESULT_CODE::OUT_OF_MEMORY);
                }
                delete vec;
                vec = nullptr;
            }

            IVector * vec2 = nullptr;
            if (pOperand2->get(vec2, vec, norm, tolerance) == RESULT_CODE::SUCCESS){
                delete vec2;
                vec2 = nullptr;

                intersect_set->insert(vec, norm, tolerance);

                delete vec;
                vec = nullptr;
            }
            else{
                if(pLogger){
                    pLogger->log("In ISet::intersect - error while finding intersections", RESULT_CODE::OUT_OF_MEMORY);
                }
            }
            delete vec;
            vec = nullptr;
        }
    }

    return intersect_set;
} // ISet::intersect

ISet *ISet::sub(ISet const *pOperand1, ISet const *pOperand2, IVector::NORM norm, double tolerance, ILogger *pLogger) {
    return nullptr;
} // ISet::sub

ISet *
ISet::symSub(ISet const *pOperand1, ISet const *pOperand2, IVector::NORM norm, double tolerance, ILogger *pLogger) {
    return nullptr;
} // ISet::symSub

ISet::~ISet()
= default;