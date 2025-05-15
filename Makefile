CXX = g++
CXXFLAGS = -Wall -std=c++14 $(INCLUDE)
INCLUDE = -I include 

TARGET = main
OBJS = bin/main.o bin/utils.o bin/Graph.o bin/GraphGenerator.o bin/RhoGenerator.o bin/PerformanceEvaluation.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

bin/main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o bin/main.o

bin/utils.o: src/utils.cpp include/utils.hpp
	$(CXX) $(CXXFLAGS) -c src/utils.cpp -o bin/utils.o

bin/Graph.o: src/Graph.cpp include/Graph.hpp
	$(CXX) $(CXXFLAGS) -c src/Graph.cpp -o bin/Graph.o

bin/GraphGenerator.o: src/GraphGenerator.cpp include/GraphGenerator.hpp
	$(CXX) $(CXXFLAGS) -c src/GraphGenerator.cpp -o bin/GraphGenerator.o

bin/RhoGenerator.o: src/RhoGenerator.cpp include/RhoGenerator.hpp
	$(CXX) $(CXXFLAGS) -c src/RhoGenerator.cpp -o bin/RhoGenerator.o

bin/PerformanceEvaluation.o: src/PerformanceEvaluation.cpp include/PerformanceEvaluation.hpp
	$(CXX) $(CXXFLAGS) -c src/PerformanceEvaluation.cpp -o bin/PerformanceEvaluation.o

clean:
	rm -f $(TARGET) bin/*.o
