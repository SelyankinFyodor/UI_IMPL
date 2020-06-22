#include <cmath>
#include <QString>
#include <QStringList>
#include "ISolver.h"
#include "IBrocker.h"

namespace  {
    class ISolvec_Impl : public ISolver{
    private:
        IVector *_params;
        IProblem *_problem;
        ICompact *_compact;
        size_t _paramsNum;
        IVector *_solution;
        ILogger *_logger;
        // ISolver interface
    public:
        RESULT_CODE setParams(const IVector *params);
        RESULT_CODE setParams(QString &str);
        RESULT_CODE setProblem(IProblem *pProblem);
        RESULT_CODE setProblemParams(const IVector *params);
        RESULT_CODE setCompact(ICompact *pCompact);
        size_t getParamsDim() const;
        RESULT_CODE solve();
        RESULT_CODE getSolution(IVector *&vec) const;
        ISolvec_Impl();
        ~ISolvec_Impl();
    };

    class IBrocker_Impl: public IBrocker{
        void* _solver;
    private:
        // IBrocker interface
    public:
        IBrocker_Impl(void *problem);
        Type getType() const;
        void *getInterfaceImpl(Type type) const;
        void release();
    };

    ISolvec_Impl::ISolvec_Impl():
        _params(nullptr),
        _problem(nullptr),
        _compact(nullptr),
        _solution(nullptr),
        _paramsNum(1),
        _logger(ILogger::createLogger(this))
    {}

    ISolvec_Impl::~ISolvec_Impl(){
        if (_params){
            delete _params;
        }
        if(_compact){
            delete _compact;
        }
        if (_solution){
            delete _solution;
        }
        _logger->destroyLogger(this);
    }

    RESULT_CODE ISolvec_Impl::setParams(const IVector *params){
        if(!params){
            if (_logger){
                _logger->log("In ISolver::setParams - params is nullptr", RESULT_CODE::BAD_REFERENCE);
            }
            return RESULT_CODE::BAD_REFERENCE;
        }

        if (params->getDim() != _paramsNum){
            if (_logger){
                _logger->log("In ISolver::setParams - params dimension mismatch", RESULT_CODE::WRONG_ARGUMENT);
            }
            return RESULT_CODE::WRONG_ARGUMENT;
        }

        IVector *clone_params = params->clone();
        if (!clone_params){
            if (_logger){
                _logger->log("In ISolver::setParams - memory error while setting params", RESULT_CODE::OUT_OF_MEMORY);
            }
            return RESULT_CODE::OUT_OF_MEMORY;
        }

        if (_params){
            delete _params;
            _params = nullptr;
        }

        _params = clone_params;
        return RESULT_CODE::SUCCESS;
    }

    RESULT_CODE ISolvec_Impl::setParams(QString &str){
        QStringList params = str.split(";");

        if (int(ISolvec_Impl::getParamsDim()) != params.size()){
            if (_logger){
                _logger->log("In ISolver::setParams - get more numbers than expected", RESULT_CODE::WRONG_DIM);
            }
            return RESULT_CODE::WRONG_DIM;
        }
        size_t dim = ISolvec_Impl::getParamsDim();
        double* d_param = new double[dim];
        for (size_t i = 0; i < dim;i++){
            bool res = false;
            d_param[i] = params.at(i).toDouble(&res);
            if (!res){
                delete[] d_param;
                if (_logger){
                    _logger->log("In ISolver::setParams - could not parse string of params", RESULT_CODE::WRONG_ARGUMENT);
                }
                return RESULT_CODE::WRONG_ARGUMENT;
            }
        }

        return RESULT_CODE::SUCCESS;
    }

    RESULT_CODE ISolvec_Impl::setProblem(IProblem *pProblem){
        if (!pProblem){
            if (_logger){
                _logger->log("In ISolver::setProblem - pProblem is nullptr", RESULT_CODE::BAD_REFERENCE);
            }
            return RESULT_CODE::BAD_REFERENCE;
        }

        _problem = pProblem;

        return RESULT_CODE::SUCCESS;
    }

    RESULT_CODE ISolvec_Impl::setProblemParams(const IVector *params){
        if (!params){
            if (_logger){
               _logger->log("In ISolver::setProblemParams - params is nullptr", RESULT_CODE::BAD_REFERENCE);
            }
            return RESULT_CODE::BAD_REFERENCE;
        }
        if (!_problem){
            if (_logger){
               _logger->log("In ISolver::setProblemParams - problem was not set", RESULT_CODE::BAD_REFERENCE);
            }
            return RESULT_CODE::BAD_REFERENCE;
        }
        RESULT_CODE rc = _problem->setParams(params);

        return rc;
    }

