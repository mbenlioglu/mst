.PHONY			: all clean _test _test.cmake
.SUFFIXES		: .cmake

SRC_DIR			:= src
TEST_DIR		:= test
_SOURCES		:= main.cpp
SOURCES			:= $(patsubst %,$(SRC_DIR)/%,$(_SOURCES))
_TEST_SOURCES	:= test_mst.cpp
TEST_SOURCES	:= $(patsubst %,$(TEST_DIR)/%,$(_TEST_SOURCES))

OBJDIR			:= build/obj
TEST_BUILD_DIR	:= $(OBJDIR)/$(TEST_DIR)
_SRC_OBJ		:= main.o
SRC_OBJ			:= $(patsubst %,$(OBJDIR)/%,$(_SRC_OBJ))
_TESTS			:= $(basename $(_TEST_SOURCES))
TESTS			:= $(patsubst %,$(TEST_BUILD_DIR)/%,$(_TESTS))


CXX_FLAGS		:= -std=c++17 -O3 -march=native -Iinclude -Isrc
LD_FLAGS		:=
CMAKE_VERSION := $(shell cmake --version | head -n1 | awk '{print $$NF; print 3.13}' | sort -V | head -n1)

ifeq ($(CMAKE_VERSION),3.13)
all: mst.cmake
test: _test.cmake
else
all: mst
test: _test
endif

$(OBJDIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS) $(OBJDIR)
	@g++ $(CXX_FLAGS) -c -o $@ $< $(LD_FLAGS)

$(TEST_BUILD_DIR)/%: $(TEST_DIR)/%.cpp $(DEPS) $(TEST_BUILD_DIR)
	@g++ $(CXX_FLAGS) -o $@ $< $(LD_FLAGS)

mst: $(SRC_OBJ)
	@g++ $(CXX_FLAGS) $^ -o $@ $(LD_FLAGS)

mst.cmake: $(TEST_SOURCES) $(SOURCES) CMakeLists.txt
	@mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j && cd .. && ln -fs build/$* .; touch mst.cmake

_test: $(TESTS) mst
	@for f in $(patsubst %.out,%,$(shell ls test/results)); do echo Test $(basename $<)-$$f\:; $< data/$$f.gr data/$$f.extra test/results/$$f.out && echo Passed || echo Failed; done

_test.cmake: mst.cmake
	@cd build && ctest -j 8; cd ..

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(TEST_BUILD_DIR):
	@mkdir -p $(TEST_BUILD_DIR)

clean:
	@rm -rf *.o mst mst.cmake build
