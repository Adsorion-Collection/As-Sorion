SOURCES := $(shell find -name *.c)
OBJECTS := $(SOURCES:.c=.o)

CC = gcc

EXEC = as-sorion

run: all
	./$(EXEC)

all: $(OBJECTS)
	$(CC) $^ -o $(EXEC) -Isrc -lgcc -Werror -Wall

%.o:%.c
	$(CC) -c $^ -o $@ -Isrc -lgcc -Wall -Werror

clean:
	rm -rf $(OBJECTS) $(EXEC)