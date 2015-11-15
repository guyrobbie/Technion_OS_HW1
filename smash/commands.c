//		commands.c
//********************************************
#include "commands.h"
#include "signals.h"
#include <errno.h>

extern char g_prevPwd[MAX_LINE_SIZE]; // saving the previous path as global and initialize it to null characters
extern char g_currPwd[MAX_LINE_SIZE];

int g_forground_pID;
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to JobList, pointer VarList, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(LIST_ELEMENT **pJobsList, LIST_ELEMENT **pVarList, char* lineSize, char* cmdString)
{
	LIST_ELEMENT* pElem;
	LIST_ELEMENT* pElem_curr;
	char* cmd; 
	char* args[MAX_ARG];
	char *val;
	char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";  
	int pID = 0, i = 0, num_arg = 0;
	
	
	bool illegal_cmd = FALSE; // illegal command
    	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters); 
		if (args[i] != NULL) 
			num_arg++; 
 
	}

/*************************************************/
// Built in Commands
/*************************************************/
	if (!strcmp(cmd, "cd") ) 
	{
		getcwd(g_currPwd, MAX_LINE_SIZE);
		if(!strcmp(args[1], "-"))
		{
			 char temp[MAX_LINE_SIZE];
			 if(!chdir(g_prevPwd))
				 printf("%s\n",g_prevPwd);
			 else
				 perror("smash error:> "); // in case we never switch path earlier
			 //swapping
			 strncpy(temp, g_currPwd, MAX_LINE_SIZE);
			 strncpy(g_currPwd, g_prevPwd,MAX_LINE_SIZE);
			 strncpy(g_prevPwd, temp, MAX_LINE_SIZE);
		}
		else
		{
			if(!chdir(args[1]))
			{
				strncpy(g_prevPwd, g_currPwd,MAX_LINE_SIZE);
				getcwd(g_currPwd, MAX_LINE_SIZE);
			}
			else
				printf("smash error:> \"%s\" - path not found\n", args[1]);
		}
		return 0;			
	} 
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		
		if(getcwd(pwd, MAX_LINE_SIZE) != NULL)
			printf("%s\n", pwd);
		else
			perror("smash error:> ");
		return 0;
	}
	
	/*************************************************/
	else if (!strcmp(cmd, "mkdir")) 
	{
		
		if(mkdir(args[1],0x777) == -1)
			if(errno == EEXIST)
				printf("smash error:> \"%s\" - directory already exists\n", args[1]);
			else
				printf("smash error:> \"%s\" - cannot create directory\n", args[1]);
		return 0;
	}
	
	else if (!strcmp(cmd, "kill")) 
	{

	}
	
	/*************************************************/
	else if (!strcmp(cmd, "set"))
	{
 		
	}
	/*************************************************/
	else if (!strcmp(cmd, "unset")) 
	{
	 	if (num_arg == 1) 
		{
			if (DelVar(pVarList, args[1]) != 0) 
				printf("smash error: > \"%s\" - variable not found\n", args[1]);
	  	} 
		else 
			illegal_cmd = TRUE;
	}
		
	/*************************************************/
	else if (!strcmp(cmd, "show")) 
	{
 		if (num_arg <= 1) 
		{
		        // Show all
				if (args[1] == NULL) 
					PrintVars(pVarList);
				// print one var only
				else 
				{
					val = GetVar(*pVarList, args[1]);
					if (val == NULL) 
						printf("smash error: > \"%s\" - variable not found\n", args[1]);
					else 
						printf("%s := %s\n", args[1], val);
				}
 		} 
		else 
		{
			illegal_cmd = TRUE;
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "jobs")) 
	{
		for(pElem_curr = *pJobsList ; pElem_curr != NULL ; pElem_curr = pElem_curr->pNext)
		{
			printf("[%d] %s : %d %d secs", pElem_curr->ID, pElem_curr->VarValue, pElem_curr->pID, (time(NULL)- pElem_curr->time ));
			
			printf( (pElem_curr->suspended == 0)? "\n": " (stopped)\n");
		}
		return 0;
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		printf("smash pid is %d\n",getpid());
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
		
		if(num_arg == 0)
		{
			for(pElem_curr = *pJobsList ; pElem_curr->pNext != NULL ; pElem_curr = pElem_curr->pNext)
			{
				printf("DEBUG: [%d] %s : %d\n", pElem_curr->ID, pElem_curr->VarValue, pElem_curr->pID);
			}
			
			g_forground_pID = pElem_curr->pID;
			strncpy(L_Fg_Cmd, pElem_curr->VarValue, MAX_COMMAND_CHARS);
			if(pElem_curr->suspended == 1)
				kill(pElem_curr->pID, SIGCONT);
			
			waitpid(pElem_curr->pID,NULL,WUNTRACED);
			
			DelPID(pJobsList,pElem_curr->pID);
		}
		else
		{
			int tempPid;
			tempPid = GetPid(*pJobsList, atoi(args[1]));
			//printf("DEBUG: args[1]: %s, args[1]_val %d\n", args[1],atoi(args[1]));
			 
			if(tempPid == -1)
				printf("smash error: > \"%s\" job does not exist\n", args[1]);
			else
			{
				printf("signal %d was sent to pid: %d \n", SIGCONT, tempPid);
				g_forground_pID = tempPid;
				strncpy(L_Fg_Cmd, GetValue(pJobsList, tempPid), MAX_COMMAND_CHARS);
				printf("DEBUG: GetValue(pJobsList, tempPid) %s \n", GetValue(pJobsList, tempPid));
				printf("DEBUG: L_Fg_Cmd %s \n", L_Fg_Cmd);
				kill(tempPid, SIGCONT);
				
				waitpid(tempPid,NULL,WUNTRACED);
				DelPID(pJobsList, tempPid);
			}
				
		}
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
  		
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
   		
	} 
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, num_arg, pJobsList);
	 	return 0;
	}
	if (illegal_cmd == TRUE)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************


