CXX = g++
CXXFLAGS = -lgtest -lgtest_main -pthread -std=c++0x

all:
	$(CXX) main-test.cpp $(CXXFLAGS) -o main-test


#CXX = g++
#CXXFLAGS = -g -L/opt/gtest/lib -lgtest -lgtest_main -pthread -std=c++0x
#INCS = -I./ -I.. -I/opt/gtest/include
#OBJS = ../drinking philosopher-test table-test

#main-test: main-test philosopher-test table-test
#	$(CXX) -o main-test.cpp $(OBJS) $(CXXFLAGS)

#.cc.o:
#	$(CXX) -c $< -o $@ $(INCS)

#clean:
#	rm main-test *.o main-test.xml