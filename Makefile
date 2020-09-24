.PHONY			: all clean
.SUFFIXES		: .cmake

SRC_DIR			:= src
TEST_DIR		:= test
_SOURCES		:= main.c
SOURCES			:= $(patsubst %,$(SRC_DIR)/%,$(_SOURCES))
_TEST_SOURCES	:=
TEST_SOURCES	:= $(patsubst %,$(SRC_DIR)/%,$(_SOURCES))

OBJDIR			:= build/obj
_OBJ			:= main.o
OBJ				:= $(patsubst %,$(OBJDIR)/%,$(_OBJ))


CXX_FLAGS		:= -std=c++17 -O3 -march=native -Iinclude
LD_FLAGS		:=
CMAKE_VERSION := $(shell cmake --version | head -n1 | awk '{print $$NF; print 3.13}' | sort -V | head -n1)

ifneq ($(CMAKE_VERSION),3.13)
all: mst.cmake
else
all: mst
endif

$(OBJDIR)/%.o: $(SRC_DIR)/%.c $(DEPS) $(OBJDIR)
	@g++ $(CXX_FLAGS) -c -o $@ $< $(LD_FLAGS)

mst: $(OBJ)
	@g++ $(CXX_FLAGS) $^ -o $@ $(LD_FLAGS)

mst.cmake: $(TEST_SOURCES) $(SOURCES) CMakeLists.txt
	@mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j && mv $* .. && cd ..

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@rm -rf *.o mst build
