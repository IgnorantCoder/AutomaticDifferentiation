#include "TopdownVariable.h"

namespace Adcs { namespace Td {
    Variable::Variable(ad::td::variable<double>&& native)
        : _native(new ad::td::variable<double>(std::move(native)))
    {}

    const ad::td::variable<double>& Variable::get_native()
    {
        return *_native;
    }

    Variable::Variable(Variable ^ other)
        : _native(new ad::td::variable<double>(*other->_native))
    {
    }

    Variable::~Variable()
    {
        this->!Variable();
    }

    Variable::!Variable()
    {
        delete _native;
        _native = nullptr;
    }
    
    Variable^ Variable::operator-(Variable^ x)
    {
        auto&& ret = - *x->_native;
        return gcnew Variable(std::move(ret));
    }

    Variable^ Variable::operator+(Variable^ rhs, Variable^ lhs)
    {
        auto&& ret = *rhs->_native + *lhs->_native;
        return gcnew Variable(std::move(ret));
    }

    Variable^ Variable::operator-(Variable^ rhs, Variable^ lhs)
    {
        auto&& ret = *rhs->_native - *lhs->_native;
        return gcnew Variable(std::move(ret));
    }

    Variable^ Variable::operator*(Variable^ rhs, Variable^ lhs)
    {
        auto&& ret = *rhs->_native * *lhs->_native;
        return gcnew Variable(std::move(ret));
    }

    Variable^ Variable::operator/(Variable^ rhs, Variable^ lhs)
    {
        auto&& ret = *rhs->_native / *lhs->_native;
        return gcnew Variable(std::move(ret));
    }

    Variable^ Variable::operator+(Variable^ rhs, double lhs)
    {
        auto&& ret = *rhs->_native + lhs;
        return gcnew Variable(std::move(ret));
    }

    Variable^ Variable::operator-(Variable^ rhs, double lhs)
    {
        auto&& ret = *rhs->_native - lhs;
        return gcnew Variable(std::move(ret));
    }

    Variable^ Variable::operator*(Variable^ rhs, double lhs)
    {
        auto&& ret = *rhs->_native * lhs;
        return gcnew Variable(std::move(ret));
    }

    Variable^ Variable::operator/(Variable^ rhs, double lhs)
    {
        auto&& ret = *rhs->_native / lhs;
        return gcnew Variable(std::move(ret));
    }

    Variable ^ Variable::operator+(double rhs, Variable ^ lhs)
    {
        auto&& ret = rhs + *lhs->_native;
        return gcnew Variable(std::move(ret));
    }

    Variable ^ Variable::operator-(double rhs, Variable ^ lhs)
    {
        auto&& ret = rhs - *lhs->_native;
        return gcnew Variable(std::move(ret));
    }

    Variable ^ Variable::operator*(double rhs, Variable ^ lhs)
    {
        auto&& ret = rhs * *lhs->_native;
        return gcnew Variable(std::move(ret));
    }

    Variable ^ Variable::operator/(double rhs, Variable ^ lhs)
    {
        auto&& ret = rhs / *lhs->_native;
        return gcnew Variable(std::move(ret));
    }

    double Variable::ToDouble()
    {
        return static_cast<double>(*_native);
    }
}}