#pragma once

#include "ad/rt/tape/variable_manager.h"
#include "ad/rt/tape/reverse_calculation_graph.h"

namespace ad { namespace rt { namespace td {
    template <typename V, typename D>
    using variable_manager 
        = ad::rt::tape::variable_manager<V, D, ad::rt::tape::reverse_calculation_graph>;

    template <typename V, typename D = V>
    variable_manager<V, D> create_manager()
    {
        return variable_manager<V, D>();
    }
} } }