CXX      = g++
CXXFLAGS = -std=c++11 $(DEBUG)
DEBUG    = -g3 -Wall
OBJS     = $(patsubst %.cpp, %.o, $(wildcard */*.cpp))
PROGRAM  = Calculator

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) -o $(PROGRAM)

clean:; rm -f *.o */*.o *~ $(PROGRAM)