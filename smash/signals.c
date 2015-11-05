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
