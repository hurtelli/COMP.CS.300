####### Compiler, tools and options

CXX           = g++
CXXFLAGS      = -std=c++17 -Wall -W

####### Output directory

SOURCES       = iteration1.cc iteration2.cc iteration3.cc iteration4.cc
OBJECTS       = iteration1.o iteration2.o iteration3.o iteration4.o
TARGET        = test_iteration


test_iteration: test.hh test.cc $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) test.cc $(OBJECTS)

iteration1.o: iteration1.cc
	$(CXX) -c $(CXXFLAGS) -o iteration1.o iteration1.cc

iteration2.o: iteration2.cc
	$(CXX) -c $(CXXFLAGS) -o iteration2.o iteration2.cc

iteration3.o: iteration3.cc
	$(CXX) -c $(CXXFLAGS) -o iteration3.o iteration3.cc

iteration4.o: iteration4.cc
	$(CXX) -c $(CXXFLAGS) -o iteration4.o iteration4.cc

clean:
	rm -f $(OBJECTS) $(TARGET) *~ core *.core

