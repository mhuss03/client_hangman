CC = gcc

SRC = src/main.c src/client.c src/display.c src/hangman_utils.c
EXEC = hangman_client

OBJ = $(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

