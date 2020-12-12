CXX		  := arm-linux-gnueabihf-g++-8
CXX_FLAGS := -Wall -Wextra -Wno-psabi -pthread -std=c++17 -ggdb

BIN		:= bin
SRC		:= src
INCLUDE	:= include -Iheaders
LIB		:= lib

LIBRARIES	:=
EXECUTABLE	:= proj_sys_rpi_app


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp $(SRC)/Websocket/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*

