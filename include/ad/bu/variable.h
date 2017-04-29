#pragma once

#include "ad/tape/variable.h"
#include "ad/tape/forward_calculation_graph.h"

namespace ad { namespace bu {
    template <typename V, typename D = V>
    using variable = ad::tape::variable<V, D, ad::tape::forward_calculation_graph>;
} }