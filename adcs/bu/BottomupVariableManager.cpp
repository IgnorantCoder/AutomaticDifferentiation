#include "BottomupVariableManager.h"

#include "bu/BottomupVariable.h"

namespace Adcs { namespace Bu {
    VariableManager^ VariableManager::Create()
    {
        return gcnew VariableManager(ad::bu::variable_manager<double, double>());
    }

    VariableManager::VariableManager(
        ad::bu::variable_manager<double, double>&& native)
        : _native(new ad::bu::variable_manager<double, double>(std::move(native)))
    {
    }

    VariableManager::VariableManager(VariableManager^ other)
        : _native(new ad::bu::variable_manager<double, double>(*other->_native))
    {

    }

    VariableManager::~VariableManager()
    {
        this->!VariableManager();
    }

    VariableManager::!VariableManager()
    {
        delete _native;
        this->_native = nullptr;
    }

    Variable ^ VariableManager::ToVariable(const double v)
    {
        auto&& ret = _native->to_variable(v);
        return gcnew Variable(std::move(ret));
    }
}}