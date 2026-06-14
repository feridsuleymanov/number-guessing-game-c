# Makefile — Library Book Manager

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -pedantic
TARGET  = library_manager
SRC     = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) library.dat

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
