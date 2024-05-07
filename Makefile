COMPILER = g++
BIN_NAME = httpcli
OPTIONS = -I ./src -o $(BIN_NAME)
SOURCE_FILES = $(shell find ./src -name '*.cpp')
OUTPUT_FOLDER = ./bin

.PHONY: all
all:
	$(COMPILER) $(OPTIONS) $(SOURCE_FILES)
	valgrind --log-file="file" --leak-check=full ./$(BIN_NAME)
