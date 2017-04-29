#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>

#include "ad/bottom_up.h"
#include "ad/top_down.h"

#define DEFINE_BINARY_FOR_OBJECT_AND_DOUBLE(OPERATOR)                           \
.def(double() OPERATOR boost::python::self)                                     \
.def(boost::python::self OPERATOR double())                                     \
.def(boost::python::self OPERATOR boost::python::self)

#define DEFINE_UNARY_FOR_OBJECT(NAME, DEIRECTION)                               \
boost::python::def(                                                             \
    #NAME,                                                                      \
    &ad::tape::NAME<double, double, ad::tape::##DEIRECTION##_calculation_graph>);

double bu_to_double(const ad::bu::variable<double, double>& v)
{
    return static_cast<double>(v);
}

double td_to_double(const ad::td::variable<double, double>& v)
{
    return static_cast<double>(v);
}

namespace bp = boost::python;

void export_bottomup() { 
#ifdef _DEBUG
    bp::object module(bp::handle<>(bp::borrowed(PyImport_AddModule("adpyd.bottomup"))));
#else
    bp::object module(bp::handle<>(bp::borrowed(PyImport_AddModule("adpy.bottomup"))));
#endif
    bp::scope().attr("bottomup") = module;
    bp::scope scope0 = module;

    using variable = ad::bu::variable<double>;
    using variable_manager = ad::tape::variable_manager<double, double, ad::tape::forward_calculation_graph>;
    using derivative = ad::bu::derivative<double, double>;

    bp::class_<variable_manager>("VariableManager")
        .def("to_variable", &variable_manager::to_variable);
    bp::def(
        "create_variable_manager",
        &ad::bu::create_manager<double>);
    
    bp::class_<variable>("Variable")
        .def(-bp::self)
        DEFINE_BINARY_FOR_OBJECT_AND_DOUBLE(+)
        DEFINE_BINARY_FOR_OBJECT_AND_DOUBLE(-)
        DEFINE_BINARY_FOR_OBJECT_AND_DOUBLE(*)
        DEFINE_BINARY_FOR_OBJECT_AND_DOUBLE(/);
    DEFINE_UNARY_FOR_OBJECT(sqrt, forward);
    DEFINE_UNARY_FOR_OBJECT(sin, forward);
    DEFINE_UNARY_FOR_OBJECT(cos, forward);
    DEFINE_UNARY_FOR_OBJECT(tan, forward);
    DEFINE_UNARY_FOR_OBJECT(log, forward);
    DEFINE_UNARY_FOR_OBJECT(exp, forward);
    DEFINE_UNARY_FOR_OBJECT(erf, forward);
    bp::def("to_double", &::bu_to_double);
    
    bp::class_<derivative>("Derivative", bp::init<variable>())
        .def("d", static_cast<double(derivative::*)(const variable&) const>(&derivative::d));
    bp::def("d", &ad::bu::d<double, double>);
}

void export_topdown()
{
#ifdef _DEBUG
    bp::object module(bp::handle<>(bp::borrowed(PyImport_AddModule("adpyd.topdown"))));
#else
    bp::object module(bp::handle<>(bp::borrowed(PyImport_AddModule("adpy.topdown"))));
#endif
    bp::scope().attr("topdown") = module;
    bp::scope scope1 = module;

    using variable = ad::td::variable<double, double>;
    using variable_manager = ad::tape::variable_manager<double, double, ad::tape::reverse_calculation_graph>;
    using gradient = ad::td::gradient_holder<double, double>;

    bp::class_<variable_manager>("VariableManager")
        .def("to_variable", &variable_manager::to_variable);
    bp::def(
        "create_variable_manager",
        &ad::td::create_manager<double>);

    bp::class_<variable>("Variable")
        .def(-bp::self)
        DEFINE_BINARY_FOR_OBJECT_AND_DOUBLE(+)
        DEFINE_BINARY_FOR_OBJECT_AND_DOUBLE(-)
        DEFINE_BINARY_FOR_OBJECT_AND_DOUBLE(*)
        DEFINE_BINARY_FOR_OBJECT_AND_DOUBLE(/ );
    DEFINE_UNARY_FOR_OBJECT(sqrt, reverse);
    DEFINE_UNARY_FOR_OBJECT(sin, reverse);
    DEFINE_UNARY_FOR_OBJECT(cos, reverse);
    DEFINE_UNARY_FOR_OBJECT(tan, reverse);
    DEFINE_UNARY_FOR_OBJECT(log, reverse);
    DEFINE_UNARY_FOR_OBJECT(exp, reverse);
    DEFINE_UNARY_FOR_OBJECT(erf, reverse);
    bp::def("to_double", &::td_to_double);

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
