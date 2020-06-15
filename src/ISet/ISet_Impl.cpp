#include <vector>
#include <cmath>
#include "ISet.h"
#include "RC.h"

namespace{
    class ISet_Impl: public ISet{
    private:
        ILogger *_logger;
        size_t _dim;
        std::vector<IVector*> _data;

    public:
        ISet_Impl(ILogger* logger);

        ~ISet_Impl() override;

        RESULT_CODE insert(const IVector *pVector, IVector::NORM norm, double tolerance) override;

        RESULT_CODE get(IVector *&pVector, size_t index) const override;

        RESULT_CODE
        get(IVector *&pVector, IVector const *pSample, IVector::NORM norm, double tolerance) const override;

        size_t getDim() const override;

        size_t getSize() const override;

        void clear() override;

        RESULT_CODE erase(size_t index) override;

        RESULT_CODE erase(IVector const *pSample, IVector::NORM norm, double tolerance) override;

        ISet *clone() const override;
    }; // ISet_Impl

    ISet_Impl::ISet_Impl(ILogger* logger):
        _logger(logger),
        _dim(0)
    {}

    RESULT_CODE ISet_Impl::insert(const IVector *pVector, IVector::NORM norm, double tolerance) {
        if (!pVector){
            return RESULT_CODE::WRONG_ARGUMENT;
        }

        if (_dim != 0 && pVector->getDim() != _dim){
            return RESULT_CODE::WRONG_DIM;
        }

        if (tolerance < 0){
            return RESULT_CODE::WRONG_ARGUMENT;
        }

        if (_data.empty()){
            IVector *clone = pVector->clone();
            if (!clone){
                return RESULT_CODE::OUT_OF_MEMORY;
            }
            _data.push_back(clone);
            _dim = clone->getDim();
            return RESULT_CODE::SUCCESS;
        }

        for (auto v : _data){
            IVector *diff = IVector::sub(v, pVector, _logger);
            if (!diff){
                return RESULT_CODE::OUT_OF_MEMORY;
            }

            if (diff->norm(norm) <tolerance){
                delete diff;
                diff = nullptr;

                return RESULT_CODE::SUCCESS;
            }

            delete diff;
            diff = nullptr;
        }

        IVector *clone = pVector->clone();
        if (!clone){
            return RESULT_CODE::OUT_OF_MEMORY;
        }
        _data.push_back(clone);

        return RESULT_CODE::SUCCESS;
    } // ISet_Impl::insert

    RESULT_CODE ISet_Impl::get(IVector *&pVector, size_t index) const {
        if (index < 0){
            return RESULT_CODE::OUT_OF_BOUNDS;
        }

        if (index >= _data.size()){
            return RESULT_CODE::NOT_FOUND;
        }

        auto clone = _data[index]->clone();
        if (!clone){
            return RESULT_CODE::OUT_OF_MEMORY;
        }
        pVector = clone;

        return RESULT_CODE::SUCCESS;
    } // ISet_Impl::get

    RESULT_CODE ISet_Impl::get(IVector *&pVector, IVector const *pSample, IVector::NORM norm, double tolerance) const {
        if (!pSample){
            return RESULT_CODE::WRONG_ARGUMENT;
        }

        if (pSample->getDim() != _dim){
            return RESULT_CODE::WRONG_DIM;
        }

        if (tolerance < 0){
            return RESULT_CODE::WRONG_ARGUMENT;
        }

        for (auto v : _data){
            auto diff = IVector::sub(pSample, v, _logger);
            if (!diff){
                return RESULT_CODE::OUT_OF_MEMORY;
            }

            auto diff_norm = diff->norm(norm);
            if (std::isnan(diff_norm)){
                return RESULT_CODE::CALCULATION_ERROR;
            }

            if (fabs(diff_norm) < tolerance){
                delete diff;
                diff = nullptr;

                auto clone = v->clone();
                if (!clone){
                    return RESULT_CODE::OUT_OF_MEMORY;
                }

                pVector = clone;
                return RESULT_CODE::SUCCESS;
            }

            delete diff;
            diff = nullptr;
        }

        return RESULT_CODE::NOT_FOUND;
    } // ISet_Impl::get

    size_t ISet_Impl::getDim() const {
        return _dim;
    } // ISet_Impl::getDim

    size_t ISet_Impl::getSize() const {
        return _data.size();
    } // ISet_Impl::getSize

    void ISet_Impl::clear() {
        for (auto v : _data){
            delete v;
            v = nullptr;
        }
        _data.clear();
        _dim = 0;
    } // ISet_Impl::clear

    ISet_Impl::~ISet_Impl() {
        for (auto v : _data){
            delete v;
            v = nullptr;
        }
        _data.clear();
        _logger->destroyLogger(this);
    } // ISet_Impl::~ISet_Impl

    RESULT_CODE ISet_Impl::erase(size_t index) {
        if (index < 0){
            return RESULT_CODE::OUT_OF_BOUNDS;
        }

        if (index >= _data.size()){
            return RESULT_CODE::NOT_FOUND;
        }
        delete _data[index];
        _data[index] = nullptr;
        _data.erase(_data.begin() + index);

        if (_data.empty()){
            _data.clear();
            _dim = 0;
        }

        return RESULT_CODE::SUCCESS;
    } // ISet_Impl::erase

    RESULT_CODE ISet_Impl::erase(IVector const *pSample, IVector::NORM norm, double tolerance) {
        if (!pSample){
            return RESULT_CODE::WRONG_ARGUMENT;
        }

        if (tolerance < 0){
            return RESULT_CODE::WRONG_ARGUMENT;
        }

        if (pSample->getDim() != _dim){
            return RESULT_CODE::WRONG_DIM;
        }



        for (auto v = _data.begin(); v != _data.end(); ){
            auto diff = IVector::sub(pSample, *v, _logger);
            if (!diff){
                return RESULT_CODE::OUT_OF_MEMORY;
            }

            auto diff_norm = diff->norm(norm);
            if (std::isnan(diff_norm)){
                return RESULT_CODE::CALCULATION_ERROR;
            }

            if (fabs(diff_norm) < tolerance){
                delete diff;
                diff = nullptr;

                delete *v;
                v = _data.erase(v);
            }
            else {
                v++;
            }

            delete diff;
            diff = nullptr;
        }

        if (_data.empty()){
            _data.clear();
            _dim = 0;
        }

        return RESULT_CODE::SUCCESS;
    } // ISet_Impl::erase

    ISet *ISet_Impl::clone() const {
        auto clone_set = new (std::nothrow)ISet_Impl(_logger);
        if(!clone_set){
            return nullptr;
        }

        for (auto v : _data){
            clone_set->_data.push_back(v);
        }

        clone_set->_dim = _dim;

        return clone_set;
    } // ISet_Impl::clone
}