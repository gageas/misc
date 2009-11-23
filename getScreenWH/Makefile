CC=cc
PREFIX=/usr/local/bin

getScreenWH: getScreenWH.o
	$(CC) -o getScreenWH getScreenWH.o -l ncurses

getScreenWH.o: getScreenWH.c
	$(CC) -c getScreenWH.c

.PHONY:clean install

clean:
	@rm -rf *.o

install:
	@install getScreenWH -o root -g root $(PREFIX)
