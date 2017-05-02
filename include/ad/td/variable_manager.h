#pragma once

#include "ad/tape/variable_manager.h"
#include "ad/tape/reverse_calculation_graph.h"

namespace ad { namespace td {
    template <typename V, typename D>
    using variable_manager 
        = ad::tape::variable_manager<V, D, ad::tape::reverse_calculation_graph>;

    template <typename V, typename D = V>
    variable_manager<V, D> create_manager()
    {
        return variable_manager<V, D>();
    }
} }