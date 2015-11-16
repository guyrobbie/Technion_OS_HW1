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
extern int Last_terminated_process;
extern int g_process_terminated;
extern int susp;
extern LIST_ELEMENT* JobsList;
extern char* L_Fg_Cmd;
extern int g_forground_pID;

void handle_SIGTSTP(int sig_num) //handles CTRL-Z action
{
	if(g_forground_pID <= 0)
		return;
	InsertElem(&JobsList, L_Fg_Cmd, 1, g_forground_pID, 1);
	printf("\nsignal SIGTSTP was sent to pID: %d \n", g_forground_pID);
	kill(g_forground_pID,SIGTSTP);
}

void handle_SIGINT(int sig_num) //handles CTRL-C action
{
	if(g_forground_pID <= 0)
		return;
	DelPID(&JobsList, g_forground_pID);
	printf("\nsignal SIGINT was sent to pID: %d \n", g_forground_pID);
	kill(g_forground_pID,SIGINT);
} 

void handle_SIGCHLD(int sig_num)
{
	int stat = 0;
	int pid = waitpid(-1, &stat, WNOHANG);
	if(pid != 0)
	{
		if(Last_terminated_process == pid)
		{
			if((WIFSIGNALED(stat) == 1) && (WTERMSIG(stat) == SIGTERM))
				g_process_terminated = 1;
		}
		DelPID(&JobsList, pid);
	}
}
