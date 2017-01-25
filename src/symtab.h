#ifndef __SYMTABL_H_
#define __SYMTABL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#define SIZE 24

#define SHIFT 4

typedef struct LineList {
	struct LineList *pNext;

	int lineno;
}LineList;

typedef struct BuckList {
	struct BuckList *pNext;

	struct LineList *pLines;
	
	char name[80];
	int memloc;
}BucketList;

extern BucketList *hashTable[SIZE];

void st_insert(char *name, int lineno, int loc);
int  st_lookup(char *name);
int st_count();

int hash(char *key);

void printSymtab();
#endif