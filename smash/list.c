// list.c
// contains linked- list based ADT for the JobsList & VarList 
//********************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <string.h>
#include <unistd.h>
#include <time.h>
//**************************************************************************************
// function name: InsertElem
// Description: insert new element to list
// Parameters: pointer to list, value to store, serial number, PID, suspended flag 
// Returns: 0- success, 1- failure
//**************************************************************************************
int InsertElem(LIST_ELEMENT** pList, char* value, int ID, int pID, int susp)
{ 
	LIST_ELEMENT *List;
	LIST_ELEMENT *temp;
	List = *pList;
	if (value == NULL) 
		return -1; 
    	if (List == NULL)
	{  
		List = (LIST_ELEMENT*)malloc(sizeof(LIST_ELEMENT));
		if (List == NULL) 
			exit (-1); 
	    
		List->VarValue=(char*)malloc(sizeof(char)*(strlen(value)+1));
		if (List->VarValue == NULL) 
			exit (-1); 
	    
		strcpy(List->VarValue, value);
		List->VarName = NULL; // VarName doesn't mean anything for job
		List->time = time(NULL);
		List->ID = ID;
		List->pID = pID;
		List->suspended = susp;
		List->pNext = NULL;
		*pList = List;
		return 0;
    	} 
	else 
	{    
		if (List->ID == ID) 
			return InsertElem(&List->pNext, value, ID+1, pID, susp);
		else
		{
			temp = (LIST_ELEMENT*)malloc(sizeof(LIST_ELEMENT)); 
			if (temp == NULL)  
				exit (-1); 
			temp->VarValue =(char*) malloc(sizeof(char)*(strlen(List->VarValue)+1));
			if (temp->VarValue == NULL) 
				exit (-1); 
			strcpy(temp->VarValue, List->VarValue);
			temp->VarName = NULL;
			List->time = time(NULL);
			temp->pID = List->pID;
			temp->ID = List->ID;
			temp->suspended = List->suspended;
			temp->pNext = List->pNext;
			 
			free(List->VarValue);
			List->VarValue=(char*)malloc(sizeof(char)*(strlen(value)+1));
			if (List->VarValue == NULL) 
				exit (-1); 
			strcpy(List->VarValue, value);
			List->time = time(NULL);
			List->ID = ID;
			List->pID = pID;
			List->suspended = susp;
			List->pNext = temp;
  			return 0;
		}
	}
}
//**************************************************************************************
// function name: ModifyElem
// Description: modify elemet
// Parameters: pointer to list, var name, var value 
// Returns: 0- success
//**************************************************************************************
int ModifyElem(LIST_ELEMENT** pList, char* varname, char* value)
{
	LIST_ELEMENT *List;
	List = *pList;
		
	if ((varname == NULL) || (value == NULL)) 
		return -1; 
	if (List == NULL)
	{  
		List = (LIST_ELEMENT*)malloc(sizeof(LIST_ELEMENT));
		if (List == NULL) 
			exit (-1);
         
		List->VarValue=(char*)malloc(sizeof(char)*(strlen(value)+1));
		if (List->VarValue == NULL) 
			exit (-1); 
		
		List->VarName=(char*) malloc(sizeof(char)*(strlen(varname)+1));
		if (List->VarName == NULL) 
			exit (-1); 
		strcpy(List->VarName, varname);
		strcpy(List->VarValue, value);
		List->pNext = NULL;
		List->suspended = NOT_USED;
		List->ID = NOT_USED;
		List->pID = NOT_USED;
		*pList = List;
		return 0;
	} 
	 
	else 
	{         
		if (!strcmp(varname,List->VarName))
		{
			free(List->VarValue);
			List->VarValue=(char*)malloc(sizeof(char)*(strlen(value)+1));
			strcpy(List->VarValue, value);
			if (List->VarValue == NULL) 
				exit (-1); 
			return 0;
        }
		return ModifyElem(&List->pNext, varname, value);
	}
}
//**************************************************************************************
// function name: DelVar
// Description: delete var from list
// Parameters: pointer to list, var name 
// Returns: 0- success ,1- else
//**************************************************************************************
int DelVar(LIST_ELEMENT** pList, char* varname)
{
	LIST_ELEMENT* temp, *List;
	List = *pList;
	
    	if ((varname == NULL) || (List == NULL)) 
		return 1; 	
	if (!strcmp(varname,List->VarName))
    	{
		free(List->VarName);
		free(List->VarValue);
        	temp = List;
        	*pList = List->pNext;
       	 	free(temp);
    	    	return 0;
    	}
	return DelVar(&List->pNext, varname);
}
//**************************************************************************************
// function name: DelPID
// Description: delete process from list
// Parameters: pointer to list, PID 
// Returns: 0- success ,1- else
//**************************************************************************************
int DelPID(LIST_ELEMENT** pList, int pID)
{
        LIST_ELEMENT* temp, *List;
        List = *pList;
       
        if (List == NULL)       
		return 1;
        if (List->pID == pID) 
        {
		free(List->VarName);
		free(List->VarValue);
	        temp = List;
            	*pList = List->pNext;
            	free(temp);
            	return 0;
        }
	return DelPID(&List->pNext, pID);
}
//**************************************************************************************
// function name: GetVar
// Description: return var value
// Parameters: pointer to list, var name 
// Returns: var name, or NULL if name not found
//**************************************************************************************
char* GetVar(LIST_ELEMENT* List, char* varname)
{
	if (varname == NULL || List == NULL) 
		return NULL;
	if (!strcmp(List->VarName, varname))
		return List->VarValue;
	return GetVar(List->pNext, varname);
}
//**************************************************************************************
// function name: GetId
// Description: get ID of element accordind its PID
// Parameters: pointer to list, PID 
// Returns: ID, or 1 if not found
//**************************************************************************************
int  GetId(LIST_ELEMENT** pList, int pID)
{
        LIST_ELEMENT *List;
        List = *pList;
        
        if (List == NULL) 
		return 1; 
        if (List->pID == pID) 			
		return List->ID;
	return GetId(&List->pNext, pID);
}

