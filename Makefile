CXX = g++
CXXFLAGS = -std=c++17 -I./include
LIBS = -L./lib -lsfml-graphics -lsfml-window -lsfml-system

main: src/main.cpp
	$(CXX) $(CXXFLAGS) -o main src/main.cpp $(LIBS)

run: main
	./main

clean:
	rm -f main
