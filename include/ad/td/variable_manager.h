#pragma once

#include "ad/tape/variable_manager.h"
#include "ad/tape/reverse_calculation_graph.h"

namespace ad { namespace td {
    template <typename V, typename D = V>
    ad::tape::variable_manager<V, D, ad::tape::reverse_calculation_graph>
    create_manager()
    {
        return ad::tape::variable_manager<
            V, D, ad::tape::reverse_calculation_graph
        >();
    }
} }