// Compile with:
// clang++ -std=c++11 -shared -l boost_python3 -I /usr/include/python3.2mu -fPIC -o bptuple.so tuple-test.cpp
//
// NOTICE
// This sourcecode is derived from https://gist.github.com/niwinz/3729459 (2019/06/06)

#include <tuple>
#include <string>
#include <boost/python.hpp>

namespace py = boost::python;

using std::string;

template<int ...> struct seq{};
template<int N, int ...S> struct gens : gens<N-1, N-1, S...>{};
template<int ...S> struct gens<0, S...> {typedef seq<S...> type;};

template <typename ...Args>
struct cpptuple2pytuple_wrapper {
    std::tuple<Args...> params;
    cpptuple2pytuple_wrapper(const std::tuple<Args...>& _params):params(_params){}

    py::tuple delayed_dispatch() {
        return callFunc(typename gens<sizeof...(Args)>::type());
    }

    template<int ...S>
    py::tuple callFunc(seq<S...>) {
        return py::make_tuple(std::get<S>(params) ...);
    }
};

template <typename ...Args>
struct pytuple2cpptuple_wrapper {
    py::tuple params;
    pytuple2cpptuple_wrapper(const py::tuple& _params):params(_params){}

    std::tuple<Args...> delayed_dispatch() {
        return callFunc(typename gens<sizeof...(Args)>::type());
    }

    template<int ...S>
    std::tuple<Args...> callFunc(seq<S...>) {
        return std::make_tuple((static_cast<Args>(py::extract<Args>(params[S])))...);
    }
};

// Convert (C++) tuple to (Python) tuple as PyObject*.
template<typename ... Args> PyObject* cpptuple2pytuple(const std::tuple<Args...>& t) {
    cpptuple2pytuple_wrapper<Args...> wrapper(t);
    py::tuple bpt = wrapper.delayed_dispatch();
    return py::incref(py::object(bpt).ptr());
}

// Convert (Python) tuple to (C++) tuple.
template<typename ... Args> std::tuple<Args...> pytuple2cpptuple(PyObject* obj) {
    py::tuple tup(py::borrowed(obj));
    pytuple2cpptuple_wrapper<Args...> wrapper(tup);
    std::tuple<Args...> bpt = wrapper.delayed_dispatch();
    return bpt;
}

template<typename ... Args>
struct cpptuple_to_python_tuple {
    static PyObject* convert(const std::tuple<Args...>& t) {
        return cpptuple2pytuple<Args...>(t);
    }
};

template<typename ... Args>
struct cpptuple_from_python_tuple {
    cpptuple_from_python_tuple() {
        py::converter::registry::push_back(&convertible, &construct, py::type_id<std::tuple<Args...> >());
    }

    static void* convertible(PyObject* obj_ptr) {
        if (!PyTuple_CheckExact(obj_ptr)) return 0;
        return obj_ptr;
    }

    static void construct(PyObject* obj_ptr, py::converter::rvalue_from_python_stage1_data* data) {
        void* storage = ((py::converter::rvalue_from_python_storage<std::tuple<Args...> >*)data)->storage.bytes;
        new (storage) std::tuple<Args...>(pytuple2cpptuple<Args...>(obj_ptr));
        data->convertible = storage;
    }
};

template<typename ...Args> void create_tuple_converter() {
    py::to_python_converter<std::tuple<Args...>, cpptuple_to_python_tuple<Args...> >();
    cpptuple_from_python_tuple<Args...>();
}

void export_cpptuple_conv() {
    create_tuple_converter<int, float>();
    create_tuple_converter<int, double, string>();
}

std::tuple<int, float> tupid1(std::tuple<int, float> t){return t;}
std::tuple<int, double, string> tupid2(std::tuple<int, double, string> t){return t;}

BOOST_PYTHON_MODULE(bptuple) {
    export_cpptuple_conv();
    py::def("tupid1", tupid1);
    py::def("tupid2", tupid2);
}
