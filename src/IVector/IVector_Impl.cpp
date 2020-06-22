//
// Created by user on 12.06.2020.
//
#define _EXPORTING

#include <cmath>
#include <cstring>
#include <new>
#include "../../include/IVector.h"

namespace {
    class IVector_Impl : public IVector{
    private:
        size_t _dim = 0;
        double *_data= nullptr;
    public:
        IVector_Impl(size_t dim, double *data);
        ~IVector_Impl() override;

        IVector *clone() const override;

        double getCoord(size_t index)const override;
        RESULT_CODE setCoord(size_t index, double value) override;
        double norm(NORM norm) const override;
        size_t getDim() const override;
    };

    IVector_Impl::IVector_Impl(size_t dim, double *data):
        _dim(dim),
        _data(data)
    {}

    IVector_Impl::~IVector_Impl() {
        if (!_data){
            delete[] _data;
            _data = nullptr;
        }
    }

    IVector * IVector_Impl::clone() const {
        if (!_dim || !_data ){
            return nullptr;
        }

        auto * data = new (std::nothrow)double[_dim];

        if (!data){
            return nullptr;
        }

        memcpy(data, _data, _dim * sizeof(double));

        return new (std::nothrow)IVector_Impl(_dim, data);
    }

    double IVector_Impl::getCoord(size_t index) const {
        return (index >= _dim || !_data) ? NAN : _data[index];
    }

    RESULT_CODE IVector_Impl::setCoord(size_t index, double value) {
        if (!_data){
            return RESULT_CODE::OUT_OF_MEMORY;
        }

        if (index >= _dim){
            return RESULT_CODE::OUT_OF_BOUNDS;
        }

        if (std::isnan(value)){
            return RESULT_CODE::WRONG_ARGUMENT;
        }

        _data[index] = value;

        return RESULT_CODE::SUCCESS;
    }

    double IVector_Impl::norm(IVector::NORM norm) const {
        double result = 0;

        switch (norm){
            case NORM::NORM_1:
                for (size_t i = 0; i < _dim; i++){
                    result += fabs(_data[i]);
                }
                break;
            case NORM::NORM_2:
                for (size_t i = 0; i < _dim; i++){
                    result += _data[i] * _data[i];
                }
                result = sqrt(result);
                break;
            case NORM::NORM_INF:
                result = fabs(_data[0]);
                for (size_t i = 0; i < _dim; i++){
                    if (result < _data[i]){
                        result = fabs(_data[i]);
                    }
                }
                break;
            default:
                result = NAN;
                break;
        }

        return result;
    }

    size_t IVector_Impl::getDim() const {
        return _dim;
    }

}

