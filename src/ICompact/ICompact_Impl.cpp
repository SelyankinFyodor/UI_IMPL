#include <new>
#include <cmath>
#include "ICompact.h"
#include "IVector.h"

namespace {
    class ICompact_Impl : public ICompact {
    private:
        size_t _dim;
        IVector *_begin;
        IVector *_end;
        ILogger *_logger;

    public:
        class Iterator_Impl : public ICompact::iterator{
        private:
            ICompact * _compact;
            IVector *_current;
            IVector *_step;
            IVector *_direction;

        public:
            Iterator_Impl(IVector* begin, IVector* step, IVector* direction, ICompact *compact);

            ~Iterator_Impl() override;

            RESULT_CODE doStep() override;

            IVector *getPoint() const override;

            RESULT_CODE setDirection(IVector const *const dir) override;
        }; // Iterator_Impl

        ICompact_Impl(IVector* begin, IVector*  end, size_t dim, ILogger *logger);

        IVector *getBegin() const override;

        IVector *getEnd() const override;

        iterator *end(IVector const *const step) override;

        iterator *begin(IVector const *const step) override;

        RESULT_CODE isContains(IVector const *const vec, bool &result) const override;

        RESULT_CODE isSubSet(ICompact const *const other, bool &result) const override;

        RESULT_CODE isIntersects(ICompact const *const other, bool &result) const override;

        size_t getDim() const override;

        ICompact *clone() const override;

        ~ICompact_Impl() override;
    }; // ICompact_Impl

    ICompact_Impl::ICompact_Impl(IVector* begin, IVector*  end, size_t dim, ILogger *logger):
        _dim(dim),
        _begin(begin),
        _end(end),
        _logger(logger)
    {} // ICompact_Impl::ICompact_Impl

    IVector *ICompact_Impl::getBegin() const {
        return _begin->clone();
    } // ICompact_Impl::getBegin

    IVector *ICompact_Impl::getEnd() const {
        return _end->clone();
    } // ICompact_Impl::getEnd

    ICompact::iterator *ICompact_Impl::end(IVector const *const step) {
        size_t dim = step->getDim();

        for (size_t i = 0; i < dim; i++){
            if (step->getCoord(i) >= 0){
                return nullptr;
            }
        }

        IVector* clone_end = _end->clone();
        if (!clone_end){
            return nullptr;
        }

        IVector* clone_step = step->clone();
        if (!clone_step){
            delete clone_end;
            return nullptr;
        }

        auto *dir_coord = new double[dim];

        for (size_t i = 0; i < dim; i++){
            dir_coord[i] = i;
        }

        IVector* default_direction = IVector::createVector(dim, dir_coord, _logger);
        if (!default_direction){
            delete clone_end;
            delete [] dir_coord;
            delete clone_step;
            return nullptr;
        }

        auto *iter_end = new(std::nothrow) Iterator_Impl(clone_end, clone_step, default_direction, this);
        delete [] dir_coord;

        return iter_end;
    } // ICompact_Impl::end

    ICompact::iterator *ICompact_Impl::begin(IVector const *const step) {
        size_t dim = step->getDim();

        for (size_t i = 0; i < dim; i++){
            if (step->getCoord(i) <= 0){
                return nullptr;
            }
        }

        IVector* clone_beg = _begin->clone();
        if (!clone_beg){
            return nullptr;
        }

        IVector* clone_step = step->clone();
        if (!clone_step){
            delete clone_beg;
            return nullptr;
        }

        auto *dir_coord = new double[dim];

        for (size_t i = 0; i < dim; i++){
            dir_coord[i] = i;
        }

        IVector* default_direction = IVector::createVector(dim, dir_coord, _logger);
        if (!default_direction){
            delete clone_beg;
            delete [] dir_coord;
            delete clone_step;
            return nullptr;
        }

        auto *iter_end = new(std::nothrow) Iterator_Impl(clone_beg, clone_step, default_direction, this);

        delete [] dir_coord;

        return iter_end;
    } // ICompact_Impl::begin

    ICompact_Impl::~ICompact_Impl() {
        delete _begin;
        delete _end;
    } // ICompact_Impl::~ICompact_Impl

    RESULT_CODE ICompact_Impl::isContains(IVector const *const vec, bool &result) const {
        if(!vec){
            return RESULT_CODE::BAD_REFERENCE;
        }

        if (vec->getDim() != _dim){
            return RESULT_CODE::WRONG_DIM;
        }

        for (size_t i = 0;i < _dim; i++){
            if (vec->getCoord(i) > _end->getCoord(i) ||
                vec->getCoord(i) < _begin->getCoord(i)){
                result = false;

                return RESULT_CODE::SUCCESS;
            }
        }
        result = true;
        return RESULT_CODE::SUCCESS;
    } // ICompact_Impl::isContains

    RESULT_CODE ICompact_Impl::isSubSet(ICompact const *const other, bool &result) const {
        if (!other){
            return RESULT_CODE::BAD_REFERENCE;
        }

        if (other->getDim() != _dim){
            return RESULT_CODE::WRONG_DIM;
        }

        bool contains = false;

        // check if compact contains other._begin
        IVector* o_beg = other->getBegin();
        if (!o_beg) {
            return RESULT_CODE::OUT_OF_MEMORY;
        }

        RESULT_CODE resultCode = isContains(o_beg, contains);
        if (resultCode != RESULT_CODE::SUCCESS){
            delete o_beg;
            return resultCode;
        }
        if (!contains){
            delete o_beg;
            result = false;
            return RESULT_CODE::SUCCESS;
        }

        // check if compact contains other._end
        IVector* o_end = other->getEnd();
        if (!o_end) {
            delete o_beg;
            return RESULT_CODE::OUT_OF_MEMORY;
        }

        resultCode = isContains(o_end, contains);
        if (resultCode != RESULT_CODE::SUCCESS){
            delete o_beg;
            delete o_end;
            return resultCode;
        }
        if (!contains){
            delete o_beg;
            delete o_end;
            result = false;
            return RESULT_CODE::SUCCESS;
        }

        delete o_beg;
        delete o_end;
        result = true;
        return RESULT_CODE ::SUCCESS;
    } // ICompact_Impl::isSubSet

