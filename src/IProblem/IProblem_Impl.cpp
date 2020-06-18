#include <cmath>
#include <iostream>
#include "IProblem.h"
#include "IBrocker.h"

namespace  {
    class IProblem_Impl: public IProblem{
    private:
        size_t _argsNum;
        size_t _paramsNum;
        IVector *_params;
        ILogger *_logger;
        static bool isCircleToRect(int cx, int cy, int radius,  int rx, int ry, int width, int height);
        // IProblem interface
    public:
        RESULT_CODE goalFunction(const IVector *args, const IVector *params, double &res) const;
        RESULT_CODE goalFunctionByArgs(const IVector *args, double &res) const;
        size_t getArgsDim() const;
        size_t getParamsDim() const;
        RESULT_CODE setParams(const IVector *params);
        bool isCompactValid(const ICompact * const &compact) const;
        IProblem_Impl();
        ~IProblem_Impl();
    };

    class IBrocker_Impl: public IBrocker{
        void* _problem;
    private:
        // IBrocker interface
    public:
        IBrocker_Impl(void *problem);
        Type getType() const;
        void *getInterfaceImpl(Type type) const;
        void release();
    };

    RESULT_CODE IProblem_Impl::goalFunction(const IVector *args, const IVector *params, double &res) const{
        if (!args || !params){
            if (_logger){
                _logger->log("In IProblem::goalFunction - args or params are nullptr", RESULT_CODE::BAD_REFERENCE);
            }
            return RESULT_CODE::BAD_REFERENCE;
        }
        if (args->getDim() != _argsNum || params->getDim() != _paramsNum){
            if (_logger){
                _logger->log("In IProblem::goalFunction - args or params dimension does not meet expected", RESULT_CODE::WRONG_DIM);
            }
            return RESULT_CODE::WRONG_DIM;
        }
        double x = args->getCoord(0);
        double y = args->getCoord(1);
        double a = params->getCoord(0);
        double b = params->getCoord(1);
        double r = params->getCoord(2);
        double result = r*r - (x-a)*(x-a) - (y-b) * (y-b);
        if (result < 0){
            res = NAN;
            return RESULT_CODE::WRONG_ARGUMENT;
        }
        res =  sqrt(result);
        return RESULT_CODE::SUCCESS;
    }

    RESULT_CODE IProblem_Impl::goalFunctionByArgs(const IVector *args, double &res) const{
        if (!args){
            if (_logger){
                _logger->log("In IProblem::goalFunctionByArgs - args are nullptr", RESULT_CODE::BAD_REFERENCE);
            }
            return RESULT_CODE::BAD_REFERENCE;
        }
        if (args->getDim() != _argsNum){
            return RESULT_CODE::WRONG_DIM;
        }
        if (_params == nullptr){
            res = NAN;
            if (_logger){
                _logger->log("In IProblem::goalFunctionByArgs - params not set", RESULT_CODE::NAN_VALUE);
            }
            return RESULT_CODE::NAN_VALUE;
        }
        double x = args->getCoord(0);
        double y = args->getCoord(1);
        double a = _params->getCoord(0);
        double b = _params->getCoord(1);
        double r = _params->getCoord(2);
        double result = r*r - (x-a)*(x-a) - (y-b) * (y-b);
        if (result < 0){
            res = NAN;
            return RESULT_CODE::WRONG_ARGUMENT;
        }
        res = sqrt(result);
        return RESULT_CODE::SUCCESS;
    }

    size_t IProblem_Impl::getArgsDim() const{
        return _argsNum;
    }

    size_t IProblem_Impl::getParamsDim() const {
        return _paramsNum;
    }

