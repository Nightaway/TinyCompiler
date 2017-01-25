#ifndef __CODE_H_
#define __CODE_H_

typedef struct TinyObjectFileHeader{
	int magic;
	int NumberOfVaribles;
	int OffsetToVarSection;
	int NumberOfLabels;
	int OffsetToLabelSection;
	int SizeOfCode;
	int OffsetToCodeSection;
	int NumberOfTempVaribles;
	int OffsetToTempVarSection;
	int OffsetToVarRefSection;
	int OffsetToTempVarRefSection;
	int unused0;
} TinyObjectFileHeader;

typedef struct VarOffset{
	int offset;

	struct VarOffset *pPrev;
	struct VarOffset *pNext;
} VarOffset;

typedef struct VaribleRef{
	int loc;
	VarOffset VarOffsets;

	struct VarOffset *pOffsetTail;

	struct VaribleRef *pPrev;
	struct VaribleRef *pNext;
} VaribleRef;
//

/*
	Regisers
*/
typedef enum REG{
	REG_EAX,
	REG_EBX,
	REG_ECX,
	REG_EDX,
	REG_ESI,
	REG_EDI,
	REG_ESP,
	REG_EBP
}REG;

/*
    VaribleTypes
*/
typedef enum {
	VTYPE_NORMAL,
	VTYPE_TEMP
}VTYPE;

void fillSection();
void fillCodeSection();

void writeFileHeader();
void updateFileHeader();

void writeVarSection();
void writeTempVarSection();
void writeVarRefSection();
void writeTempVarRefSection();

void writeCode(int opcode, int sizeofopcode, 
			   int oprand, int sizeofoprand);
//////////

void AddVarRef(int loc, int offset, VTYPE vtype);

///////////
int mallocTempVar();
void freeTempVar();


/*
	Load Const to Register
*/
void emitLOC(REG reg, const int Const);

/*
	Load Varible to Register
*/
void emitLOV_N(REG reg, int loc);
void emitLOV(REG reg, int loc, VTYPE vtype);

/*
	Add 
*/
void emitAdd(REG reg1, REG reg2);

/*
    Mov
*/

void emitMov_N(REG reg, int loc);
void emitMov(REG reg, int loc, VTYPE vtype);
/*
    Mul
*/
void emitMul(REG reg1, REG reg2);
#endif