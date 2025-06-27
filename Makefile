# Makefile for main.cpp

CXX = g++
CXXFLAGS = -std=c++11 `pkg-config --cflags opencv4`
LDFLAGS = `pkg-config --libs opencv4` -lncurses

TARGET = main
SRCS = main.cpp ARUCO.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run:
	./$(TARGET)
