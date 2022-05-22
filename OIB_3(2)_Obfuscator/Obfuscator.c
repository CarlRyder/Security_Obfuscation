// Made by Y. Sendov. May 2022

#define _CRT_SECURE_NO_WARNINGS
#define DEFAULT_ERROR -1
#define TRUE 1
#define FALSE 0
#define NEW_STR_MAX_LEN 28

#include <stdio.h>
#include <windows.h>
#include <string.h>

struct info
{
	char* code;
	int size;
};

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
		resource->code = (char*)malloc(resource->size * sizeof(char));
		if (resource->code != NULL)
		{
			for (int i = 0; i < resource->size; i++) resource->code[i] = code_text[i];
		}
		free(code_text);
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

void add_mess(char* code, int size, struct info* resource)
{
	FILE* mess = fopen("mess.txt", "r");
	if (mess == NULL)
	{
		system("cls");
		printf("File Opening error.\nFile was expected: \"mess.txt\".\nCheck the source directory!\n");
		exit(DEFAULT_ERROR);
	}
	char* code_text = (char*)malloc(size * sizeof(char));
	if (code_text != NULL)
	{
		int iter = 0, counter = 0, loop = 0; // counter - счётчик количества добавленных мусорных переменных
		while (iter < size)
		{
			if (code[iter] == '{' && code[iter - 1] != '=' && code[iter - 2] != '=')
			{
				counter++;
				code_text[loop] = code[iter];
				char new_str[NEW_STR_MAX_LEN] = { 0 };
				fgets(new_str, NEW_STR_MAX_LEN, mess);
				new_str[strcspn(new_str, "\n")] = 0;
				char* temp = (char*)realloc(code_text, (size + (NEW_STR_MAX_LEN - 1) * counter) * sizeof(char));
				if (temp != NULL)
				{
					code_text = temp;
					for (int i = 1; i <= NEW_STR_MAX_LEN - 1; i++) code_text[loop + i] = new_str[i - 1];
					loop += NEW_STR_MAX_LEN - 1;
					iter++;
				}
			}
			else
			{
				code_text[loop] = code[iter];
				loop++;
				iter++;
			}
		}
		size = size + (NEW_STR_MAX_LEN - 2) * counter;
		char* temp = (char*)realloc(resource->code, size * sizeof(char));
		if (temp != NULL)
		{
			resource->code = temp;
			for (int i = 0; i < size; i++)
			{
				resource->code[i] = code_text[i];
			}
		}
		resource->size = size;
	}
	fclose(mess);
}

void change_str(char* code, int size)
{
	int i = 0;
	while (TRUE)
	{
		if (i + 1 == size) break;
		if (code[i] == '"')
		{
			int count = i + 1;
			while (code[count] != '"')
			{
				code[count]++;
				count++;
				if (count == size) return;
			}
			i = count + 1;
		}
		else i++;
	}
}

void change_names(char* code, int size, struct info* resource)
{
	
}

int main()
{
	struct info resource = { NULL, 0 };
	read_code(&resource);
	printf("Source code:\n");
	for (int i = 0; i < resource.size; i++) printf("%c", resource.code[i]);

	delete_comments(resource.code, resource.size);
	if (resource.code != NULL) resource.size = strlen(resource.code);
	add_mess(resource.code, resource.size, &resource);
	delete_tabs(resource.code, resource.size);
	delete_newline(resource.code, resource.size);
	change_str(resource.code, resource.size);
	//change_names(&resource);
	
	FILE* output = fopen("obfuscate_code.txt", "w");
	printf("\nObfuscate code:\n");
	for (int i = 0; i < resource.size; i++)
	{
		printf("%c", resource.code[i]);
		fprintf(output, "%c", resource.code[i]);
	}
	fclose(output);
}