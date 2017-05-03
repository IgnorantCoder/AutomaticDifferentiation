#include "TopdownGradient.h"

#include "td/TopdownVariable.h"

namespace Adcs { namespace Td {
    Gradient^ Gradient::Generate(Variable ^ y)
    {
        auto&& grad = ad::rt::td::gradient(y->get_native());
        return gcnew Gradient(std::move(grad));
    }

    Gradient::Gradient(ad::rt::td::gradient_holder<double, double>&& native)
        : _native(new ad::rt::td::gradient_holder<double, double>(native))
    {
    }

    Gradient::Gradient(Gradient ^ other)
        : _native(new ad::rt::td::gradient_holder<double, double>(*other->_native))
    {
    }

    Gradient::~Gradient()
    {
        this->!Gradient();
    }

    Gradient::!Gradient()
    {
        delete _native;
        _native = nullptr;
    }

    double Gradient::InDirectionOf(Variable ^ x)
    {
        return _native->in_direction_of(x->get_native());
    }
}}