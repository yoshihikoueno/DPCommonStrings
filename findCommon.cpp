#include <iostream>
#include <tuple>
#include <string.h>
#include <cstdlib>
#include <map>
#include "utf8.h"
#include "boost/python.hpp"
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

using namespace std;

vector<tuple<int, int, int>> find_common(char* string0, char* string1, int minLength){
    vector<int> cp_string0, cp_string1;
    int len_str0 = strlen(string0);
    int len_str1 = strlen(string1);

    vector<tuple<int, int, int>> matches;
    map<tuple<int, int>, int> match_map;

    int cp=0;
    char* it;
    for (it=string0; it-string0<len_str0;){
        cp=utf8::next(it, string0+len_str0);
        cp_string0.push_back(cp);
    }
    for (it=string1; it-string1<len_str1;){
        cp=utf8::next(it, string1+len_str1);
        cp_string1.push_back(cp);
    }

    int n_chars0 = cp_string0.size();
    int n_chars1 = cp_string1.size();
    vector<vector<int>> matrix = vector<vector<int>>(n_chars0, vector<int>(n_chars1, 0));
    for (int i=0; i< n_chars1 ; i++){
        for (int j=0; j< n_chars0 ; j++){
            if (cp_string0[j] == cp_string1[i]){
                int size = 0;
                if(i>0 && j>0 && matrix[j-1][i-1] != 0){
                    size = matrix[j][i] = abs(matrix[j-1][i-1]) + 1;
                    matrix[j-size+1][i-size+1] = -matrix[j][i];
                }
                else if(i>0 && j>0 && matrix[j-1][i-1] ==0){
                    size = 1;
                    matrix[j][i] = -1;
                }
                else{
                    size = 1;
                    matrix[j][i] = -1;
                }
                if (size >= minLength) match_map[tuple<int, int>(j-size+1, i-size+1)] = size;
            }
        }
    }
    //for (int i=0; i< n_chars1 ; i++){
    //    for (int j=0; j< n_chars0 ; j++){
    //        cout << matrix[j][i];
    //        cout << ' ';
    //    }
    //    cout << endl;
    //}
    //cout << endl;

    for (auto v : match_map)
        matches.push_back(tuple<int,int,int>(get<0>(v.first), get<1>(v.first), v.second));

    return matches;
}

// Derived from https://stackoverflow.com/questions/42186986/boostpython-converting-tuple-to-python-works-vectortuple-does-not
template <typename A>
int tuple_length(const A&)
{
    return std::tuple_size<A>::value;
}

template <int cidx, typename ... A>
typename std::enable_if<cidx >= sizeof...(A), boost::python::object>::type
get_tuple_item_(const std::tuple<A...>& a, int idx, void* = nullptr)
{
    throw std::out_of_range{"Ur outta range buddy"};
}

template <int cidx, typename ... A, typename = std::enable_if<(cidx < sizeof ...(A))>>
typename std::enable_if<cidx < sizeof...(A), boost::python::object>::type
get_tuple_item_(const std::tuple<A...>& a, int idx, int = 42)
{
    if (idx == cidx)
        return boost::python::object{std::get<cidx>(a)};
    else
        return get_tuple_item_<cidx+1>(a, idx);
};

template <typename A>
boost::python::object get_tuple_item(const A& a, int index)
{
    return get_tuple_item_<0>(a, index);
}

BOOST_PYTHON_MODULE(findCommon){
    using boost::python::class_;
    using boost::python::def;
    using boost::python::init;
    using boost::python::vector_indexing_suite;

    class_<vector<tuple<int, int, int> > >("TupleVector")
        .def(vector_indexing_suite<vector<tuple<int, int, int> > >());

    class_<tuple<int, int, int>>("tuple<int, int, int>", init<int, int, int>())
        .def("__len__", &tuple_length<tuple<int, int, int>>)
        .def("__getitem__", &get_tuple_item<tuple<int, int, int>>)
        ;

    def("findCommon", find_common);
}
