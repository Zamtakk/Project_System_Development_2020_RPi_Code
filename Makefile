
# all: $(BIN)/$(EXECUTABLE)

# $(BIN)/$(EXECUTABLE): $(SRC)/*.cpp $(SRC)/Websocket/*.cpp $(SRC)/Devices/*.cpp
# 	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -o $@ $^

# $(COMP)/%.o: 

# clean:
# 	-rm $(BIN)/*



# all: $(BIN)/$(EXECUTABLE)
# 	echo All done

# $(BIN)/$(EXECUTABLE): *.o
# 	$(CXX) $(CXX_FLAGS) -o $@ $^

# $(OBJS)/%.o: %.cpp
# 	$(CXX) $(CXX_FLAGS) -o $@ -c $<

# %.o : %.h




# SRCDIR := src
# OBJDIR := obj
# DEPDIR := dep

# SRCS     := $(shell find $(SRCDIR) -name "*.cpp")
# OBJS     := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
# DEPS     := $(SRCS:$(SRCDIR)/%.cpp=$(DEPDIR)/%.d)
# TREE     := $(patsubst %/,%,$(dir $(OBJS)))
# CPPFLAGS  = -MMD -MP -MF $(@:$(OBJDIR)/%.o=$(DEPDIR)/%.d)

# .PHONY: all clean

# all: $(BIN)/$(EXECUTABLE)

# $(BIN)/$(EXECUTABLE): $(OBJS)
# 	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -o $@ $^

# $(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $$(@D)
# 	$(CXX) $(CPPFLAGS) $(INCLUDE) $(CXXFLAGS) -o $@ -c $<

# $(TREE): %:
# 	mkdir -p $@
# 	mkdir -p $(@:$(OBJDIR)%=$(DEPDIR)%)

# clean:
# 	$(RM) -r $(OBJDIR) $(DEPDIR)

# ifeq "$(MAKECMDGOALS)" ""
# -include $(DEPS)
# endif



CXX		  := arm-linux-gnueabihf-g++-8
CXX_FLAGS := -Wall -Wextra -Wno-psabi -pthread -std=c++17 -ggdb
# CXX_FLAGS := -Wno-psabi -pthread -std=c++17 -ggdb

BIN		:= bin
# OBJS	:= compiled
# SRC		:= src
INCLUDE	:= -Iinclude -Iheaders

EXECUTABLE	:= proj_sys_rpi_app




CC := arm-linux-gnueabihf-g++-8
CFLAGS := -Wall -Wextra -Wno-psabi -pthread -std=c++17 -ggdb
TARGET := bin/proj_sys_rpi_app

# $(wildcard *.cpp /xxx/xxx/*.cpp): get all .cpp files from the current directory and dir "/xxx/xxx/"
SRCS := $(wildcard src/*.cpp src/Devices/*.cpp src/Websocket/*.cpp)
# $(patsubst %.cpp,%.o,$(SRCS)): substitute all ".cpp" file name strings to ".o" file name strings
OBJS := $(patsubst %.cpp,%.o,$(SRCS))

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $^
%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
clean:
	rm -rf $(TARGET) *.o
	
.PHONY: all clean