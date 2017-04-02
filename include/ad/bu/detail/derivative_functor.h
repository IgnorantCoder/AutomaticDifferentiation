#pragma once

#include <cmath>
#include <type_traits>

#include "ad/bu/detail/unary_functor.h"
#include "ad/bu/detail/binary_functor.h"

namespace ad { namespace bu { namespace detail {
    template <typename F>
    struct derivative_functor;
}}}

#include "ad/bu/detail/derivative_unary_functor.h"
#include "ad/bu/detail/derivative_binary_functor.h"