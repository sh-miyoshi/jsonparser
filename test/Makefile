CXX = g++
ifeq ($(OS),Windows_NT)
  TARGET = a.exe
else
  TARGET = a.out
endif
CXXFLAGS = -O0
SRCS = test.cpp ../jsonparser.cpp
OBJS := $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)
clean:
	rm -f $(TARGET) $(OBJS)

# Header Dependencies
main.o: ../jsonparser.h
jsonparser.o: ../jsonparser.h