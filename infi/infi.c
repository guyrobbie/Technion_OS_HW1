/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

//**************************************************************************************
// function name: main
// Description: main function of infi, the purpose of this program is to run 
//              infinite loop so we can test functionalities of smash program
//              in addition it ignores SIGTERM signal, (CTRL + C)
//**************************************************************************************
int main(int argc, char *argv[])
{
	signal(SIGTERM, SIG_IGN);
	while(1);
	return 0;
}
