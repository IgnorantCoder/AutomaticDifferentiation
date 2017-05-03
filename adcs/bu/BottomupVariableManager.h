#pragma once

#include "ad/bottom_up.h"

namespace Adcs { namespace Bu {
    ref class Variable;

    public ref class VariableManager {
    public:
        static VariableManager^ Create();

    private:
        VariableManager(ad::rt::bu::variable_manager<double, double>&& native);

    public:
        VariableManager(VariableManager^ other);
        ~VariableManager();
        !VariableManager();

    public:
        Variable^ ToVariable(const double v);

    private:
        ad::rt::bu::variable_manager<double, double>* _native;
    };
}}