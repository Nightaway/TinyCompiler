#include "code.h"
#include "linker.h"

#include <Windows.h>

FILE *f_tobj = NULL;
FILE *f_exe  = NULL;

//
TinyObjectFileHeader TObjHeader;
int *pVaribles;
char *pCodes;
int *pTempVars;

VaribleRef *pTempVarRefTail;
VaribleRef *pVarRefTail;
VaribleRef *pVarRef;
VaribleRef *pTempVarRef;

//
void readVarRefSection();
void readTempVarRefSection();
void freeMemory();
void freeVarRef();
void freeTempVarRef();

void readObjFile()
{
	fread((void *)&TObjHeader, sizeof(char), 
		 sizeof(TinyObjectFileHeader), f_tobj);

	pVaribles = (int *)malloc(TObjHeader.NumberOfVaribles * sizeof(int));
	fread((void *)pVaribles, sizeof(int), 
		 TObjHeader.NumberOfVaribles, f_tobj);

	pCodes = (char *)malloc(TObjHeader.SizeOfCode);
	fseek(f_tobj, TObjHeader.OffsetToCodeSection, SEEK_SET);
	fread((void *)pCodes, sizeof(char), TObjHeader.SizeOfCode, f_tobj);

	pTempVars = (int *)malloc(TObjHeader.NumberOfTempVaribles * sizeof(int));
	fseek(f_tobj, TObjHeader.OffsetToTempVarSection, SEEK_SET);
	fread((void *)pTempVars, sizeof(int),
		TObjHeader.NumberOfTempVaribles, f_tobj);

	fseek(f_tobj, TObjHeader.OffsetToVarRefSection, SEEK_SET);
	readVarRefSection();
	readTempVarRefSection();
}

void link(const char *tobj)
{
	fopen_s(&f_tobj, tobj, "rb");
	if (f_tobj == NULL)
	{
		printf("Can't read tiny obj file.\n");
		return ;
	}
	fclose(f_tobj);
	freeMemory();

	//readObjFile();
	CopyFile(TEXT("sample1.x"),
			 TEXT("sample1.exe"),
		     FALSE);

	//rename("sample1.x", "sample1.exe");
}

void readVarRefSection()
{
	int offset = 1;
	int flag_1 = 1;
	int flag_2 = 1;
	VaribleRef *pVRef       = NULL;
	VaribleRef *pPrevVRef   = NULL;
	VarOffset *pVOffsetHead = NULL;
	VarOffset *pVOffset     = NULL;
	VarOffset *pPrevVOffset = NULL;

	while (offset&&!feof(f_tobj))
	{
		flag_2 = 1;
		pVOffsetHead = NULL;
		pVRef = (VaribleRef *)malloc(sizeof(VaribleRef));

		if (flag_1)
		{
			fread(&pVRef->loc, sizeof(int), 1, f_tobj);
			pVarRef = pVRef;
			flag_1 = 0;
		}
		else
		{
			pVRef->loc = offset;
			pPrevVRef->pNext = pVRef;
		}

		fread(&(pVRef->VarOffsets.offset), sizeof(int), 1, f_tobj);
		pVRef->VarOffsets.pNext = NULL;
		pVRef->VarOffsets.pPrev = NULL;
		pVRef->pPrev = pPrevVRef;
		pVRef->pNext = NULL;

		pVarRefTail  = pVRef;
		pVRef->pOffsetTail = &pVRef->VarOffsets;
		pPrevVOffset = &pVRef->VarOffsets;

		fread((void *)&offset, 4, 1, f_tobj);
		while (offset)
		{
			pVOffset = (VarOffset *)malloc(sizeof(VarOffset));

			if (flag_2)
			{
				pVOffsetHead = pVOffset;	
				flag_2 = 0;
			}
			else
				pPrevVOffset->pNext = pVOffset;

			pVOffset->offset = offset;
			pVOffset->pPrev  = pPrevVOffset;
			pVOffset->pNext  = NULL;
			pVRef->pOffsetTail  = pVOffset;

			fread((void *)&offset, 4, 1, f_tobj);
			pPrevVOffset = pVOffset;
			pVOffset = pVOffset->pNext;
		}
		fread((void *)&offset, 4, 1, f_tobj);
		pVRef->VarOffsets.pNext = pVOffsetHead;
	    pPrevVRef = pVRef;
		pVRef = pVRef->pNext;
	}
}

