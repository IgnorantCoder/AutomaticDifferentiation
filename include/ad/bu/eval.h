#pragma once

#include <type_traits>

#include "ad/bu/variable.h"
#include "ad/bu/variable_expression.h"

namespace ad { namespace bu {
    /**
    @note Please do not use for rvalue generation. It causes a runtime error.
    */
    template <typename E>
    variable<typename E::value_type> eval(const variable_expression<E>& e)
    {
        using value_type = typename E::value_type;
        return variable<value_type>(e);
    }

    template <typename T>
    std::enable_if_t<
        !is_variable_expression<T>::value,
        T
    > eval(const T& t)
    {
        return t;
    }
} }