CC=gcc
CFLAGS=-Wall -Werror
SRC_DIR=src
BLD_DIR=build
SOURCES=`find $(SRC_DIR)/*.c`
TARGET=$(BLD_DIR)/going


all: going

clean:
	rm -rf $(BLD_DIR)

going: prepare
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

prepare:
	mkdir -p $(BLD_DIR)

