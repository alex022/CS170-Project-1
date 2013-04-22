#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLENGTH 1024

int argCount(char*);
void parse(char*, int, char**);

int main(void)
{
	char input[CLENGTH];
	char command[CLENGTH];
	char pipe[100][100];
	char* path = "/bin/";
	char* split;

	int comNum = 0;

	while(1)
	{
      	printf("sish:> ");
		fgets(input, CLENGTH, stdin);
		input[strlen(input)-1] = '\0';

		split = strtok(input, "|");
		while(1)
		{
			if(split != NULL)
			{
				comNum++;
				strcpy(pipe[comNum], split);
				split = strtok(NULL, "|");
			}
			else
				break;
		}

		int pid = fork(); //If pid != 0, then parent process. Else, child process

		if(pid == 0)
		{
			int num_args = argCount(input);
			char* args[num_args+1];
			parse(input, num_args, args);
			args[num_args] = NULL;
			
			strcpy(command, path);
			strcat(command, args[0]);
			int rv = execv(command, args);
		}
		else
			wait(); //Parent process

    	if(strcmp(input, "exit") == 0)
	  		break;

		if(strcmp(input, "lm") == 0){
			printf("ERROR: exec failed\n");
			continue;
	}

    }

  return 0;
}

int argCount(char buffer[])
{
	int args = 0;
	char* token;
	
	token = strtok(buffer," ");
	while(1)
	{
		if(token != NULL)
		{
			args++;
			token = strtok(NULL, " ");
		}
		else
			break;
	}
	return args;
}

void parse(char in[], int argNum, char* arguments[])
{
	char* token;
	int i;
	char buffer[512];

	token = strtok(in," ");
	for(i = 0; i < argNum; i++)
	{
		if(token != NULL)
		{
			strcpy(buffer, in);
			arguments[i] = buffer;
			token = strtok(NULL, " ");
		}
	}
}



