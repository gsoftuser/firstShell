// firstShell.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <stdio.h>
//#include <process.h>
#include <direct.h>    // _chdir

#define FSH_RL_BUFSIZE 1024
char *fsh_read_line(void)
{
	int bufsize = FSH_RL_BUFSIZE;
	int position = 0;
	char *buffer = (char *)malloc(sizeof(char)*bufsize);
	int c;

	if (!buffer) 
	{
		fprintf(stderr, "fsh: allocation memory error\n");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		c = getchar();
		if (c == EOF || c == '\n')
		{
			buffer[position] = '\0';
			return buffer;
		}
		else
		{
			buffer[position] = c;
			position++;
		}
		//position++;

		if (position >= bufsize)
		{
			bufsize += FSH_RL_BUFSIZE;
			buffer = (char *)realloc(buffer, bufsize);
			if (!buffer)
			{
				fprintf(stderr, "fsh: allocation memory error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}
/*
char* fsh_read_line(void)
{
	char *line = NULL;
	ssize_t buffsize = 0; // have getline allocate a buffer
	//getline(&line, &buffer, stdin);
	return line;
}
*/
#define FSH_TOK_BUFFSIZE 64
#define FSH_TOK_DELIM "\t\r\n\a"

char **fsh_split_line(char *line)
{
	int bufsize = FSH_TOK_BUFFSIZE;
	int position = 0;
	char **tokens = (char **)malloc(bufsize * sizeof(char*));
	char *token;
	char *next_token = NULL;
	if (!tokens)
	{
		fprintf(stderr, "fsh: allocation memory error \n");
		exit(EXIT_FAILURE);
	}

	token = strtok_s(line, FSH_TOK_DELIM,&next_token);
	while (token != NULL)
	{
		tokens[position] = token;
		position++;
		if (position >= bufsize)
		{
			bufsize += FSH_TOK_BUFFSIZE;
			tokens = (char **)realloc(tokens, bufsize * sizeof(char*));
			if (!tokens)
			{
				fprintf(stderr, "FSH: allocation memory error\n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok_s(NULL, FSH_TOK_DELIM,&next_token);
	}
	tokens[position] = NULL;
	return tokens;
}


int fsh_launch(char **args)
{
	//pid_t pid;
	//pid_t wpid;

	//int status;

	//pid = fork();
	//pid = _wspawnl(0, filename, 0);
	//execvp(args[0], args == -1)
		//_wexec
	printf("args[0]:%s ", args[0]);
	return 1;

}

/*
  Function Declarations for builtin shell commands:
 */
int fsh_cd(char **args);
int fsh_help(char **args);
int fsh_exit(char **args);

const char *builtin_str[] =
{
	"cd",
	"help",
	"exit"
};

int(*builtin_func[])(char **) =
{
	&fsh_cd,
	&fsh_help,
	&fsh_exit
};

int fsh_num_builtins()
{
	return sizeof(builtin_str) / sizeof(char*);
}

int fsh_cd(char **args)
{
	if (args[1] == NULL)
	{
		fprintf(stderr, "fsh: expected argument to \"cd\"\n");
	}
	else
	{
		if (_chdir(args[1]) != 0)
		{
			perror("fsh");
		}
	}
	return 1;
}

int fsh_help(char **args)
{
	int i;
	printf("My first shell...\n");
	printf("Type program names and arguments, and hit enter.\n");
	printf("The folloing are built in \n");

	for (i = 0; i < fsh_num_builtins(); i++)
	{
		printf("  %s\n", builtin_str[i]);
	}

	printf("Use the man command for information on other programs. \n");
	return 1;
}



int fsh_exit(char **args)
{
	return 0;
}

int fsh_execute(char **args)
{
	int i;
	if (args[0] == NULL)
	{
		return 1;
	}

	for (i = 0; i < fsh_num_builtins(); i++)
	{
		if (strcmp(args[0], builtin_str[i]) == 0)
		{
			return(*builtin_func[i])(args);
		}
	}
	//return fsh_launch(args);
	return 0;
}

void fsh_loop(void) 
{
	char *line;
	char **args;
	int status;

	std::cout << "Welcome ! \n";
	std::cout << "First shell from brennan 2019/04/26 version 0.01 \n";

		do {
			printf(">");
			line = fsh_read_line();
			args = fsh_split_line(line);
			//status = fsh_execute(args);
			status = 0;

			free(line);
			free(args);
		} while (status);
}


int main()
{
    //std::cout << "Hello World!\n"; 
	// Load config files, if any

	// Run command loop
	fsh_loop();

	return EXIT_SUCCESS;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
