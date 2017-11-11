#define _CRT_SECURE_NO_WARNINGS // Go away.

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	FILE* file;
	char* fileName;
	char* fileData;
	size_t i;
	size_t totalFileNameSize;
	size_t totalDataSize;

	totalFileNameSize = 0;

	if (argc < 2 || argv == NULL || argv[1] == NULL)
	{
		printf("You need to give me a file to create a C header hex dump of!\n");
		return 0;
	}

	for (i = 1; i < argc; i++)
	{
		if (argv[i] == NULL)
			continue;

		totalFileNameSize += strlen(argv[i]);
		if (argc > i + 1)
			totalFileNameSize++;
	}

	totalFileNameSize += 3; // .h\0

	fileName = (char*)malloc(totalFileNameSize);

	if (!fileName)
	{
		printf("Couldn't allocate memory of size %zu bytes for file name buffer.", totalFileNameSize);
		return 0;
	}

	fileName[0] = '\0';

	for (i = 1; i < argc; i++)
	{
		if (argv[i] == NULL)
			continue;

		strcat(fileName, argv[i]);
		if (argc > i + 1)
			strcat(fileName, " ");
	}

	fileName[totalFileNameSize - 3] = '\0';

	file = fopen(fileName, "r");
	if (!file)
	{
		printf("Couldn't open file for reading: %s!", fileName);
		return 0;
	}

	fileName[totalFileNameSize - 3] = '.';
	fileName[totalFileNameSize - 2] = 'h';
	fileName[totalFileNameSize - 1] = '\0';

	fseek(file, 0, SEEK_END);
	totalDataSize = ftell(file);

	if (totalDataSize == 0)
	{
		printf("File has size of 0.");
		return 0;
	}

	fseek(file, 0, SEEK_SET);
	fileData = (char*)malloc(totalDataSize);
	if (!fileData)
	{
		printf("Couldn't allocate memory of size %zu bytes for file data buffer.", totalDataSize);
		return 0;
	}

	fread((void*)fileData, totalDataSize, 1, file);
	fclose(file);

	file = fopen(fileName, "w");
	if (!file)
	{
		printf("Couldn't open file for writing: %s!", fileName);
		return 0;
	}

	fileName[totalFileNameSize - 3] = '\0';
	for (i = 0; i < totalFileNameSize; i++)
	{
		if (fileName[i] == '\0')
			break;

		if ( (fileName[i] > 'z' || fileName[i] < 'A') )
			fileName[i] = '_';
	}

	fprintf(file, 
		"#ifndef %s_H\n"
		"#define %s_H\n"
		"\n"
		"#include <stdlib.h>\n"
		"\n"
		"const size_t %s_size = %zu;\n"
		"const unsigned char %s_data[] = \n"
		"{\n\t",
		fileName, fileName, fileName, totalDataSize, fileName);

	if (totalDataSize >= 1)
		fprintf(file, "0x%02X", (unsigned int)fileData[0]);

	for (i = 1; i < totalDataSize; i++)
		fprintf(file, ",0x%02X", (unsigned int)fileData[i]);

	fprintf(file, "\n};\n\n");
	fprintf(file, "#endif\n");
	fclose(file);

	free(fileData);
	free(fileName);

	return 1;
}