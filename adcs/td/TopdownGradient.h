#pragma once

#include "ad/top_down.h"

namespace Adcs { namespace Td {
    ref class Variable;

    public ref class Gradient {
    public:
        static Gradient^ Generate(Variable^ y);

    private:
        Gradient(ad::td::gradient_holder<double, double>&& native);

    public:
        Gradient(Gradient^ other);
        ~Gradient();
        !Gradient();

    public:
        double InDirectionOf(Variable^ x);

    private:
        ad::td::gradient_holder<double, double> const* _native;
    };
} }