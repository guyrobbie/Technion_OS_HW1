#ifndef _VARS_H
#define _VARS_H
#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include "list.h"
#define MAX_LINE_SIZE 80
int VarRplc(LIST_ELEMENT *VarList, char* lineSize);
void PrintVars(LIST_ELEMENT** pVarList);
#endif

