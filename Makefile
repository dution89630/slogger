CFLAGS = -g -O2 -Wall 
OBJ = slog.o
CC=gcc
example: slog_t.c $(OBJ)
	$(CC) $(CFLAGS) -I.  -o example slog_t.c $(OBJ) -lpthread

slog.o: slog.h
	$(CC) $(CFLAGS) -I. -c slog.c
.PHONY: clean

clean:
	$(RM) example $(OBJ)
