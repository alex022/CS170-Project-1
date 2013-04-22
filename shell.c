#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CLENGTH 1024

int argCount(char*);
void parse(char*, char**);

int main(void)
{
  char input[CLENGTH];
  char* path = "/bin/";
  char pipe[100][100]; 
  char* split;
  
  int comNum = 0;
  
  while(1)
    {
      printf("sish:> ");
      fgets(input, CLENGTH, stdin);
      input[strlen(input)-1] = '\0';
	  
      /* split = strtok(input, "|");
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
	 }*/

      int pid = fork(); //If pid != 0, then parent process. Else, child process
	  
      if(pid == 0)
	{
	  int num_args = argCount(input);
	  char** args = malloc(((num_args+1)*sizeof(char*)));

	  parse(input, args);
	  int k = 0;
	  args[num_args] = NULL;
	  
	  char* command = malloc(strlen(args[0]+strlen(path))*sizeof(char*));
	  command = strcat(strcpy(command, path), args[0]);
	  int rv = execv(command, args);
	}
      else
	wait(NULL); //Parent process
	  
      if(strcmp(input, "exit") == 0)
	break;
	  
      if(strcmp(input, "lm") == 0){
	printf("ERROR: exec failed\n");
	continue;
      }
	  
    }
	
  return 0;
}

int argCount(char* buffer)
{
  int args = 0;
  char* token;
  char in[CLENGTH];

  strcpy(in, buffer);
  token = strtok(in," ");
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

void parse(char* in, char** arguments)
{
  char* token;
  int i = 0;

  token = strtok(in, " ");
  while(token != NULL)
    {
      arguments[i] = (char*)malloc((strlen(token)*sizeof(char*)));
      strcpy(arguments[i], token);
      token = strtok(NULL, " ");
      i++;
    }
}



