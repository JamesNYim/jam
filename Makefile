CC = gcc
CFLAGS = -std=c17 -Wall
DEBUGFLAGS = -g
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

EXECS = jam 

.PHONY: all clean debug

all: $(EXECS)

clean:
	rm -f $(OBJ) $(EXECS)

debug:	$(CFLAGS) += $(DEBUGFLAGS)

debug: clean all

$(EXECS): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXECS) 

%.o: %.c
	$(CC) $(CFLAGS) -c $<
