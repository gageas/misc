/*
	get screen width&height
	cc t.c -lcurses
*/

#include <curses.h>
#include <term.h>
#include <stdio.h>

int main(void){
	setupterm(NULL,1,NULL);
 	printf("%dx%d\n",columns,lines);
	return 0;
}