//**************************************************************************************
// function name: GetValue
// Description: get Value of element accordind its PID
// Parameters: pointer to list, PID 
// Returns: value, or NULL if not found
//**************************************************************************************
char* GetValue(LIST_ELEMENT** pList, int pID)
{
        LIST_ELEMENT *List;
        List = *pList;
        
        if (List == NULL) 
		return NULL; 
        if (List->pID == pID) 			
		return List->VarValue;
	return GetValue(&List->pNext, pID);
}

//**************************************************************************************
// function name: GetPid
// Description: get PID of element accordind its ID
// Parameters: pointer to list, ID 
// Returns: PID, or 1 if not found
//**************************************************************************************
int GetPid(LIST_ELEMENT* List, int ID)
{
        if (List == NULL)
		return -1;
        if (List->ID == ID)
		return List->pID;
	return GetPid(List->pNext, ID);
}


//**************************************************************************************
// function name: DelList
// Description: delete list. free all related memory
// Parameters: pointer to list
// Returns: 0 - if success
//**************************************************************************************
int DelList(LIST_ELEMENT** pList)
{
    	LIST_ELEMENT* temp, *List;
	List = *pList;
	if (List == NULL) 
		return 0;     
	free(List->VarName);
	free(List->VarValue);
    	temp = List;
    	*pList = List->pNext;
    	free(temp);
    	return DelList(pList);
}
pNode Push(pNode stack, char* Dir)
{
	pNode new_node =(pNode)malloc(sizeof(node));
	pNode tmp_node;
	if (new_node == NULL) 
	{
		return stack;
	}
	new_node->pDir = (char*)calloc(strlen(Dir)+1, sizeof(char));
	strcpy(new_node->pDir,Dir);
	new_node->next = stack;
	printf("directory stack: ");
	tmp_node = new_node;
	while(tmp_node)
	{    // PRINTING THE STACK
		printf("%s ",tmp_node->pDir);
		tmp_node = tmp_node->next;
	}
	printf("\n");
	return new_node; 
}
pNode Pop(pNode stack)
{
	pNode tmp_node;
	if(stack == NULL)
	{
		printf("smash error:=> \"directory stack empty\" \n");
		return stack;
	}
	else 
	{
		tmp_node = stack->next; // prints from the next dir
		printf("directory stack: ");
		for (; tmp_node != NULL; tmp_node = tmp_node->next) 
		{
			printf("%s ",tmp_node->pDir);
		}
		printf("\n");
		pNode next_node = stack->next;
		chdir(stack->pDir);   // change dir tp the dir stored in p.dir
		free(stack->pDir);
		free(stack);
		return next_node;
	}
}

