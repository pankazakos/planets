LIBS = -lGL -lGLU -lglfw -lXrandr -lX11 -lrt -ldl -lassimp
GLAD = /usr/include/glad/glad.c
BIN = ./bin
INCLUDE = ./include
SRC = ./src
PROGRAM_PATH = $(SRC)
FLAGS = -Wall

NAME = planets

all:
	g++ $(PROGRAM_PATH)/$(NAME).cpp $(GLAD) -o $(BIN)/$(NAME) $(LIBS) -I $(INCLUDE) $(FLAGS)

run: all
	$(BIN)/$(NAME)

clean:
	rm -f $(BIN)/*
