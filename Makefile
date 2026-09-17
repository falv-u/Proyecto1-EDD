.PHONY: clean folders run send
CC=cc
EXEC=main.run

SRC_DIR=src
OBJ_DIR=obj
SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
OBJ_FILES=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
INCLUDE=-I./include/
#LIBS= -lm -lp

CFLAGS=-Wall -Wextra -Wpedantic
LDFLAGS= -Wall -lm

all: folders $(OBJ_FILES)
	$(CC) $(CFLAGS) -o build/$(EXEC) $(OBJ_FILES) $(INCLUDE) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(INCLUDE)


clean:
	rm -f $(OBJ_FILES)
	rm -rf build/*

folders:
	mkdir -p src obj include build

run:
	@./build/$(EXEC)

