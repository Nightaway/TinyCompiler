#include <stdio.h>

#include "parser.h"
#include "scanner.h"
#include "symtab.h"
#include "analyzer.h"
#include "cgen.h"
#include "utility.h"
#include "boo-vm/vm.h"

/*
		Դ�����ļ�
*/
FILE *code;

/*
		��������Դ�����ַ��� 
*/
extern char *pStream;


/*
		������ǰ��
*/
void front_end(const char *soureCode, TreeNode **tree);

/*
		����������
*/
void back_end(const char *objCode, TreeNode *tree);



int main(int argc, char *argv[])
{
	// Դ�����ַ���
	const char *sourceCode;

	// �����﷨��
	TreeNode *tree;

	// 
	char objName[40];
	
	// ��ȡԴ�����ļ����ļ���
	sourceCode = argv[1];

	// ���к��ļ�׺�滻 etc. "example.txt" -> "example.tobj"
	replaceFilenameExt(sourceCode, "asm", objName);

	// ǰ��  ����: Դ�����ļ��ַ���   �����������﷨�� 
	front_end(sourceCode, &tree);

	// ����  ���룺�����﷨��        ������Windows�µ�exe��ִ���ļ�
	back_end(objName, tree);

	// �ɹ� ���� 0
	return 0;
}

void front_end(const char *soureCode, 
			   TreeNode **tree)
{
	// Դ�����ļ�
	FILE *sourceFile;
	char *bufString;

	// �ļ���С byte
	int fileSize = 0;

	// �����ļ����Ƿ�Ϊ��
	if (soureCode == NULL) 
	{
		printf("SourceCode file name cannot be null.\n");
		exit(1);
	}

	// ����Դ�����ļ�
	sourceFile = fopen(soureCode, "rb");

	// �ļ�����ʧ��
	if (sourceFile == NULL)
	{
		printf("Can not open sourceCode file: %s .\n", soureCode);
		exit(2);
	}

	/*	
			�����ַ���������־
	*/
	fseek(sourceFile, 0, SEEK_END);
	fileSize = ftell(sourceFile);
	fseek(sourceFile, 0, SEEK_SET);
	bufString = (char *) malloc(fileSize + 2);
	fread(bufString, 1, fileSize, sourceFile);
	fclose(sourceFile);
	printf("file size : %d bytes \n", fileSize - 2);
	bufString[fileSize] = '#';
	bufString[fileSize + 1] = 'e';

	pStream = bufString;

	// ���ɳ����﷨��
	*tree = parse();

	// �ͷŴ���Դ�����ַ�����ռ�õ��ڴ�
	free(bufString);

	// �������ű�
	buildSymtab(*tree);

	// ���ͼ���
	typeCheck(*tree);
}

void back_end(const char *objCode, 
			  TreeNode *tree)
{
	char *asm_code = codeGen(tree);
	printf("Assembler Code:\n");
	printf("%s", asm_code);
	printf("Reuslt:\n");
	boovm::VM vm;
	vm.Compile(asm_code, strlen(asm_code));
	vm.Run();
	printf("\n", asm_code);
}
