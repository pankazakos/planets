LIBS = -lGL -lGLU -lglfw -lXrandr -lX11 -lrt -ldl -lassimp
BIN = ./bin
BUILD = ./build
INCLUDE = ./include
SRC = ./src
FLAGS = -Wall

CXX = g++

NAME = planets
GLAD = ./include/glad/glad.c
SRCS = $(SRC)/planets.cpp $(GLAD)
OBJS = $(BUILD)/planets.o $(BUILD)/glad.o

.PHONY: clean

all: $(OBJS)
	$(CXX) $(BUILD)/planets.o $(BUILD)/glad.o -I $(INCLUDE) $(LIBS) -o $(BIN)/planets

run: all
	$(BIN)/$(NAME)

clean:
	rm -f $(BIN)/* $(BUILD)/*

$(BUILD)/planets.o: $(SRC)/planets.cpp
	$(CXX) $(SRC)/planets.cpp -c -I $(INCLUDE) -o $(BUILD)/planets.o  $(FLAGS)

$(BUILD)/glad.o: $(GLAD)
	$(CXX) $(GLAD) -c -I $(INCLUDE) -o $(BUILD)/glad.o  $(FLAGS)
