#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>

#include "ad/bottom_up.h"
#include "ad/top_down.h"

namespace bp = boost::python;

#ifdef _DEBUG
#define DEFINE_PYTHON_SCOPE(METHOD)                                             \
bp::object module(                                                              \
    bp::handle<>(bp::borrowed(PyImport_AddModule("adpyd.##METHOD"))));          \
bp::scope().attr(#METHOD) = module;                                             \
bp::scope scope = module;
#else
#define DEFINE_PYTHON_SCOPE(METHOD)                                             \
bp::object module(                                                              \
    bp::handle<>(bp::borrowed(PyImport_AddModule("adpy.##METHOD"))));           \
bp::scope().attr(#METHOD) = module;                                             \
bp::scope scope = module;
#endif

#define DEFINE_VARIABLE_MANAGER(SHORT_METHOD)                                   \
using variable_manager                                                          \
    = ad::##SHORT_METHOD##::variable_manager<                                   \
        double,                                                                 \
        double>;                                                                \
bp::class_<variable_manager>("VariableManager")                                 \
    .def("to_variable", &variable_manager::to_variable);                        \
bp::def(                                                                        \
    "create_variable_manager",                                                  \
    &ad::##SHORT_METHOD##::create_manager<double>);

#define DEFINE_BINARY_FOR_OBJECT_AND_DOUBLE(OPERATOR)                           \
.def(double() OPERATOR boost::python::self)                                     \
.def(boost::python::self OPERATOR double())                                     \
.def(boost::python::self OPERATOR boost::python::self)

#define DEFINE_UNARY_FOR_OBJECT(NAME, DEIRECTION)                               \
boost::python::def(                                                             \
    #NAME,                                                                      \
    &ad::tape::##NAME<                                                          \
        double,                                                                 \
        double,                                                                 \
        ad::tape::##DEIRECTION##_calculation_graph>);

#define DEFINE_VARIABLE(SHORT_METHOD, DEIRECTION)                               \
using variable = ad::##SHORT_METHOD##::variable<double>;                        \
bp::class_<variable>("Variable")                                                \
    .def(-bp::self)                                                             \
    DEFINE_BINARY_FOR_OBJECT_AND_DOUBLE(+)                                      \
    DEFINE_BINARY_FOR_OBJECT_AND_DOUBLE(-)                                      \
    DEFINE_BINARY_FOR_OBJECT_AND_DOUBLE(*)                                      \
    DEFINE_BINARY_FOR_OBJECT_AND_DOUBLE(/);                                     \
DEFINE_UNARY_FOR_OBJECT(sqrt, DEIRECTION);                                      \
DEFINE_UNARY_FOR_OBJECT(sin, DEIRECTION);                                       \
DEFINE_UNARY_FOR_OBJECT(cos, DEIRECTION);                                       \
DEFINE_UNARY_FOR_OBJECT(tan, DEIRECTION);                                       \
DEFINE_UNARY_FOR_OBJECT(log, DEIRECTION);                                       \
DEFINE_UNARY_FOR_OBJECT(exp, DEIRECTION);                                       \
DEFINE_UNARY_FOR_OBJECT(erf, DEIRECTION);                                       \
bp::def("to_double", &::##DEIRECTION##_to_double);

double forward_to_double(const ad::bu::variable<double, double>& v)
{
    return static_cast<double>(v);
}

void export_bottomup() { 
    DEFINE_PYTHON_SCOPE(bottomup)

    DEFINE_VARIABLE_MANAGER(bu);
    DEFINE_VARIABLE(bu, forward);
    
    using derivative
        = ad::bu::derivative<double, double>;
    bp::class_<derivative>("Derivative", bp::init<variable>())
        .def("d", static_cast<double(derivative::*)(const variable&) const>(&derivative::d));
    bp::def("d", &ad::bu::d<double, double>);
}

double reverse_to_double(const ad::td::variable<double, double>& v)
{
    return static_cast<double>(v);
}

void export_topdown()
{
    DEFINE_PYTHON_SCOPE(topdown)

    DEFINE_VARIABLE_MANAGER(td);
    DEFINE_VARIABLE(td, reverse);

    using gradient
        = ad::td::gradient_holder<double, double>;
    bp::class_<gradient>("Gradient", bp::init<variable, typename gradient::gradient_data>())
        .def("in_direction_of", static_cast<double(gradient::*)(const variable&) const>(&gradient::in_direction_of));
    bp::def("gradient", &ad::td::gradient<double, double>);
}

#ifdef _DEBUG
BOOST_PYTHON_MODULE(adpyd)
#else
BOOST_PYTHON_MODULE(adpy)
#endif
{
    bp::object package = bp::scope();
#ifdef _DEBUG
    package.attr("__path__") = "adpyd";
#else
    package.attr("__path__") = "adpy";
#endif

    export_bottomup();
    export_topdown();
}