    RESULT_CODE ICompact_Impl::isIntersects(ICompact const *const other, bool &result) const {
        if (!other){
            return RESULT_CODE::BAD_REFERENCE;
        }

        if (other->getDim() != _dim){
            return RESULT_CODE::WRONG_DIM;
        }

        bool contains_beg = false;

        // check if compact contains other._begin
        IVector* o_beg = other->getBegin();
        if (!o_beg) {
            return RESULT_CODE::OUT_OF_MEMORY;
        }

        RESULT_CODE resultCode = isContains(o_beg, contains_beg);
        if (resultCode != RESULT_CODE::SUCCESS){
            delete o_beg;
            return resultCode;
        }
        delete o_beg;

        bool contains_end = false;

        // check if compact contains other._end
        IVector* o_end = other->getEnd();
        if (!o_end) {
            return RESULT_CODE::OUT_OF_MEMORY;
        }

        resultCode = isContains(o_end, contains_end);
        if (resultCode != RESULT_CODE::SUCCESS){
            delete o_beg;
            delete o_end;
            return resultCode;
        }
        delete o_end;

        result = contains_beg || contains_end;
        return RESULT_CODE::SUCCESS;
    } // ICompact_Impl::isIntersects

    size_t ICompact_Impl::getDim() const {
        return _dim;
    } // ICompact_Impl::getDim

    ICompact *ICompact_Impl::clone() const {
        return ICompact_Impl::createCompact(_begin, _end, _logger);
    } // ICompact_Impl::clone

    ICompact_Impl::Iterator_Impl::~Iterator_Impl(){
        delete _direction;
        delete _current;
        delete _step;
    } // ICompact_Impl::Iterator_Impl::~Iterator_Impl

    RESULT_CODE ICompact_Impl::Iterator_Impl::doStep() {
        IVector* clone_end = _compact->getEnd();
        IVector* clone_beg = _compact->getBegin();
        size_t i = 0;
        size_t dim = _direction->getDim();
        bool from_begin = _step->getCoord(0) > 0;

        for (; i < dim; i++){
            size_t cur_i = _direction->getCoord(i);

            double new_value = _current->getCoord(cur_i) + _step->getCoord(cur_i);
            if (from_begin && (new_value > clone_end->getCoord(cur_i))){
                _current->setCoord(cur_i, clone_beg->getCoord(cur_i));
            }
            else if (!from_begin && (new_value < clone_beg->getCoord(cur_i))){
                _current->setCoord(cur_i, clone_end->getCoord(cur_i));
            }
            else{
                _current->setCoord(cur_i, new_value);
                break;
            }
        }

        delete clone_beg;
        delete clone_end;

        if (i>=0 && i < dim){
            return RESULT_CODE::SUCCESS;
        }

        return RESULT_CODE ::OUT_OF_BOUNDS;
    } // ICompact_Impl::Iterator_Impl::doStep

    IVector *ICompact_Impl::Iterator_Impl::getPoint() const {
        return _current->clone();
    } // ICompact_Impl::Iterator_Impl::getPoint

    RESULT_CODE ICompact_Impl::Iterator_Impl::setDirection(IVector const *const dir) {
        IVector *copy_dir = dir->clone();
        if (!copy_dir){
            return RESULT_CODE::OUT_OF_MEMORY;
        }
        //check direction
        size_t dim = dir->getDim();
        if (dim != _compact->getDim()){
            delete copy_dir;
            return RESULT_CODE::WRONG_DIM;
        }
        int *check = new int[dim]{0};
        for (size_t i = 0; i < dim ; i++){
            double cord = copy_dir->getCoord(i);
            if (int(cord) != cord){
                delete copy_dir;
                return RESULT_CODE::WRONG_ARGUMENT;
            }
            check[int(cord)] += 1;
        }
        for (size_t i = 0; i < dim;i++){
            if (check[i] != 1){
                delete copy_dir;
                delete[] check;
                return RESULT_CODE::WRONG_ARGUMENT;
            }
        }

        delete _direction;
        _direction = copy_dir;

        bool from_begin = _step->getCoord(0) > 0;
        if (from_begin){
            IVector * clone_beg = _compact->getBegin();
            if (!clone_beg){
                return RESULT_CODE::OUT_OF_MEMORY;
            }
            delete _current;
            _current = clone_beg;
        }
        else{
            IVector * clone_end = _compact->getEnd();
            if (!clone_end){
                return RESULT_CODE::OUT_OF_MEMORY;
            }
            delete _current;
            _current = clone_end;
        }

        return RESULT_CODE ::SUCCESS;
    } // ICompact_Impl::Iterator_Impl::setDirection

    ICompact_Impl::Iterator_Impl::Iterator_Impl(IVector* begin, IVector* step, IVector* direction, ICompact *compact):
        _current(begin),
        _step(step),
        _direction(direction),
        _compact(compact)
        {}
}