CC=gcc
CFLAGS=-Wall -Werror
SRC_DIR=src
SOURCES=`find $(SRC_DIR)/*`
TARGET=going


all: going

clean:
	rm $(TARGET)	

going:
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