void readTempVarRefSection()
{
	int offset = 1;
	int flag_1 = 1;
	int flag_2 = 1;
	VaribleRef *pVRef       = NULL;
	VaribleRef *pPrevVRef   = NULL;
	VarOffset *pVOffsetHead = NULL;
	VarOffset *pVOffset     = NULL;
	VarOffset *pPrevVOffset = NULL;

	fseek(f_tobj, TObjHeader.OffsetToTempVarRefSection, SEEK_SET);

	while (offset&&!feof(f_tobj))
	{
		flag_2 = 1;
		pVOffsetHead = NULL;
		pVRef = (VaribleRef *)malloc(sizeof(VaribleRef));

		if (flag_1)
		{
			fread(&pVRef->loc, sizeof(int), 1, f_tobj);
			pTempVarRef = pVRef;
			flag_1 = 0;
		}
		else
		{
			pVRef->loc = offset;
			pPrevVRef->pNext = pVRef;
		}

		fread(&(pVRef->VarOffsets.offset), sizeof(int), 1, f_tobj);
		pVRef->VarOffsets.pNext = NULL;
		pVRef->VarOffsets.pPrev = NULL;
		pVRef->pPrev = pPrevVRef;
		pVRef->pNext = NULL;

		pTempVarRefTail  = pVRef;
		pVRef->pOffsetTail = &pVRef->VarOffsets;
		pPrevVOffset  = &pVRef->VarOffsets;

		fread((void *)&offset, 4, 1, f_tobj);
		while (offset)
		{
			pVOffset = (VarOffset *)malloc(sizeof(VarOffset));

			if (flag_2)
			{
				pVOffsetHead = pVOffset;	
				flag_2 = 0;
			}else
				pPrevVOffset->pNext = pVOffset;


			pVOffset->offset = offset;
			pVOffset->pPrev  = pPrevVOffset;
			pVOffset->pNext  = NULL;
			pVRef->pOffsetTail  = pVOffset;

			fread((void *)&offset, 4, 1, f_tobj);
			pPrevVOffset = pVOffset;
			pVOffset = pVOffset->pNext;
		}
		fread((void *)&offset, 4, 1, f_tobj);
		pVRef->VarOffsets.pNext = pVOffsetHead;
	    pPrevVRef = pVRef;
		pVRef = pVRef->pNext;
	}
}


void freeMemory()
{
	free (pVaribles);
	pVaribles = NULL;
	free (pCodes);
	pCodes = NULL;
    free (pTempVars);
	pTempVars = NULL;
 //   freeVarRef ();
	//freeTempVarRef ();
}

//void freeVarRef()
//{
//	VaribleRef *pVRefTail  = pVarRefTail;
//	while (pVRefTail)
//	{
//		while (pOffsetTail)
//		{
//			free(pOffsetTail);
//			pOffsetTail = pOffsetTail->pPrev;
//		}
//		free(pVRefTail);
//		pVRefTail = pVRefTail->pPrev;
//	}
//}
//
//void freeTempVarRef()
//{
//	VaribleRef *pVRefTail  = pTempVarRefTail;
//	VarOffset *pOffsetTail = pTVOffsetTail;
//
//	while (pVRefTail)
//	{
//		while (pOffsetTail)
//		{
//			free(pOffsetTail);
//			pOffsetTail = pOffsetTail->pPrev;
//		}
//		free(pVRefTail);
//		pVRefTail = pVRefTail->pPrev;
//	}
//}