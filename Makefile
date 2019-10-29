CXX = g++
ifeq ($(OS),Windows_NT)
  TARGET = a.exe
else
  TARGET = a.out
endif
CXXFLAGS = -O2
SRCS = main.cpp
OBJS := $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)
clean:
	rm -f $(TARGET) $(OBJS)

# Header Dependencies
