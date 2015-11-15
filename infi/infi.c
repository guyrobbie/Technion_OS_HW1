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
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
	signal(SIGTERM, SIG_IGN);
	while(1);
	return 0;
}
