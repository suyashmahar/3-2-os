all: diningPhilosopher boundedBufferMult cigaretteSmokers

CXXFLAGS := $(CXXFLAGS)-Wno-write-strings

diningPhilosopher: assignment7.cc
	$(CXX) $(CXXFLAGS) assignment7.cc -lpthread -o phil.out -I.

boundedBufferMult: bbsem_mult.cpp
	$(CXX) $(CXXFLAGS) bbsem_mult.cpp -lpthread -o bbsem_mult.out -I.

cigaretteSmokers: cigsmoker.cpp
	$(CXX) $(CXXFLAGS) cigsmoker.cpp -lpthread -o cigsmoker.out -I.
