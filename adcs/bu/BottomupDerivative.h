#pragma once

#include "ad/bottom_up.h"

namespace Adcs { namespace Bu {
    ref class Variable;

    public ref class Derivative {
    public:
        static Derivative^ Create(Variable^ y);

    private:
        Derivative(const ad::rt::bu::variable<double>& native);

    public:
        Derivative(Derivative^ other);
        ~Derivative();
        !Derivative();

    public:
        double D(Variable^ x);

    private:
        ad::rt::bu::variable<double> const* _native;
    };
} }