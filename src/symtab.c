#include "symtab.h"

BucketList *hashTable[SIZE];

int hash(char *key)
{
	int temp = 0;
	int i = 0;
	while (key[i] != '\0')
	{
		temp = ((temp << SHIFT) + key[i]) % SIZE;
		++i;
	}
	return temp;
}

void st_insert(char *name, int lineno, int loc)
{
	int index = hash(name);
	BucketList *l = hashTable[index];

	while ((l != NULL) && (strcmp(name, l->name) != 0))
		l = l->pNext;

	if (l == NULL)
	{
		l = (BucketList *) malloc(sizeof(BucketList));
		strcpy_s(l->name, 40, name);
		l->pLines = (LineList *) malloc(sizeof(LineList));
		l->pLines->lineno = lineno;
		l->pLines->pNext = NULL;
		l->memloc = loc;
		l->pNext = hashTable[index];
		hashTable[index] = l;
	}
	else
	{
		LineList *pLine = l->pLines;
		while (pLine->pNext != NULL)
			pLine = pLine->pNext;
		pLine->pNext = (LineList *) malloc(sizeof(LineList));
		pLine->pNext->lineno = lineno;
		pLine->pNext->pNext = NULL;
	}
}

int st_lookup(char *name)
{
	int index = hash(name);
	BucketList *l = hashTable[index];
	while ((l != NULL) && (strcmp(l->name, name) != 0))
		l = l->pNext;

	if (l == NULL)
		return -1;
	else	
		return l->memloc;
}

int st_count()
{
	int count = 0;
	int i;

	for (i = 0; i < SIZE; ++i)
	{
		if (hashTable[i] != NULL)
		{
			BucketList *l = hashTable[i];
			while (l != NULL)
			{
				++count;
				l = l->pNext;
			}
		}
	}
	return count;
}

void printSymtab()
{
	int i;
	printf("Variable name  Location    Line Numbers\n");
	printf("-------------  --------    ------------\n");
	for (i = 0; i < SIZE; ++i)
	{
		if (hashTable[i] != NULL)
		{
			BucketList *l = hashTable[i];
			while (l != NULL)
			{
				LineList *t = l->pLines;
				printf("%-14s ", l->name);
				printf("%-8d  ", l->memloc);
				while (t != NULL)
				{
					printf("%4d", t->lineno);
					t = t->pNext;
				}
				printf("\n");
				l = l->pNext;
			}
		}
	}

}