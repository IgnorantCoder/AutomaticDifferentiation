#pragma once

#include "ad/rt/tape/variable.h"
#include "ad/rt/tape/forward_calculation_graph.h"

namespace ad { namespace rt {namespace bu {
    template <typename V, typename D = V>
    using variable = ad::rt::tape::variable<V, D, ad::rt::tape::forward_calculation_graph>;
} } }