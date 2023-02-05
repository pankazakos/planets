LIBS = -lGL -lGLU -lglfw -lXrandr -lX11 -lrt -ldl -lassimp
GLAD = /usr/include/glad/glad.c
BIN = ./bin
INCLUDE = ./include
SRC = ./src
FLAGS = -Wall

NAME = planets

all:
	g++ $(SRC)/$(NAME).cpp $(GLAD) -o $(BIN)/$(NAME) $(LIBS) -I $(INCLUDE) $(FLAGS)

run: all
	$(BIN)/$(NAME)

clean:
	rm -f $(BIN)/*