void ExeExternal(char *args[MAX_ARG], int num_arg, LIST_ELEMENT **pJobsList)
{
	int pID;
   	switch(pID = fork()) 
	{
    		case -1: 
					// Add your code here (error)
					perror("fork failed - ");
					/* 
					your code
					*/
		break;
        	case 0 :
                	// Child Process
               		setpgrp();
					printf("Debug, Child process, pid: %d\n", getpid()); // TODO: for debug, need to be remove
					execv(args[0],args);
			        // Add your code here (execute an external command)
					perror("smash error: > ");
					exit(1);
					/* 
					your code
					*/
		break;
			default:
					g_forground_pID = pID;
					strncpy(L_Fg_Cmd, args[0], MAX_COMMAND_CHARS);
					waitpid(pID,NULL,WUNTRACED);
					printf("Debug, Parent process, pid: %d\n", getpid()); // TODO: for debug, need to be remove
                	// Add your code here
					
					/* 
					your code
					*/
			break;
	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize)
{
	int pID;
	char ExtCmd[MAX_LINE_SIZE+2];
	char *args[MAX_ARG];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
		// Add your code here (execute a complicated command)
					
		/* 
		your code
		*/
	} 
	return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to JobList
// Parameters: command string, pointer to JobList
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, LIST_ELEMENT** pJobsList)
{
	int pID, i;
	char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
	int num_arg = 0;
	int res;
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		
		// Extract Args
		Command = strtok(lineSize, delimiters);
		if (Command == NULL)
			return 0; 
		args[0] = Command;
		for (i=1; i<MAX_ARG; i++)
		{
			args[i] = strtok(NULL, delimiters); 
			if (args[i] != NULL) 
				num_arg++; 
		}
			
		// Add your code here (execute a in the background)
		pID = fork();
		if(pID == 0)
		{
			setpgrp();
			//printf("Debug, Background, Child process, pid: %d\n", getpid()); // TODO: for debug, need to be remove
			execv(args[0], args);
			perror("Execv error");
			exit(1);
		}
		else if(pID > 0)
		{
			//printf("Debug, Background, Parent process, pid: %d\n", getpid()); // TODO: for debug, need to be remove
			res = InsertElem(pJobsList, args[0], 1, pID, 0);
			//
		}
		else
		{
			printf("\nFork Error");
		}
		/* 
		your code
		*/
		return 0;
	}
	return -1;
}

