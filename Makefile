CXX := arm-linux-gnueabihf-g++-8
CXX_FLAGS := -Wall -Wextra -Wno-psabi -pthread -std=c++17 -ggdb
INCLUDE	:= -Iinclude -Iheaders
TARGET_FOLDER := bin
TARGET := proj_sys_rpi_app

# $(wildcard *.cpp /xxx/xxx/*.cpp): get all .cpp files from the current directory and dir "/xxx/xxx/"
SRCS := $(wildcard src/*.cpp src/Devices/*.cpp src/Websocket/*.cpp)
# $(patsubst %.cpp,%.o,$(SRCS)): substitute all ".cpp" file name strings to ".o" file name strings
OBJS := $(patsubst %.cpp,obj/%.o,$(SRCS))

all: $(TARGET_FOLDER)/$(TARGET)

$(TARGET_FOLDER)/$(TARGET): $(OBJS)
	$(CXX) $(CXX_FLAGS) $(INCLUDE) -o $@ $^

obj/%.o: %.cpp
	$(CXX) $(CXX_FLAGS) $(INCLUDE) -c $< -o $@

clean:
	find . -name '*.o' -delete
	find ./$(TARGET_FOLDER) -name '$(TARGET)' -delete
	
.PHONY: all clean