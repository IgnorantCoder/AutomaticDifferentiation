#pragma once

namespace ad { namespace type_traits {
    template <typename T, typename ...Rest>
    struct head {
        using type = T;
    };
} }