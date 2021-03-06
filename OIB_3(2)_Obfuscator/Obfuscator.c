// Made by Y. Sendov. May 2022

#define _CRT_SECURE_NO_WARNINGS
#define DEFAULT_ERROR -1

#define TRUE 1
#define FALSE 0

#define NEW_STR_MAX_LEN 28
#define NEW_NAME_MAX_LEN 10

#define LEN_INT_MAIN 10
#define LEN_INT 3
#define LEN_CHAR 4
#define LEN_FLOAT 5
#define LEN_BOOL 4
#define LEN_FILE 4
#define LEN_DOUBLE 5
#define LEN_SHORT 5
#define LEN_LONG 4
#define LEN_VOID 4

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct info
{
	char* text_code;
	int text_size;
};

void read_text(struct info* resource)
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
		unsigned int counts = 0;
		code_text[counts] = symbol;
		counts++;
		while (!feof(file))
		{
			symbol = fgetc(file);
			char* temp = (char*)realloc(code_text, (counts + 1) * sizeof(char));
			if (temp != NULL)
			{
				code_text = temp;
				code_text[counts] = symbol;
			}
			counts++;
		}
		code_text[counts - 1] = '\0';
		resource->text_size = counts - 1;
		resource->text_code = (char*)malloc(resource->text_size * sizeof(char));
		if (resource->text_code != NULL)
		{
			for (int i = 0; i < resource->text_size; i++) resource->text_code[i] = code_text[i];
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
			int counts = i;
			while (code[counts] != '\n') counts++;
			int buffer = i;
			while (counts < size)
			{
				code[buffer] = code[counts];
				buffer++;
				counts++;
			}
			while (buffer < size)
			{
				code[buffer] = 0;
				buffer++;
			}
		}
		if (code[i] == '/' && code[i + 1] == '*')
		{
			int counts = i;
			while (!(code[counts - 1] == '*' && code[counts] == '/')) counts++;
			counts++;
			int buffer = i;
			while (counts < size)
			{
				code[buffer] = code[counts];
				buffer++;
				counts++;
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
	int mark = 0;
	for (int i = 0; i < size; i++)
	{
		if (code[i] == '#') mark++;
		if (code[i] == '\n')
		{
			if (mark == 0) code[i] = ' ';
			else mark--;
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
		int iter = 0, counter = 0, loop = 0; // counter - ??????? ?????????? ??????????? ???????? ??????????
		while (iter < size)
		{
			if (code[iter] == '{' && code[iter - 1] != '=' && code[iter - 2] != '=' && code[iter - 1] != '\'')
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
		char* temp = (char*)realloc(resource->text_code, size * sizeof(char));
		if (temp != NULL)
		{
			resource->text_code = temp;
			for (int i = 0; i < size; i++)
			{
				resource->text_code[i] = code_text[i];
			}
		}
		resource->text_size = size;
	}
	fclose(mess);
}

void change_names(char* code, int size, struct info* resource)
{
	FILE* names = fopen("names.txt", "r");
	if (names == NULL)
	{
		system("cls");
		printf("File Opening error.\nFile was expected: \"mess.txt\".\nCheck the source directory!\n");
		exit(DEFAULT_ERROR);
	}
	char new_name[15] = { 0 };
	int count = 0, diff = 0;
	char* code_text = (char*)malloc(size * sizeof(char));
	if (code_text != NULL)
	{
		int sym = 0;
		int iter = 0;
		int check = 0;
		while(TRUE)
		{
			if (sym == size) break;
			// Processing the int main()
			if (code[sym] == 'i' && code[sym + 1] == 'n' && code[sym + 2] == 't' && code[sym + 3] == ' ' && code[sym + 4] == 'm' && code[sym + 5] == 'a' && code[sym + 6] == 'i' && code[sym + 7] == 'n' && code[sym + 8] == '(')
			{
				for (int j = 0; j <= LEN_INT_MAIN; j++) code_text[iter + j] = code[sym + j];
				sym += LEN_INT_MAIN + 1;
				iter += LEN_INT_MAIN + 1;
				check = 0;
			}
			// Processing the int or int*
			if (code[sym] == 'i' && code[sym + 1] == 'n' && code[sym + 2] == 't' && (code[sym + 3] == ' ' || code[sym + 3] == '*') && code[sym + 4] != ')')
			{
				if (code[sym + 3] == '*')
				{
					for (int j = 0; j <= LEN_INT + 1; j++) code_text[iter + j] = code[sym + j];
					sym += LEN_INT + 2;
					iter += LEN_INT + 2;
				}
				else if (code[sym + 3] == ' ')
				{
					for (int j = 0; j <= LEN_INT; j++) code_text[iter + j] = code[sym + j];
					sym += LEN_INT + 1;
					iter += LEN_INT + 1;
				}
				check = 1;
			}
			// Processing the char or char*
			else if (code[sym] == 'c' && code[sym + 1] == 'h' && code[sym + 2] == 'a' && code[sym + 3] == 'r' && (code[sym + 4] == ' ' || code[sym + 4] == '*') && code[sym + 5] != ')')
			{
				if (code[sym + 4] == '*')
				{
					for (int j = 0; j <= LEN_CHAR + 1; j++) code_text[iter + j] = code[sym + j];
					sym += LEN_CHAR + 2;
					iter += LEN_CHAR + 2;
				}
				else if (code[sym + 4] == ' ')
				{
					for (int j = 0; j <= LEN_CHAR; j++) code_text[iter + j] = code[sym + j];
					sym += LEN_CHAR + 1;
					iter += LEN_CHAR + 1;
				}
				check = 1;
			}
			// Processing the void
			else if (code[sym] == 'v' && code[sym + 1] == 'o' && code[sym + 2] == 'i' && code[sym + 3] == 'd' && (code[sym + 4] == ' ' || code[sym + 4] == '*'))
			{
				if (code[sym + 4] == '*')
				{
					for (int j = 0; j <= LEN_VOID + 1; j++) code_text[iter + j] = code[sym + j];
					sym += LEN_VOID + 2;
					iter += LEN_VOID + 2;
				}
				else if (code[sym + 4] == ' ')
				{
					for (int j = 0; j <= LEN_VOID; j++) code_text[iter + j] = code[sym + j];
					sym += LEN_VOID + 1;
					iter += LEN_VOID + 1;
				}
				check = 1;
			}
			// Processing the float or float*
			else if (code[sym] == 'f' && code[sym + 1] == 'l' && code[sym + 2] == 'o' && code[sym + 3] == 'a' && code[sym + 4] == 't' && (code[sym + 5] == ' ' || code[sym + 5] == '*'))
			{
				if (code[sym + 5] == '*')
				{
					for (int j = 0; j <= LEN_FLOAT + 1; j++) code_text[iter + j] = code[sym + j];
					sym += LEN_FLOAT + 2;
					iter += LEN_FLOAT + 2;
				}
				else if (code[sym + 5] == ' ')
				{
					for (int j = 0; j <= LEN_FLOAT; j++) code_text[iter + j] = code[sym + j];
					sym += LEN_FLOAT + 1;
					iter += LEN_FLOAT + 1;
				}
				check = 1;
			}
			// Processing the double or double*
			else if (code[sym] == 'd' && code[sym + 1] == 'o' && code[sym + 2] == 'u' && code[sym + 3] == 'b' && code[sym + 4] == 'l' && code[sym + 5] == 'e' && (code[sym + 6] == ' ' || code[sym + 6] == '*'))
			{
				if (code[sym + 6] == '*')
				{
					for (int j = 0; j <= LEN_DOUBLE + 1; j++) code_text[iter + j] = code[sym + j];
					sym += LEN_DOUBLE + 2;
					iter += LEN_DOUBLE + 2;
				}
				else if (code[sym + 6] == ' ')
				{
					for (int j = 0; j <= LEN_DOUBLE; j++) code_text[iter + j] = code[sym + j];
					sym += LEN_DOUBLE + 1;
					iter += LEN_DOUBLE + 1;
				}
				check = 1;
			}
			// Processing the short or short*
			else if (code[sym] == 's' && code[sym + 1] == 'h' && code[sym + 2] == 'o' && code[sym + 3] == 'r' && code[sym + 4] == 't' && (code[sym + 5] == ' ' || code[sym + 5] == '*'))
			{
				if (code[sym + 5] == '*')
				{
					for (int j = 0; j <= LEN_SHORT + 1; j++) code_text[iter + j] = code[sym + j];
					sym += LEN_SHORT + 2;
					iter += LEN_SHORT + 2;
				}
				else if (code[sym + 5] == ' ')
				{
					for (int j = 0; j <= LEN_SHORT; j++) code_text[iter + j] = code[sym + j];
					sym += LEN_SHORT + 1;
					iter += LEN_SHORT + 1;
				}
				check = 1;
			}
			// Processing the long or long*
			else if (code[sym] == 'l' && code[sym + 1] == 'o' && code[sym + 2] == 'n' && code[sym + 3] == 'g' && (code[sym + 4] == ' ' || code[sym + 4] == '*') && code[sym + 5] != 'd')
			{
				if (code[sym + 4] == '*')
				{
					for (int j = 0; j <= LEN_LONG + 1; j++) code_text[iter + j] = code[sym + j];
					sym += LEN_LONG + 2;
					iter += LEN_LONG + 2;
				}
				else if (code[sym + 4] == ' ')
				{
					for (int j = 0; j <= LEN_LONG; j++) code_text[iter + j] = code[sym + j];
					sym += LEN_LONG + 1;
					iter += LEN_LONG + 1;
				}
				check = 1;
			}
			else
			{
				code_text[iter] = code[sym];
				iter++;
				sym++;
				check = 0;
			}
			// Save the current position
			int save_sym = sym;
			int save_iter = iter;
			int vsize = 0;
			char* variable = (char*)malloc(sizeof(char));
			if (check == 1 && variable != NULL)
			{
				count = 0, vsize = 0;
				while (code[sym] != ' ' && code[sym] != '[' && code[sym] != '(' && code[sym] != ',' && code[sym] != ';' && code[sym] != ')')
				{
					char* temp1 = (char*)realloc(variable, (vsize + 1) * sizeof(char));
					if (temp1 != NULL)
					{
						variable = temp1;
						variable[vsize] = code[sym];
						sym++;
						vsize++;
					}
				}
				variable[vsize] = '\0';
				fgets(new_name, sizeof(new_name), names);
				new_name[strcspn(new_name, "\n")] = 0;
				diff = NEW_NAME_MAX_LEN - vsize;
				int control = 0;
				char* temp2 = (char*)realloc(code_text, (size + diff) * sizeof(char));
				if (temp2 != NULL)
				{
					code_text = temp2;
					for (int j = 0; j < NEW_NAME_MAX_LEN; j++)
					{
						code_text[iter] = new_name[j];
						iter++;
					}
				}
				count++;
				for (int j = sym; j < size; j++)
				{
					control = 0;
					for (int p = 0; p < vsize; p++)
					{
						if (code[j + p] == variable[p]) control = 1;
						else
						{
							control = 0;
							break;
						}
					}
					if (control == 1 && (code[j + vsize] == ' ' || code[j + vsize] == '[' || code[j + vsize] == '(' || code[j + vsize] == ')' || code[j + vsize] == '+' || code[j + vsize] == '-' || code[j + vsize] == ',' || code[j + vsize] == ';' || code[j + vsize] == ']'))
					{
						count++;
						char* temp3 = (char*)realloc(code_text, (size + diff * count) * sizeof(char));
						if (temp3 != NULL)
						{
							code_text = temp3;
							for (int k = 0; k < NEW_NAME_MAX_LEN; k++)
							{
								code_text[iter] = new_name[k];
								iter++;
							}
						}
						j += vsize - 1;
					}
					else
					{
						code_text[iter] = code[j];
						iter++;
					}
				}
				int new_size = size + diff * count;
				char* temp4 = (char*)realloc(code, new_size * sizeof(char));
				if (temp4 != NULL)
				{
					code = temp4;
					for (int i = 0; i < new_size; i++)
					{
						code[i] = code_text[i];
					}
				}
				size = new_size;
			}
			sym = save_sym + vsize;
			iter = save_iter + vsize;
			sym = iter;
		}
		resource->text_code = NULL;
		char* tmp = (char*)realloc(resource->text_code, size * sizeof(char));
		if (tmp != NULL)
		{
			resource->text_code = tmp;
			for (int i = 0; i < size; i++)
			{
				resource->text_code[i] = code_text[i];
			}
		}
		resource->text_size = size;
	}
	fclose(names);
}

int main()
{
	clock_t time_start = clock();
	struct info resource = { NULL, 0 };
	read_text(&resource);
	printf("Source code:\n");
	for (int i = 0; i < resource.text_size; i++) printf("%c", resource.text_code[i]);

	delete_comments(resource.text_code, resource.text_size);
	if (resource.text_code != NULL) resource.text_size = strlen(resource.text_code);
	change_names(resource.text_code, resource.text_size, &resource);
	add_mess(resource.text_code, resource.text_size, &resource);
	delete_tabs(resource.text_code, resource.text_size);
	delete_newline(resource.text_code, resource.text_size);
	
	FILE* output = fopen("obfuscate_code.txt", "w");
	printf("\nObfuscate code:\n");
	for (int i = 0; i < resource.text_size; i++)
	{
		printf("%c", resource.text_code[i]);
		fprintf(output, "%c", resource.text_code[i]);
	}
	fclose(output);
	clock_t time_end = clock();
	double dtime = (time_end - time_start) * 1000 / CLOCKS_PER_SEC;
	printf("\n\n Time spent on obfuscation of the program: %lf\n", dtime);
}