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
  
  int i, k;

  int comNum = 0;
  
  while(1)
    {
      printf("sish:> ");
      fgets(input, CLENGTH, stdin);
      input[strlen(input)-1] = '\0';
      
      if(strcmp(input ,"exit")==0)
	exit(0);

      if(feof(stdin))
	{
	  printf("\n");
	  break;
	}
      
      int pid = fork(); //If pid != 0, then parent process. Else, child process
	  
      if(pid == 0)
	{
	  int num_args = argCount(input);
	  char** args = malloc(((num_args+1)*sizeof(char*)));

	  parse(input, args);
	 
	  args[num_args] = NULL;
	  
	if(strcmp(args[0], "cd")==0)
	  {
	    char curdir[CLENGTH];
	    if(getcwd(curdir, sizeof(curdir)) != NULL)
	      {
		strcat(curdir, "/");
		strcat(curdir, args[1]);		
		chdir(curdir);  		
	      }
	    continue;
	  }

	  char* command = malloc(strlen(args[0]+strlen(path))*sizeof(char*));
	  command = strcat(strcpy(command, path), args[0]);
	  int rv = execv(command, args);
	  
	    if(rv == -1)
	      {
		printf("ERROR: Exec failed.\n");
		if(pid==0)
		  exit(0);
	      }


	}
      else
	wait(NULL); //Parent process
	  
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



