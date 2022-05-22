// Made by Y. Sendov. May 2022

#define _CRT_SECURE_NO_WARNINGS
#define DEFAULT_ERROR -1
#define TRUE 1
#define FALSE 0
#define NEW_STR_MAX_LEN 27

#include <stdio.h>
#include <windows.h>
#include <string.h>

struct info
{
	char* code;
	int size;
};

int get_size(FILE* pointer)
{
	fseek(pointer, 0, SEEK_END);
	int size = ftell(pointer);
	rewind(pointer);
	return size;
}

void read_code(struct info* resource)
{
	FILE* file = fopen("source.txt", "r");
	if (file == NULL)
	{
		system("cls");
		printf("File Opening error.\nFile was expected: \"source.txt\".\nCheck the source directory!\n");
		exit(DEFAULT_ERROR);
	}
	char* code_text = (char*)malloc(sizeof(char));
	if (code_text != NULL)
	{
		char symbol = fgetc(file);
		unsigned int count = 0;
		code_text[count] = symbol;
		count++;
		while (!feof(file))
		{
			symbol = fgetc(file);
			char* temp = (char*)realloc(code_text, (count + 1) * sizeof(char));
			if (temp != NULL)
			{
				code_text = temp;
				code_text[count] = symbol;
			}
			count++;
		}
		code_text[count - 1] = '\0';
		resource->size = count - 1;
		resource->code = code_text;
	}
	fclose(file);
}

void delete_comments(char* code, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (code[i] == '/' && code[i + 1] == '/')
		{
			int count = i;
			while (code[count] != '\n') count++;
			int buffer = i;
			while (count < size)
			{
				code[buffer] = code[count];
				buffer++;
				count++;
			}
			while (buffer < size)
			{
				code[buffer] = 0;
				buffer++;
			}
		}
		if (code[i] == '/' && code[i + 1] == '*')
		{
			int count = i;
			while (!(code[count - 1] == '*' && code[count] == '/')) count++;
			count++;
			int buffer = i;
			while (count < size)
			{
				code[buffer] = code[count];
				buffer++;
				count++;
			}
			while (buffer < size)
			{
				code[buffer] = 0;
				buffer++;
			}
		}
	}
}

void delete_tabs(char* code, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (code[i] == '\t') code[i] = ' ';
	}
}

void delete_newline(char* code, int size)
{
	int marker = 0;
	for (int i = 0; i < size; i++)
	{
		if (code[i] == '#') marker++;
		if (code[i] == '\n')
		{
			if (marker == 0) code[i] = ' ';
			else marker--;
		}
	}
}

void change_names(struct info* resource)
{
	char* code = resource->code;
	int size = resource->size;
	int i = 0;
	while (i < size)
	{

		if (code[i] == 'i' && code[i + 1] == 'n' && code[i + 2] == 't')
		{

		}
		i++;
	}
}

void add_mess(struct info* resource)
{
	int size = resource->size;
	FILE* mess = fopen("mess.txt", "r");
	if (mess == NULL)
	{
		system("cls");
		printf("File Opening error.\nFile was expected: \"mess.txt\".\nCheck the source directory!\n");
		exit(DEFAULT_ERROR);
	}
	char* code = (char*)malloc(size * sizeof(char));
	if (code != NULL)
	{
		int iter = 0, counter = 0, loop = 0; // counter - счётчик количества добавленных мусорных переменных
		while (iter < size)
		{
			if (resource->code[iter] == '{' && resource->code[iter - 2] != '=')
			{
				counter++;
				code[loop] = resource->code[iter];
				char new_str[NEW_STR_MAX_LEN];
				fgets(new_str, NEW_STR_MAX_LEN, mess);
				new_str[strcspn(new_str, "\n")] = 0;
				char* temp = (char*)realloc(code, (size + (NEW_STR_MAX_LEN - 1) * counter) * sizeof(char));
				if (temp != NULL)
				{
					code = temp;
					for (int i = 1; i <= NEW_STR_MAX_LEN - 1; i++) code[loop + i] = new_str[i - 1];
					loop += NEW_STR_MAX_LEN - 1;
					iter++;
				}
				memset(new_str, 0, sizeof(new_str));
			}
			else
			{
				code[loop] = resource->code[iter];
				loop++;
				iter++;
			}
		}
		size = size + (NEW_STR_MAX_LEN - 2) * counter;
		char* temp = (char*)realloc(resource->code, size * sizeof(char));
		if (temp != NULL)
		{
			resource->code = temp;
			for (int i = 0; i < size; i++) resource->code[i] = code[i];
		}
		resource->size = size;
	}
	fclose(mess);
}

int main()
{
	struct info resource = { NULL, 0 };
	read_code(&resource);
	printf("Source code:\n");
	for (int i = 0; i < resource.size; i++) printf("%c", resource.code[i]);

	delete_comments(resource.code, resource.size);
	delete_tabs(resource.code, resource.size);
	delete_newline(resource.code, resource.size);
	//add_mess(&resource);
	//change_names(&resource);
	
	FILE* output = fopen("finale.txt", "w");
	printf("\nObfuscate code:\n");
	for (int i = 0; i < resource.size; i++)
	{
		printf("%c", resource.code[i]);
		fprintf(output, "%c", resource.code[i]);
	}
	fclose(output);
}