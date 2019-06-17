findCommon.so: findCommon.cpp
	g++ findCommon.cpp -fPIC -DPIC -Ofast -shared -I`python -c 'from distutils.sysconfig import*; print(get_python_inc())'` -lboost_python -o findCommon.so

test: findCommon.so
	python -c 'import findCommon'
