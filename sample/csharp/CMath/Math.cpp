#include "Math.h"

#include <cmath>

namespace CMath {
    double Math::Erf(const double x)
    {
        return std::erf(x);
    }
}