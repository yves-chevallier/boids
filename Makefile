CXX=g++
CPPFLAGS=-std=c++14
LDFLAGS=-g
LDLIBS=-lsfml-graphics -lsfml-window -lsfml-system -lpthread

SRCS=$(wildcard *.cpp)
OBJS=$(notdir $(SRCS:.cpp=.o))

all: boids

boids: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDLIBS)

%.o: %.cpp
	$(CXX) -c $(LDFLAGS) $(CPPFLAGS) $< 

clean:
	$(RM) $(OBJS)
