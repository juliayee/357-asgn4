CC = gcc
CFLAGS = -Wall -pedantic -g
CTEST = -Wall -pedantic -g -D TEST
NAME = mytar

all: $(NAME) $(NAME)test

$(NAME): $(NAME).c
	$(CC) $(CFLAGS) -o $(NAME) $(NAME).c

$(NAME)test: $(NAME)
	$(CC) $(CTEST) -o $(NAME)test $(NAME).c

clean:
	rm -f $(NAME) $(NAME)test
