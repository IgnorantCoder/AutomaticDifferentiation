#pragma once

#include "ad/top_down.h"

namespace Adcs { namespace Td {
    ref class Variable;

    public ref class VariableManager {
    public:
        static VariableManager^ Create();

    private:
        VariableManager(ad::rt::td::variable_manager<double, double>&& native);

    public:
        VariableManager(VariableManager^ other);
        ~VariableManager();
        !VariableManager();

    public:
        Variable^ ToVariable(const double v);

    private:
        ad::rt::td::variable_manager<double, double>* _native;
    };
}}