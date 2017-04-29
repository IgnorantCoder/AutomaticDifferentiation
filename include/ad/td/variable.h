#pragma once

#include "ad/tape/variable.h"
#include "ad/tape/reverse_calculation_graph.h"

namespace ad { namespace td {
    template <typename V, typename D = V>
    using variable = ad::tape::variable<V, D, ad::tape::reverse_calculation_graph>;
} }