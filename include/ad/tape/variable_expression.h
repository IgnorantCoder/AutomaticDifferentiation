#pragma once

#include <type_traits>

#include "ad/tape/variable.h"

namespace ad { namespace tape {
    template <typename E>
    struct is_variable_expression : std::false_type {};

    template <typename V, typename D, template <typename> typename G>
    struct is_variable_expression<variable<V, D, G>> : std::true_type {};
} }