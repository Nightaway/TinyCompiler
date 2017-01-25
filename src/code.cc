#include "globals.h"
#include "symtab.h"
#include "code.h"


extern FILE *code;

TinyObjectFileHeader header = {0};

static int offset  = 0x0;
static int var_loc = 0;
static VaribleRef *pVarRef;
static VaribleRef *pTempVarRef;

static int tempvar_loc = 0;
static int tempvar_max = 0;

void AddVarRef(int loc, int offset, VTYPE vtype)
{	
	VaribleRef *pVRef = NULL;
	VaribleRef *pNewRef = NULL;
	VarOffset *pVOffset = NULL;
	VarOffset *pNewOffset = NULL;

	switch (vtype)
	{
	case VTYPE_NORMAL :
		pVRef = pVarRef;
		break;

	case VTYPE_TEMP :
		pVRef = pTempVarRef;
		break;
	}

	while (pVRef)
	{
		if (pVRef->loc == loc)
		{
			pVOffset = &pVRef->VarOffsets;
			while (pVOffset->pNext)
				pVOffset = pVOffset->pNext;
			
			pNewOffset = (VarOffset *)malloc(sizeof(VarOffset));
			pNewOffset->offset = offset;
			pNewOffset->pNext = NULL;
			pVOffset->pNext = pNewOffset;
			return ;
		}

		pVRef = pVRef->pNext;
	}

	pNewRef = (VaribleRef *)malloc(sizeof(VaribleRef));
	pNewRef->loc = loc;
	pNewRef->pNext = NULL;
	pNewRef->VarOffsets.offset = offset;
	pNewRef->VarOffsets.pNext  = NULL;

	switch (vtype)
	{
	case VTYPE_NORMAL :
		if (pVarRef)
		{
			pVarRef->pNext = pNewRef;
		}
		else {
			pVarRef = pNewRef;
		}
		break;

	case VTYPE_TEMP :
		if (pTempVarRef)
		{
			pTempVarRef->pNext = pNewRef;
		}
		else {
			pTempVarRef = pNewRef;
		}
		break;
	}
}


void writeFileHeader()
{
	header.magic		         = 0x00010001;
	header.NumberOfVaribles      = st_count();
	header.OffsetToVarSection    = sizeof(header);
	header.NumberOfLabels        = 0;
	header.OffsetToLabelSection  = 0;
	fwrite((const void *)&header, sizeof(char), sizeof(TinyObjectFileHeader), code);
	offset += sizeof(TinyObjectFileHeader);
}

void updateFileHeader()
{
	fseek(code, 0,SEEK_SET);
	fwrite((const void *)&header, sizeof(header), 1, code);
}

void writeCode(int opcode, int sizeofopcode, 
			   int oprand, int sizeofoprand)
{
	fwrite((const void *)&opcode, sizeof(char), sizeofopcode, code);
	offset += sizeofopcode * sizeof(char);
	fwrite((const void *)&oprand, sizeof(char), sizeofoprand, code);
	offset += sizeofoprand * sizeof(char);
}

void writeVarSection()
{
	int count = st_count();
	int size  = (count / 16 + 1) * 16;
	char *data = (char *)malloc(size);
	memset((void *)data, 0, size);
	fwrite((const void *)data, sizeof(char), size, code);
	offset += size;
	free((void *)data);

	//
	header.OffsetToCodeSection = offset;
}

void fillSection()
{
	char *data;
	int count = offset % 16;

	if (count != 0)
	    count = 16 - count;

	if (!count)
		return ;

	data = (char *)malloc(count);
	memset((void *)data, 0, count);
	fwrite((const void *)data, sizeof(char), count, code);
	offset += count;
	free((void *)data);
}

void fillCodeSection()
{
	int offset_1 = (offset / 16 + 1) * 16;
	int size = offset_1 - offset;
	char *data;

	header.SizeOfCode =  offset - header.OffsetToCodeSection;

	if (!size)
		return ;
	data = (char *)malloc(size);
	memset((void *)data, 0, size);
	fwrite((const void *)data, sizeof(char), size, code);
	offset += size;
	free((void *)data);

	header.OffsetToTempVarSection = offset;
}

void writeTempVarSection()
{
	int size  = (tempvar_max / 16 + 1) * 16;
	char *data = (char *)malloc(size);

	header.NumberOfTempVaribles = tempvar_max;

	memset((void *)data, 0, size);
	fwrite((const void *)data, sizeof(char), size, code);
	offset += size;
	free((void *)data);
}

void writeVarRefSection()
{
	int data = 0;
	VaribleRef *pVRef;
	VarOffset  *pOffset;

	header.OffsetToVarRefSection = offset;

	if (!pVarRef)
		return ;

	pVRef = pVarRef;

	do {
		fwrite((const void *)&pVRef->loc, sizeof(char), 4, code);
		offset += 4;
		
		pOffset = &(pVRef->VarOffsets);
		do {
			fwrite((const void *)&pOffset->offset, sizeof(char), 4, code);
			offset += 4;

			pOffset = pOffset->pNext;
		} while (pOffset);
		fwrite((const void *)&data, sizeof(char), 4, code);
		offset += 4;

		pVRef = pVRef->pNext;
	} while (pVRef);
}

