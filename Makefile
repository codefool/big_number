#
# Makefile for ROECE
#
# This lives at https://github.com/codefool/big_number
#
INC_DIR := ./include
SRC_DIR := ./src
OBJ_DIR := ./obj
LIB_DIR := ./lib
LIB_NAME := $(LIB_DIR)/libbigly
# LIB_INC := /usr/local/libcf/lib/libcf

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
HDR := $(wildcard $(INC_DIR)/*.h)

CC := g++
#CFLAGS := -g -std=c++2a -I$(INC_DIR) -I/usr/local/libcf/include
CFLAGS := -g -std=c++23 -I$(INC_DIR)
ARC := ar
AFLAGS := rvs

# garth : garth.cpp garth.h $(HDR) $(LIB_NAME)
# 	$(CC) $(CFLAGS) garth.cpp -L/usr/lib/x86_64-linux-gnu -L/usr/local/libcf/lib/libcf $(LIB_NAME) -o $@

$(LIB_NAME) : $(OBJ)
	$(ARC) $(AFLAGS) $@ $(OBJ)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HDR)
	$(CC) $(CFLAGS) -c $< -o $@

build-ver:
	sed -ri 's/(.*)(ver_build = )([0-9]*);/echo "\1\2$$((\3+1));"/ge' $(INC_DIR)/buildinfo.h

clean:
	-rm $(OBJ_DIR)/*.o $(LIB_NAME)

.PHONY: build-ver

main: main.cpp build-ver $(HDR) $(LIB_NAME)
	$(CC) $(CFLAGS) main.cpp -I. -L/usr/lib/x86_64-linux-gnu $(OBJ) $(LIB_NAME) -o main