    RESULT_CODE IProblem_Impl::setParams(const IVector *params){
        if (!params){
            _logger->log("In IProblem::setParams - args or params dimension does not meet expected", RESULT_CODE::BAD_REFERENCE);
            return RESULT_CODE::BAD_REFERENCE;
        }

        IVector *clone_params = params->clone();
        if (!clone_params){
            if (_logger){
                _logger->log("In IProblem::setParams - args or params dimension does not meet expected", RESULT_CODE::OUT_OF_MEMORY);
            }
            return RESULT_CODE::OUT_OF_MEMORY;
        }

        if (_params!= nullptr){
            delete _params;
            _params = nullptr;
        }
        _params = clone_params;
        return RESULT_CODE::SUCCESS;
    }

    bool IProblem_Impl::isCompactValid(const ICompact * const &compact) const{
        if (!compact){
            if (_logger){
                _logger->log("In IProblem::isCompactValid - compact is nullptr", RESULT_CODE::BAD_REFERENCE);
            }
            return false;
        }
        if (!_params){
            return false;
        }
        if (compact->getDim() != _argsNum){
            if (_logger){
                _logger->log("In IProblem::isCompactValid - dimension of compact is not equal to dimentional of problem", RESULT_CODE::WRONG_DIM);
            }
            return false;
        }

        IVector *beg = compact->getBegin();
        if (!beg){
            if (_logger){
                _logger->log("In IProblem::isCompactValid - memory error", RESULT_CODE::OUT_OF_MEMORY);
            }
            return false;
        }
        IVector *end= compact->getEnd();
        if (!end){
            delete beg;
            if (_logger){
                _logger->log("In IProblem::isCompactValid - memory error", RESULT_CODE::OUT_OF_MEMORY);
            }
            return false;
        }
        double a = _params->getCoord(0);
        double b = _params->getCoord(1);
        double r = _params->getCoord(2);
        double x1 = beg->getCoord(0);
        double x2 = end->getCoord(0);
        double y1 = beg->getCoord(1);
        double y2 = beg->getCoord(1);

        delete beg;
        delete end;

        // ld corner
        if ((x1-a)*(x1-a) + (y1-b)*(y1-b) > r*r ||
            (x1-a)*(x1-a) + (y2-b)*(y2-b) > r*r ||
            (x2-a)*(x2-a) + (y1-b)*(y1-b) > r*r ||
            (x2-a)*(x2-a) + (y2-b)*(y2-b) > r*r){
            if (_logger){
                _logger->log("In IProblem::isCompactValid - invalid compact", RESULT_CODE::OUT_OF_BOUNDS);
            }
            return false;
        }
        return true;
    }

    IProblem_Impl::~IProblem_Impl(){
        delete _params;
        _logger->destroyLogger(this);
    }

    bool IProblem_Impl::isCircleToRect(int cx, int cy, int radius, int rx, int ry, int width, int height){
        int x = cx;
        int y = cy;

        if(cx < rx)
            x = rx;
        else if(cx > (rx + width))
            x = rx + width;

        if(cy < ry)
            y = ry;
        else if(cy > (ry + height))
            y = ry + height;

        return (((cx - x)*(cx - x) + (cy - y)*(cy - y)) <= (radius * radius));
    }

    IProblem_Impl::IProblem_Impl():
        _argsNum(2),
        _paramsNum(3),
        _params(nullptr),
        _logger(ILogger::createLogger(this))
    {}

    IBrocker_Impl::IBrocker_Impl(void *problem):
        _problem(problem)
    {}

    IBrocker::Type IBrocker_Impl::getType() const{
        return Type::PROBLEM;
    }

    void *IBrocker_Impl::getInterfaceImpl(IBrocker::Type type) const{
        if (type == Type::PROBLEM){
            return _problem;
        }
        else {
            return nullptr;
        }
    }

    void IBrocker_Impl::release(){
        delete reinterpret_cast<IProblem_Impl*>(_problem);
    }
}
void* getBrocker() {
    void *problem = new IProblem_Impl();
    if (problem){
        void *brocker = new IBrocker_Impl(problem);
        if (brocker){
            return brocker;
        }
        delete reinterpret_cast<IProblem_Impl*>(problem);
    }

    return nullptr;
}