    RESULT_CODE ISolvec_Impl::setCompact(ICompact *pCompact){
        if (!pCompact){
            if (_logger){
                _logger->log("In ISolver::setCompact - params dimension mismatch", RESULT_CODE::BAD_REFERENCE);
            }
            return RESULT_CODE::BAD_REFERENCE;
        }

        if (!_problem->isCompactValid(pCompact)){
            if (_logger){
                _logger->log("In ISolver::setCompact - invalid compact for this problem", RESULT_CODE::OUT_OF_BOUNDS);
            }
            return RESULT_CODE::OUT_OF_BOUNDS;
        }

        ICompact* compact_clone = pCompact->clone();
        if(!compact_clone){
            if (_logger){
                _logger->log("In ISolver::setCompact - memory error while setting compact", RESULT_CODE::OUT_OF_MEMORY);
            }
            return RESULT_CODE::OUT_OF_MEMORY;
        }

        if (_compact){
            delete _compact;
            _compact = nullptr;
        }

        _compact = compact_clone;
        return RESULT_CODE::SUCCESS;
    }

    size_t ISolvec_Impl::getParamsDim() const{
        return _paramsNum;
    }

    RESULT_CODE ISolvec_Impl::solve(){
        if (!_problem || !_compact || !_params){
            if (_logger){
               _logger->log("In ISolver::solve - params is nullptr", RESULT_CODE::BAD_REFERENCE);
            }
            return RESULT_CODE::BAD_REFERENCE;
        }
        size_t dim = _compact->getDim();

        double* step = new double[dim];
        for (size_t i = 0; i < dim;i++){
            step[i] = _params->getCoord(0);
        }
        IVector* step_v = IVector::createVector(dim, step, _logger);
        if (!step_v){
            delete[] step;
            if (_logger){
               _logger->log("In ISolver::solve - memory error", RESULT_CODE::OUT_OF_MEMORY);
            }
            return RESULT_CODE::OUT_OF_MEMORY;
        }
        delete[] step;

        ICompact::iterator *iter = _compact->begin(step_v);
        if (!iter){
            delete step_v;
            step_v = nullptr;
            if (_logger){
               _logger->log("In ISolver::solve - memory error", RESULT_CODE::OUT_OF_MEMORY);
            }
            return RESULT_CODE::OUT_OF_MEMORY;
        }
        delete step_v;
        step_v = nullptr;

        IVector *solution = iter->getPoint();
        if (!solution){
            delete iter;
            iter = nullptr;
            if (_logger){
               _logger->log("In ISolver::solve - memory error", RESULT_CODE::OUT_OF_MEMORY);
            }
            return RESULT_CODE::OUT_OF_MEMORY;
        }
        double min_value = NAN;
        RESULT_CODE rc = _problem->goalFunctionByArgs(solution, min_value);
        IVector *new_solution = nullptr;

        while (iter->doStep() == RESULT_CODE::SUCCESS){
            double new_min_value = NAN;

            new_solution= iter->getPoint();
            if (!new_solution){
                delete iter;
                delete solution;
                iter = nullptr;
                solution = nullptr;
                if (_logger){
                   _logger->log("In ISolver::solve - memory error", RESULT_CODE::OUT_OF_MEMORY);
                }
                return RESULT_CODE::OUT_OF_MEMORY;
            }

            rc = _problem->goalFunctionByArgs(new_solution, new_min_value);
            if (rc == RESULT_CODE::SUCCESS){
                if (!std::isnan(new_min_value)){
                    if (std::isnan(min_value)){
                        min_value = new_min_value;
                        delete solution;
                        solution = new_solution;
                    }
                    else if (min_value > new_min_value){
                        min_value = new_min_value;
                        delete solution;
                        solution = new_solution;
                    }

                }
            }
        }
        if (!std::isnan(min_value)){
            delete _solution;
            _solution = solution;
            return RESULT_CODE::SUCCESS;
        }
        else {
            return RESULT_CODE::NOT_FOUND;
        }
    }

    RESULT_CODE ISolvec_Impl::getSolution(IVector *&vec) const{
        if (!_solution){
            return RESULT_CODE::NOT_FOUND;
        }
        IVector* solution_copy = _solution->clone();
        if (!solution_copy){
            if(_logger){
                _logger->log("In getSolution memory error", RESULT_CODE::OUT_OF_MEMORY);
            }
            return RESULT_CODE::OUT_OF_MEMORY;
        }
        vec = solution_copy;

        return RESULT_CODE::SUCCESS;
    }


    IBrocker_Impl::IBrocker_Impl(void *solver):
        _solver(solver)
    {}

    IBrocker::Type IBrocker_Impl::getType() const{
        return Type::SOLVER;
    }

    void *IBrocker_Impl::getInterfaceImpl(IBrocker::Type type) const{
        if (type == Type::SOLVER){
            return _solver;
        }
        else {
            return nullptr;
        }
    }

    void IBrocker_Impl::release(){
        delete reinterpret_cast<ISolvec_Impl*>(_solver);
    }
}

void* getBrocker() {
    void *solver = new ISolvec_Impl();
    if (solver){
        void *brocker = new IBrocker_Impl(solver);
        if (brocker){
            return brocker;
        }
        delete reinterpret_cast<ISolvec_Impl*>(solver);
    }
    return nullptr;
}
