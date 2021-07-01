SOURCES := $(shell find -name *.c)
OBJECTS := $(SOURCES:.c=.o)

CC = gcc

EXEC = as-sorion

run: all

all: $(OBJECTS)

%.o:%.c
	$(CC) -c $^ -o $@ -Isrc -lgcc -Wall -Werror

clean:
	rm -rf $(OBJECTS) $(EXEC)