CC = gcc
CFLAGS = -std=c2x -Wall -Werror
LDLIBS = -lm
OUTPUT = ps1

OBJ = bmp.o main.o playfair.o

all: $(OUTPUT)

$(OUTPUT): $(OBJ)
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OUTPUT) $(OBJ)