void writeTempVarRefSection()
{
	int data = 0;
	VaribleRef *pVRef;
	VarOffset  *pOffset;

	header.OffsetToTempVarRefSection = offset;

	if (!pTempVarRef)
		return ;

	pVRef = pTempVarRef;

	do {
		fwrite((const void *)&pVRef->loc, sizeof(char), 4, code);
		offset += 4;
		
		pOffset = &(pVRef->VarOffsets);
		do {
			fwrite((const void *)&pOffset->offset, sizeof(char), 4, code);
			offset += 4;

			pOffset = pOffset->pNext;
		} while (pOffset);
		fwrite((const void *)&data, sizeof(char), 4, code);
		offset += 4;

		pVRef = pVRef->pNext;
	} while (pVRef);
}

int mallocTempVar()
{
	if ((tempvar_loc+1) > tempvar_max)
		tempvar_max = tempvar_loc + 1;

	return tempvar_loc++;
}

void freeTempVar()
{
	tempvar_loc--;
}

void emitLOC(REG reg, const int Const)
{
	int opcode = 0x00;
	int SizeOfopcode = 1;
	switch (reg)
	{
	case REG_EAX :
		opcode = 0xB8;
		break;
	case REG_EBX :
		opcode = 0xBB;
		break;
	case REG_ECX :
		break;
	case REG_EDX :
		break;
	case REG_ESI :
		break;
	case REG_EDI :
		break;
	case REG_ESP :
		break;
	case REG_EBP :
		break;
	default : return ;
	}
	writeCode(opcode, SizeOfopcode, Const, 4);
}

void emitLOV_N(REG reg, int loc)
{
	emitLOV(reg, loc, VTYPE_NORMAL);
}

void emitLOV(REG reg, int loc, VTYPE vtype)
{
	int opcode = 0x8B;
	int SizeOfopcode = 2;

	char oprand = 0x00;
	char opmod = 0x00;
	char oprm  = 0x05;
	char opreg = 0x00;

	switch (reg)
	{
	case REG_EAX :
		opreg = 0x00;
		break;
	case REG_EBX :
        opreg = 0x03;
		break;
	case REG_ECX :
		opreg = 0x01;
		break;
	case REG_EDX :
		opreg = 0x02;
		break;
	case REG_ESI :
		opreg = 0x06;
		break;
	case REG_EDI :
		opreg = 0x07;
		break;
	case REG_ESP :
		opreg = 0x04;
		break;
	case REG_EBP :
		opreg = 0x05;
		break;
	default : return ;
	}
	oprand += (opreg << 3);
	oprand += oprm;

	opcode += (oprand << 8);

	writeCode(opcode, SizeOfopcode, loc, 4);
	AddVarRef(loc, offset - 4, vtype);
}


void emitAdd(REG reg1, REG reg2)
{
	int opcode = 0x03;
	int SizeOfopcode = 1;
	char opmod = 0xC0;
	char temp = 0x00;

	switch (reg2)
	{
	case REG_EAX :
		temp = 0x00;
		break;
	case REG_EBX :
		temp = 0x03;
		break;
	case REG_ECX :
		temp = 0x01;
		break;
	case REG_EDX :
		temp = 0x02;
		break;
	}
	opmod += temp;

	switch (reg1)
	{
	case REG_EAX :
		temp = 0x00;
		break;
	case REG_EBX :
		temp = 0x03;
		break;
	case REG_ECX :
		temp = 0x01;
		break;
	case REG_EDX :
		temp = 0x02;
		break;
	}
	opmod += (temp << 3);

	writeCode(opcode, SizeOfopcode, opmod, 1);
}

void emitMov_N(REG reg, int loc)
{
	emitMov(reg, loc, VTYPE_NORMAL);
}
void emitMov(REG reg, int loc, VTYPE vtype)
{
	int opcode = 0x89;
	int SizeOfopcode = 2;

	char oprand = 0x00;
	char opmod = 0x00;
	char oprm  = 0x05;
	char opreg = 0x00;

	switch (reg)
	{
	case REG_EAX :
		opreg = 0x00;
		break;
	case REG_EBX :
        opreg = 0x03;
		break;
	case REG_ECX :
		opreg = 0x01;
		break;
	case REG_EDX :
		opreg = 0x02;
		break;
	case REG_ESI :
		opreg = 0x06;
		break;
	case REG_EDI :
		opreg = 0x07;
		break;
	case REG_ESP :
		opreg = 0x04;
		break;
	case REG_EBP :
		opreg = 0x05;
		break;
	default : return ;
	}
	oprand += (opreg << 3);
	oprand += oprm;

	opcode += (oprand << 8);

	writeCode(opcode, SizeOfopcode, loc, 4);
	AddVarRef(loc, offset - 4, vtype);
}

void emitMul(REG reg1, REG reg2)
{
	int opcode = 0x0FAF;
	int SizeOfopcode = 2;
	char opmod = 0xC0;
	char temp = 0x00;

	switch (reg2)
	{
	case REG_EAX :
		temp = 0x00;
		break;
	case REG_EBX :
		temp = 0x03;
		break;
	case REG_ECX :
		temp = 0x01;
		break;
	case REG_EDX :
		temp = 0x02;
		break;
	}
	opmod += temp;

	switch (reg1)
	{
	case REG_EAX :
		temp = 0x00;
		break;
	case REG_EBX :
		temp = 0x03;
		break;
	case REG_ECX :
		temp = 0x01;
		break;
	case REG_EDX :
		temp = 0x02;
		break;
	}
	opmod += (temp << 3);

	writeCode(opcode, SizeOfopcode, opmod, 1);
}