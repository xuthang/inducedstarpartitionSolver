# Information
This folder contains the implementation of an algorithm for the induced star partition problem as described in thesis of Ing. Nguyen Xuan Thang - https://dspace.cvut.cz/handle/10467/108886

The implementation is in ``./starPartitioning/``

Check the ``./main.cpp`` function to see how to call the function that implements the algorithm

There is a ``Makefile`` available, there are several compilation modes
* compile everything: ``make``
* compile only the solver: ``make notests``
* run tests: ``make tests``

# installation requirements
C++17 is needed

To invoke the tests, gTests needs to be installed on the system - https://google.github.io/googletest/

There is an option to call the solver without providing a vertex cover, but gurobi needs to be installed - https://www.gurobi.com/

Once gurobi is installed, please make sure the in include location is correct in the ``Makefile``
* the line containing ``echo "-I$$HOME/gurobi952/linux64/include/"`` should lead to the correct position of the gurobi include

# usage
``./exe/main [graph] [optional vertex cover]``

if gurobi is installed, then the program can compute the minimum vertex cover and it does not have to be given on the input

``./exe/main [graph]``

# input format
same as used in PACE challenge - https://pacechallenge.org/2019/vc/vc_format/

Vertex cover format is also the same

# output format 
First line consists of number of stars ``q``

Then follows ``2q`` lines - the partitioning. First comes the number of vertices of the i-th star ``n``, then on the next line follows ``n`` integers
