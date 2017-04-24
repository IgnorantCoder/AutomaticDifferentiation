#pragma once

#include <cmath>
#include <type_traits>

#include "ad/functor/unary_functor.h"
#include "ad/functor/binary_functor.h"

namespace ad { namespace functor {
    template <typename F>
    struct derivative_functor;
}}

#include "ad/functor/derivative_unary_functor.h"
#include "ad/functor/derivative_binary_functor.h"