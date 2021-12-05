CXX=clang++
CPPFLAGS=-std=c++17
LDFLAGS=-g -pedantic -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function
LDLIBS=$(shell pkg-config sfml-graphics --libs)

SRCS=$(wildcard *.cpp)
OBJS=$(notdir $(SRCS:.cpp=.o))

all: boids

boids: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDLIBS)

%.o: %.cpp
	$(CXX) -c $(LDFLAGS) $(CPPFLAGS) $< 

clean:
	$(RM) $(OBJS)
