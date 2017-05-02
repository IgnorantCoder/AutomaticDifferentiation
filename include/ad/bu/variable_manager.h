#pragma once

#include "ad/tape/variable_manager.h"
#include "ad/tape/forward_calculation_graph.h"

namespace ad { namespace bu {
    template <typename V, typename D>
    using variable_manager
        = ad::tape::variable_manager<V, D, ad::tape::forward_calculation_graph>;

    template <typename V, typename D = V>
    variable_manager<V, D> create_manager()
    {
        return variable_manager<V, D>();
    }
} }