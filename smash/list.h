#ifndef _LIST_H
#define _LIST_H
#define NOT_USED 999
typedef struct _LIST_ELEMENT
{
	char* VarName;
    	char* VarValue;
	int ID;
	int pID;
    	int suspended;
    	struct _LIST_ELEMENT* pNext;
} LIST_ELEMENT;   
typedef struct _node{
	char* pDir;
	struct _node* next;
} node;
typedef node *pNode;
pNode Push(pNode stack_head, char* Dir);
pNode Pop(pNode stack_head);
int DelPID(LIST_ELEMENT** pList, int pID);
int InsertElem(LIST_ELEMENT** pList, char* value, int ID, int pID, int susp);
int DelList(LIST_ELEMENT** pList);
char* GetVar(LIST_ELEMENT* List, char* varname);
int ModifyElem(LIST_ELEMENT** pList, char* varname, char* value);
int GetId(LIST_ELEMENT** pList, int pID);
int DelVar(LIST_ELEMENT** pList, char* varname);
int GetPid(LIST_ELEMENT* List, int ID);
#endif

