#include "BottomupMath.h"

#include "ad/bottom_up.h"
#include "bu/BottomupVariable.h"

namespace Adcs { namespace Bu {
    Variable ^ Math::Sqrt(Variable ^ x)
    {
        auto&& y = std::sqrt(x->get_native());
        return gcnew Variable(std::move(y));
    }

    Variable ^ Math::Sin(Variable ^ x)
    {
        auto&& y = std::sin(x->get_native());
        return gcnew Variable(std::move(y));
    }

    Variable ^ Math::Cos(Variable ^ x)
    {
        auto&& y = std::cos(x->get_native());
        return gcnew Variable(std::move(y));
    }

    Variable ^ Math::Tan(Variable ^ x)
    {
        auto&& y = std::tan(x->get_native());
        return gcnew Variable(std::move(y));
    }

    Variable ^ Math::Log(Variable ^ x)
    {
        auto&& y = std::log(x->get_native());
        return gcnew Variable(std::move(y));
    }

    Variable ^ Math::Exp(Variable ^ x)
    {
        auto&& y = std::exp(x->get_native());
        return gcnew Variable(std::move(y));
    }

    Variable ^ Math::Erf(Variable ^ x)
    {
        auto&& y = std::erf(x->get_native());
        return gcnew Variable(std::move(y));
    }
}}