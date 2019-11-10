CXX      = g++-8
CXXFLAGS = -std=c++11 -O3
DEBUG    = -Wall -g3
OBJS     = $(patsubst %.cpp, %.o, $(wildcard */*.cpp))
PROGRAM  = Calculator

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) -o $(PROGRAM)

clean:; rm -f *.o */*.o *~ $(PROGRAM)
