COMPILER = g++
OPTIONS = -I ./src -o httpcli
SOURCE_FILES = $(shell find ./src -name '*.cpp')
OUTPUT_FOLDER = ./bin

.PHONY: all
all:
	$(COMPILER) $(OPTIONS) $(SOURCE_FILES)
