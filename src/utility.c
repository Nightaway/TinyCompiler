#include "globals.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
		替换文件后缀名
*/
#define FILE_CHAR_MAX 20
#define EXT_CHAR_MAX  5

typedef enum {
	S_FILE,
	S_EXT
};

void replaceFilenameExt(const char *sourceFile, 
						const char *ext, 
						char *outFile)
{
	int state = S_FILE;
	int file_buffer_index = 0;
	int ext_buffer_index  = 0;
	char file_buffer[FILE_CHAR_MAX] = {0};
	char ext_buffer[EXT_CHAR_MAX]   = {0};
	char curr;

	curr = *sourceFile;
	while (curr != '\0')
	{
		curr = *sourceFile;
		++sourceFile;

		if (curr != '.')
		{
			switch (state)
			{
			case S_FILE :
				file_buffer[file_buffer_index++] = curr;

				if (file_buffer_index > FILE_CHAR_MAX)
				{
					printf("filename is too long.\n");
					exit(1);
				}
				break;

			case S_EXT :
				ext_buffer[ext_buffer_index++] = curr;

				if (ext_buffer_index > EXT_CHAR_MAX)
				{
					printf("ext is too long.\n");
					exit(1);
				}
				break;
			}
		} else {
			state = S_EXT;
		}
	}
	file_buffer[file_buffer_index] = '\0';
	ext_buffer[ext_buffer_index]   = '\0'; 
	/*
			重组字符串
	*/
	strcpy(outFile, file_buffer);
	outFile[file_buffer_index] = '.';
	strcpy(outFile+file_buffer_index+1, ext);
}