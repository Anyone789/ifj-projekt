TARGET = program

# Kompilátor a príznaky kompilátora
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Zoznam zdrojových súborov a objektových súborov
SRC = expression.c stack.c scanner.c dstring.c
OBJ = $(SRC:.c=.o)

# Výchozí cieľ - preklad kompletného programu
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Pravidlo pre preklad každého .c súboru na .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean-up rule to remove object files and the target
clean:
	rm -f $(OBJ) $(TARGET)
