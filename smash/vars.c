//		vars.c
// contain var- related functions (replace values, print vars)
//**********************************************************************
#include "vars.h"
//**************************************************************************************
// function name: ValueReplace
// Description: swap values
// Parameters: original var, old values , new value 
// Returns: pointer to original
//**************************************************************************************
char *ValueReplace(char *orig_string, char *old_var_value, char *new_var_value)
{ 
	int oldlen, newlen;
	char *s, *p;
	s = orig_string;
	while (s != NULL)
	{
		p = strstr(s, old_var_value);
		if (p == NULL )
			return orig_string;
		oldlen = strlen(old_var_value);
		newlen = strlen(new_var_value);
		
		if (orig_string == NULL)
			return NULL;
		memmove(p + newlen, p + oldlen, strlen(p + oldlen) + 1);
		memcpy(p, new_var_value, newlen);
		s = p + newlen;
	}
	return orig_string;
}
//**************************************************************************************
// function name: VarRplc
// Description: replace $var with its value
// Parameters: VarList, command string 
// Returns: 0- success, 1- failure
//**************************************************************************************
int VarRplc(LIST_ELEMENT *VarList, char* lineSize) 
{
	char VarName[MAX_LINE_SIZE];
	char* delimiters = " \t\n";
	char* curr_name;
	char* val;
	while((curr_name = strstr(lineSize, "$"))) 
	{
       	curr_name = strtok(curr_name, delimiters);
		strcpy(VarName, curr_name);
		curr_name[strlen(curr_name)]=' ';
   		val = GetVar(VarList, VarName+1);
		if (val == NULL) 
		{
         		printf("smash error: => \"%s\" - variable not found\n", VarName);
			return 1;
   	    	}
	    	else 
	    	{
			// switch var with value.
			curr_name = ValueReplace(lineSize, VarName, val);
		}
	}
	return 0;
}		
//**************************************************************************************
// function name: PrintVars
// Description: print all vars + values
// Parameters: VarList
// Returns: void
//**************************************************************************************
void PrintVars(LIST_ELEMENT** pVarList)
{
	LIST_ELEMENT* pElem = *pVarList;
    
	while (pElem) 
	{
		printf("%s := %s\n", pElem->VarName, pElem->VarValue);
        	pElem = pElem->pNext;
    	}
}

