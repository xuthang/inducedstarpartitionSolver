CXXFLAGS =-Wall -pedantic -O2 -g -std=c++17

# checking if user has gurobi installed, if not then excluding in compilation
GUROBI		 = $(shell command -v gurobi.sh >/dev/null && echo 1 || echo 0)
LOADLIBES    = $(shell [ $(GUROBI) = 1 ] && echo -lgurobi_c++ -lgurobi95 )
INCLUDES 	 = $(shell [ $(GUROBI) = 1 ] && echo "-I$$HOME/gurobi952/linux64/include/")
CXXFLAGS 	+= $(shell [ $(GUROBI) = 1 ] && echo -D__GUROBI__)

GTEST := -lgtest -pthread

DEPFLAGS     = -MT $@ -MMD -MP -MF ./build/$*.d
MAIN_FILES   = $(shell grep -lr 'int main' . | grep 'cpp$$')
OTHER_FILES  = $(shell grep -Lr 'int main' . | grep 'cpp$$')
SOURCE_FILES = $(OTHER_FILES) $(MAIN_FILES)

EXE_FILES    = $(MAIN_FILES:./%.cpp=./exe/%)
TEST_EXE_FLS = $(filter %test, $(EXE_FILES))
RUN_EXE_FLS  = $(filter-out %test, $(EXE_FILES))

RUN_TESTS = $(addprefix run_, $(TEST_EXE_FLS))

OBJECT_FILES = $(OTHER_FILES:./%.cpp=./build/%.o)
HEADER_FILES = $(OTHER_FILES:.cpp=.hpp)
DEPENDENCIES = $(SOURCE_FILES:./%.cpp=./build/%.d)

notests: $(RUN_EXE_FLS)

compile: $(EXE_FILES)

./build/%.o: ./%.cpp ./build/%.d
	@mkdir -p `dirname $@`
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c -o $@ $< $(INCLUDES)

$(RUN_EXE_FLS): ./exe/%: ./%.cpp $(OBJECT_FILES)
	@mkdir -p `dirname $@`
	$(CXX) $(CXXFLAGS) -o $@ $< $(OBJECT_FILES) $(LOADLIBES) 

$(TEST_EXE_FLS): ./exe/%: ./%.cpp $(OBJECT_FILES)
	@mkdir -p `dirname $@`
	$(CXX) $(CXXFLAGS) -o $@ $< $(OBJECT_FILES) $(LOADLIBES) $(GTEST)

run: ./exe/main
	./$< instances/vc-exact_006.gr

debug: $(EXE_FILES)
	valgrind ./$<

.PHONY: $(RUN_TESTS)
$(RUN_TESTS):run_%:%
	./$<

.PHONY: tests
tests: $(RUN_TESTS)


.PHONY : clean
clean :
	rm -rf build/ exe/

$(DEPENDENCIES):
include $(wildcard $(DEPENDENCIES))
