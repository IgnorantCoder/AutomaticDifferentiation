#pragma once

#include "ad/top_down.h"

namespace Adcs { namespace Td {
    public ref class Variable {
    internal:
        explicit Variable(ad::td::variable<double>&& native);
        const ad::td::variable<double>& get_native();

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
        ad::td::variable<double> const* _native;
    };
}}