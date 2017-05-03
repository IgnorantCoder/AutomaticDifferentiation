#pragma once

#include "ad/bottom_up.h"

namespace Adcs { namespace Bu {
    public ref class Variable {
    internal:
        explicit Variable(ad::rt::bu::variable<double>&& native);
        const ad::rt::bu::variable<double>& get_native();

    public:
        Variable(Variable^ other);
        ~Variable();
        !Variable();

    public:
        static Variable^ operator -(Variable^ x);
        static Variable^ operator +(Variable^ rhs, Variable^ lhs);
        static Variable^ operator -(Variable^ rhs, Variable^ lhs);
        static Variable^ operator *(Variable^ rhs, Variable^ lhs);
        static Variable^ operator /(Variable^ rhs, Variable^ lhs);
        static Variable^ operator +(Variable^ rhs, double lhs);
        static Variable^ operator -(Variable^ rhs, double lhs);
        static Variable^ operator *(Variable^ rhs, double lhs);
        static Variable^ operator /(Variable^ rhs, double lhs);
        static Variable^ operator +(double rhs, Variable^ lhs);
        static Variable^ operator -(double rhs, Variable^ lhs);
        static Variable^ operator *(double rhs, Variable^ lhs);
        static Variable^ operator /(double rhs, Variable^ lhs);

    public:
        double ToDouble();

    private:
        ad::rt::bu::variable<double> const* _native;
    };
}}