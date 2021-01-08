CXX := arm-linux-gnueabihf-g++-8
CXX_FLAGS := -Wall -Wextra -Wno-psabi -pthread -std=c++17 -ggdb
INCLUDE	:= -Iinclude -Iheaders
TARGET := bin/proj_sys_rpi_app

# $(wildcard *.cpp /xxx/xxx/*.cpp): get all .cpp files from the current directory and dir "/xxx/xxx/"
SRCS := $(wildcard src/*.cpp src/Devices/*.cpp src/Websocket/*.cpp)
# $(patsubst %.cpp,%.o,$(SRCS)): substitute all ".cpp" file name strings to ".o" file name strings
OBJS := $(patsubst %.cpp,%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXX_FLAGS) $(INCLUDE) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(TARGET) *.o
	
.PHONY: all clean