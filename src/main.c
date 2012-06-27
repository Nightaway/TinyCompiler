#include <stdio.h>

#include "parser.h"
#include "scanner.h"
#include "symtab.h"
#include "analyzer.h"
#include "cgen.h"
#include "linker.h"
#include "utility.h"

/*
		源代码文件
*/
FILE *code;

/*
		用于贮存源代码字符串 
*/
extern char *pStream;


/*
		编译器前端
*/
void front_end(const char *soureCode, TreeNode **tree);

/*
		编译器后端
*/
void back_end(const char *objCode, TreeNode *tree);



int main(int argc, char *argv[])
{
	// 源代码字符串
	const char *sourceCode;

	// 抽象语法树
	TreeNode *tree;

	// 
	char objName[40];
	
	// 获取源代码文件的文件名
	sourceCode = argv[1];

	// 进行后文件缀替换 etc. "example.txt" -> "example.tobj"
	replaceFilenameExt(sourceCode, "tobj", objName);

	// 前端  输入: 源代码文件字符串   输出：抽象语法树 
	front_end(sourceCode, &tree);

	// 后端  输入：抽象语法树        输出：Windows下的exe可执行文件
	back_end(objName, tree);

	// 成功 返回 0
	return 0;
}

void front_end(const char *soureCode, 
			   TreeNode **tree)
{
	// 源代码文件
	FILE *sourceFile;
	char *bufString;

	// 文件大小 byte
	int fileSize = 0;

	// 检查文件名是否为空
	if (soureCode == NULL) 
	{
		printf("SourceCode file name cannot be null.\n");
		exit(1);
	}

	// 打开源代码文件
	fopen_s(&sourceFile, soureCode, "rb");

	// 文件打开失败
	if (sourceFile == NULL)
	{
		printf("Can not open sourceCode file: %s .\n", soureCode);
		exit(2);
	}

	/*	
			插入字符串结束标志
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

	// 生成抽象语法树
	*tree = parse();

	// 释放储存源代码字符串所占用的内存
	free(bufString);

	// 创建符号表
	buildSymtab(*tree);

	// 类型检查
	typeCheck(*tree);
}

void back_end(const char *objCode, 
			  TreeNode *tree)
{
	fopen_s(&code, objCode, "wb");
	if (code == NULL)
		printf("\nCan't create tiny obj file.");
	codeGen(tree ,objCode);
	fclose(code);
	link(objCode);
}
