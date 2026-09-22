.PHONY: clean folders run

CC=cc
EXEC=main.run

SRC=src/*.c include/miniaudio/miniaudio.c
INCLUDE=-I./include/
LIBS=-lm
CFLAGS=-Wall -Wextra -Wpedantic

all: folders
	$(CC) $(CFLAGS) -o build/$(EXEC) $(SRC) $(INCLUDE) $(LIBS)

clean:
	rm -rf build/*

folders:
	mkdir -p src obj include build

run:
	@./build/$(EXEC)
