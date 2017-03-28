#include <functional>
#include <iostream>

#include "ad/bottom_up.h"

struct const_values {
    static constexpr double c1 = 2;
    static constexpr double c2 = -3;
    static constexpr double c3 = 1;
};

template <typename T1, typename T2>
auto f(const T1& x0, const T2& x1)
{
    constexpr double c1 = const_values::c1;
    constexpr double c2 = const_values::c2;
    constexpr double c3 = const_values::c3;
    return ((std::cos(x0) - c1) * std::sin(x1) - (x1 * x1 / c2)) * c3;
}

template <typename F>
auto create_simulator(const F f, const double h)
{
    return [f, h](const double x)
    {
        return (f(x + h) - f(x - h)) / (2.0 * h);
    };
}

int main()
{
    const double x0 = 0.5;
    const double x1 = 1.0;

    {
        const double h = 0.001;
        const auto sim_dfdx0
            = create_simulator([x1](const double x) {return f(x, x1);}, h);
        const auto sim_dfdx1
            = create_simulator([x0](const double x) {return f(x0, x);}, h);

        std::cout << f(x0, x1) << std::endl;
        std::cout << sim_dfdx0(x0) << std::endl;
        std::cout << sim_dfdx1(x1) << std::endl;
    }
    {
        const auto mgr = ad::bu::create_variable_manager(x0, x1);
        const auto v0 = mgr.get_variable(x0);
        const auto v1 = mgr.get_variable(x1);
        const auto y = f(v0, v1);
        std::cout << static_cast<double>(y) << std::endl;
        std::cout << ad::bu::d(y).d(x0) << std::endl;
        std::cout << ad::bu::d(y).d(x1) << std::endl;
    }
    {
        std::vector<double> v = { x0, x1 };
        const auto mgr = ad::bu::create_variable_manager(v);
        const auto v0 = mgr.get_variable(v[0]);
        const auto v1 = mgr.get_variable(v[1]);
        const auto y = f(v0, v1);
        std::cout << static_cast<double>(y) << std::endl;
        std::cout << ad::bu::d(y).d(v[0]) << std::endl;
        std::cout << ad::bu::d(y).d(v[1]) << std::endl;
    }
    return 0;
}