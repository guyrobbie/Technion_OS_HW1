//		commands.c
//********************************************
#include "commands.h"
#include "signals.h"
#include <errno.h>
#include <sys/stat.h>

extern char g_prevPwd[MAX_LINE_SIZE]; // saving the previous path as global and initialize it to null characters
extern char g_currPwd[MAX_LINE_SIZE];

int g_forground_pID;
int g_last_terminated_process;
int g_process_terminated;


//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to JobList, pointer VarList, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(LIST_ELEMENT** pJobsList, LIST_ELEMENT** pVarList, char* lineSize, char* cmdString)
{
    LIST_ELEMENT* pElem_curr = NULL;
    char* cmd;
    char* args[MAX_ARG];
    char* val;
    char pwd[MAX_LINE_SIZE];
    int i = 0, num_arg = 0;

    bool illegal_cmd = FALSE; // illegal command
    cmd = strtok(lineSize, DELIMITERS);
    if (cmd == NULL)
        return 0;
    args[0] = cmd;
    for (i = 1; i < MAX_ARG; i++) {
        args[i] = strtok(NULL, DELIMITERS);
        if (args[i] != NULL)
            num_arg++;
    }

    
    /*************************************************/
    // Built in Commands
    /*************************************************/
    if (!strcmp(cmd, "cd")) {
        getcwd(g_currPwd, MAX_LINE_SIZE);
        if (!strcmp(args[1], "-")) {
            char temp[MAX_LINE_SIZE];
            if (!chdir(g_prevPwd))
                printf("%s\n", g_prevPwd);
            else
                perror("smash error:> "); // in case we never switch path earlier
            // swapping
            strncpy(temp, g_currPwd, MAX_LINE_SIZE);
            strncpy(g_currPwd, g_prevPwd, MAX_LINE_SIZE);
            strncpy(g_prevPwd, temp, MAX_LINE_SIZE);
        } else {
            if (!chdir(args[1])) {
                strncpy(g_prevPwd, g_currPwd, MAX_LINE_SIZE);
                getcwd(g_currPwd, MAX_LINE_SIZE);
            } else
                printf("smash error:> \"%s\" - path not found\n", args[1]);
        }
        return 0;
    }

    /*************************************************/
    else if (!strcmp(cmd, "pwd")) {

        if (getcwd(pwd, MAX_LINE_SIZE) != NULL)
            printf("%s\n", pwd);
        else
            perror("smash error:> ");
        return 0;
    }

    /*************************************************/
    else if (!strcmp(cmd, "mkdir")) {

        if (mkdir(args[1], 0x777) == -1) {
            if (errno == EEXIST) {
                printf("smash error:> \"%s\" - directory already exists\n", args[1]);
            } else {
                printf("smash error:> \"%s\" - cannot create directory\n", args[1]);
            }
        }

        return 0;
    } else if (!strcmp(cmd, "kill")) {
        if (args[1][0] == '-') {
            int job = atoi(args[2]);
            int signum = atoi(&args[1][1]); // skips the '-' character
            int is_signal_send = 0;

            for (pElem_curr = *pJobsList; pElem_curr != NULL; pElem_curr = pElem_curr->pNext) {
                if (job == pElem_curr->ID) {
                    if ((signum < 32) && (signum > 0)) {
                        if (kill(pElem_curr->pID, signum) == 0) {
                            is_signal_send = 1;
                            printf("signal %d was sent to pid: %d \n", signum, pElem_curr->pID);
                        } else
                            printf("smash error: > kill %d - cannot send signal\n", job);

                    } else
                        printf("smash error: > kill %d - cannot send signal\n", job);
                    break; // exit when signal was sent
                }
            }
            if (is_signal_send == 0)
                printf("smash error: > kill %d - job does not exist\n", job);
        }
    }

    /*************************************************/
    else if (!strcmp(cmd, "set")) {

    }
    /*************************************************/
    else if (!strcmp(cmd, "unset")) {
        if (num_arg == 1) {
            if (DelVar(pVarList, args[1]) != 0)
                printf("smash error: > \"%s\" - variable not found\n", args[1]);
        } else
            illegal_cmd = TRUE;
    }

    /*************************************************/
    else if (!strcmp(cmd, "show")) {
        if (num_arg <= 1) {
            // Show all
            if (args[1] == NULL)
                PrintVars(pVarList);
            // print one var only
            else {
                val = GetVar(*pVarList, args[1]);
                if (val == NULL)
                    printf("smash error: > \"%s\" - variable not found\n", args[1]);
                else
                    printf("%s := %s\n", args[1], val);
            }
        } else {
            illegal_cmd = TRUE;
        }
    }
    /*************************************************/
    else if (!strcmp(cmd, "jobs")) {
        for (pElem_curr = *pJobsList; pElem_curr != NULL; pElem_curr = pElem_curr->pNext) {
            printf("[%d] %s : %d %d secs",
                   pElem_curr->ID,
                   pElem_curr->VarValue,
                   pElem_curr->pID,
                   (int)(time(NULL) - pElem_curr->time));

            printf((pElem_curr->suspended == 0) ? "\n" : " (stopped)\n");
        }
        return 0;
    }
    /*************************************************/
    else if (!strcmp(cmd, "showpid")) {
        printf("smash pid is %d\n", getpid());
    }
    /*************************************************/
    else if (!strcmp(cmd, "fg")) {

        if (num_arg == 0) {
            g_forground_pID = pElem_curr->pID;
            strncpy(L_Fg_Cmd, pElem_curr->VarValue, MAX_COMMAND_CHARS);
            if (pElem_curr->suspended == 1)
                kill(pElem_curr->pID, SIGCONT);

            waitpid(pElem_curr->pID, NULL, WUNTRACED);

            DelPID(pJobsList, pElem_curr->pID);
        } else {
            int tempPid;
            tempPid = GetPid(*pJobsList, atoi(args[1]));

            if (tempPid == -1)
                printf("smash error: > \"%s\" job does not exist\n", args[1]);
            else {
                printf("signal %d was sent to pid: %d \n", SIGCONT, tempPid);
                g_forground_pID = tempPid;
                strncpy(L_Fg_Cmd, GetValue(pJobsList, tempPid), MAX_COMMAND_CHARS);
                kill(tempPid, SIGCONT);

                waitpid(tempPid, NULL, WUNTRACED);
                DelPID(pJobsList, tempPid);
            }
        }
    }
    /*************************************************/
    else if (!strcmp(cmd, "bg")) {

    }
    /*************************************************/
    else if (!strcmp(cmd, "quit")) {
        if (num_arg == 0) {
            exit(0);
        } else {
            if (!strcmp(args[1], "kill")) // testing if kill parameter was passed
            {
                for (pElem_curr = *pJobsList; pElem_curr != NULL; pElem_curr = pElem_curr->pNext) {
                    g_last_terminated_process = pElem_curr->pID;
                    g_process_terminated = 0;
                    printf("[%d] %s - Sending SIGTERM...", pElem_curr->ID, pElem_curr->VarValue);
                    kill(pElem_curr->pID, SIGTERM);

                    if (sleep(5) != 0) // meaning it was Signaled SIGCHLD, for the terminated child.
                    {
                        if (g_process_terminated == 1) {
                            printf("Done.\n");
                        }
                    } else {
                        kill(pElem_curr->pID, SIGKILL);
                        printf("(5 sec passed) Sending SIGKILL...Done.\n");
                    }
                }
                exit(0);
            }
        }

    }
    /*************************************************/
    else // external command
    {
        ExeExternal(args, num_arg, pJobsList);
        return 0;
    }
    if (illegal_cmd == TRUE) {
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

void ExeExternal(char* args[MAX_ARG], int num_arg, LIST_ELEMENT** pJobsList)
{
    int pID;
    switch (pID = fork()) {
    case -1:
        // Add your code here (error)
        perror("fork failed - ");
        /*
        your code
        */
        break;
    case 0:
        // Child Process
        setpgrp();
        execv(args[0], args);
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
        waitpid(pID, NULL, WUNTRACED);
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
    int status;
    char* args[MAX_ARG];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) ||
        (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&"))) {
        args[0] = "csh";
        args[1] = "-fc";
        args[2] = lineSize;
        args[3] = (char*)NULL; // try

        pID = fork();
        if (pID == 0) {
            setpgrp();
            // Build csh args
            execvp(args[0], args);
            // execv(args[0], args);
            perror("Execv error");
            exit(1);
        } else if (pID > 0) {
            wait(&status);
        } else {
            printf("\nFork Error");
        }
        // Add your code here (execute a complicated command)
        return 0;
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
    char* args[MAX_ARG];
    int num_arg = 0;

    if (lineSize[strlen(lineSize) - 2] == '&') {
        lineSize[strlen(lineSize) - 2] = '\0';

        // Extract Args
        Command = strtok(lineSize, delimiters);
        if (Command == NULL)
            return 0;
        args[0] = Command;
        for (i = 1; i < MAX_ARG; i++) {
            args[i] = strtok(NULL, delimiters);
            if (args[i] != NULL)
                num_arg++;
        }

        // Add your code here (execute a in the background)
        pID = fork();
        if (pID == 0) {
            setpgrp();
            execv(args[0], args);
            perror("Execv error");
            exit(1);
        } else if (pID > 0) {
            InsertElem(pJobsList, args[0], 1, pID, 0);
            //
        } else {
            printf("\nFork Error");
        }
        /*
        your code
        */
        return 0;
    }
    return -1;
}
