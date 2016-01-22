#ifndef _SIGS_H
#define _SIGS_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <signal.h>
#include "list.h"

void handle_SIGTSTP(int sig_num); 	//handles CTRL-Z action
void handle_SIGINT(int sig_num); 	//handles CTRL-C action
void handle_SIGCHLD(int sig_num);	//handles Signals sends by Childs that had stopped or terminated
#endif

