CC=mpicxx
FLAGS=-std=c++11 -fopenmp -O3
INCLUDE= -Iinclude
EXE=main
Q=@

SOURCES=$(shell find -name "*.cpp")
OBJECTS=$(SOURCES:.cpp=.o)

.PHONY: all clean

all: $(EXE)

.cpp.o:
	$(Q)echo CXX $<
	$(Q)$(CC) $(FLAGS) $(INCLUDE) -c $^ -o $@

$(EXE): $(OBJECTS)
	$(Q)echo CXX $@
	$(Q)$(CC) $(FLAGS) -o $@ $^

clean: 
	$(Q)rm -f $(OBJECTS) $(EXE)
