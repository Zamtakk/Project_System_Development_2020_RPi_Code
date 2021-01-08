CXX		  := arm-linux-gnueabihf-g++-8
CXX_FLAGS := -Wall -Wextra -Wno-psabi -pthread -std=c++17 -ggdb

BIN		:= bin
OBJS	:= compiled
SRC		:= src
INCLUDE	:= include -Iheaders

EXECUTABLE	:= proj_sys_rpi_app


# all: $(BIN)/$(EXECUTABLE)

# $(BIN)/$(EXECUTABLE): $(SRC)/*.cpp $(SRC)/Websocket/*.cpp $(SRC)/Devices/*.cpp
# 	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -o $@ $^

# $(COMP)/%.o: 

# clean:
# 	-rm $(BIN)/*



all : $(BIN)/$(EXECUTABLE)
  echo All done

$(BIN)/$(EXECUTABLE) : $(OBJS)/*.o
  $(CXX) $(CXX_FLAGS) -o $@ $^

%.o : %.cpp
  $(CXX) $(CXX_FLAGS) -o $@ -c $<

%.o : %.h
