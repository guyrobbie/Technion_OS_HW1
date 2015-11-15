// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"

extern int GPid;
extern int Last_Bg_Pid;
extern int Susp_Bg_Pid;
extern int susp;
extern LIST_ELEMENT* JobsList;
extern char* L_Fg_Cmd;
extern int g_forground_pID;

void handle_SIGTSTP(int sig_num) //handles CTRL-Z action
{
	InsertElem(&JobsList, L_Fg_Cmd, 1, g_forground_pID, 1);
	printf("\nDEBUG: L_Fg_Cmd %s \n", L_Fg_Cmd);
	printf("\nDEBUG: g_forground_pID %d \n", g_forground_pID);
	printf("\nsignal %d was sent to pID: %d \n", SIGTSTP, g_forground_pID);
	kill(g_forground_pID,SIGTSTP);
}

void handle_SIGCHLD(int sig_num)
{
	int stat = 0;
	int pid = waitpid(-1, &stat, WNOHANG);
	if(pid != 0)
	{
		printf("pid: %d\n", pid);
		printf("DEBUG: child process send SIGCHLD signal\n");	
		DelPID(&JobsList, pid);
	}
	
	
}
