#include "BottomupDerivative.h"

#include "bu/BottomupVariable.h"

namespace Adcs { namespace Bu {
    Derivative ^ Derivative::Create(Variable ^ y)
    {
        return gcnew Derivative(y->get_native());
    }

    Derivative::Derivative(const ad::bu::variable<double>& native)
        : _native(new ad::bu::variable<double>(native))
    {
    }

    Derivative::Derivative(Derivative ^ other)
        : _native(new ad::bu::variable<double>(*other->_native))
    {
    }

    Derivative::~Derivative()
    {
        this->!Derivative();
    }

    Derivative::!Derivative()
    {
        delete _native;
        _native = nullptr;
    }

    double Derivative::D(Variable ^ x)
    {
        return ad::bu::d(*_native).d(x->get_native());
    }
}}