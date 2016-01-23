#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include "list.h"
#include "vars.h"

#define MAX_LINE_SIZE       80
#define MAX_ARG             20
#define MAX_COMMAND_CHARS   50
#define DELIMITERS          " \t\n"

pNode stack_head;

int ExeComp(char* lineSize);
int BgCmd(char* lineSize, LIST_ELEMENT** pJobsList);
int ExeCmd(LIST_ELEMENT **pJobsList, LIST_ELEMENT **pVarList, char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], int num_arg, LIST_ELEMENT **pJobsList);
int GPid;// PID (global)
int Last_Bg_Pid;
int Susp_Bg_Pid;
char* L_Fg_Cmd;
int susp; //is the process suspended: 0- no, 1- yes
#endif

