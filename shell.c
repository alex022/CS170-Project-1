//PENIS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define CLENGTH 1024
#define NONE 0
#define PIPE 1
#define BACKGROUND 2
#define OUT 3
#define IN 4

//int argCount(char*);
void parse(char*);
void op(char **, int, char**);
void split(char*, char**, int*, char**);

int main(void)
{  
  int delim = NONE;
  int command;
  char *input = (char*)malloc(sizeof(char)*CLENGTH);
  char *commands[CLENGTH];
  char *strptr = NULL;
  char cwd[CLENGTH];

  while(1)
    {
      printf("sish:> ");
      delim = NONE;
      getcwd(cwd, CLENGTH);
      fgets(input, CLENGTH, stdin);
      if(strcmp(input, "exit\n")==0)
	exit(0);
       
      split(input, commands, &delim, &strptr);
      if(strcmp(*commands, "cd") == 0)
	{
	  chdir(commands[1]);
	}
      else
	op(commands, delim, &strptr);
    }

  /* char input[CLENGTH];
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
  */
  return 0;
}

void split(char* input, char* commands[], int *delim, char **strptr)
{
  int command = 0;
  int end = 0;
  char *sptr = input;

  while((*sptr != '\0') && (!end))
    {
      *commands = sptr;
      command++;
      while((*sptr != ' ') && (*sptr != '\0') && (*sptr != '\n') && (!end))
	{
	  if(*sptr == '>')
	    {
	      *delim = OUT;
	      *commands = '\0';
	      sptr++;
	      while(*sptr == ' ')
		sptr++;

	      *strptr = sptr;
	      parse(*strptr);
	      end = 1;	      
	    }

	  if(*sptr == '<')
	    {
	      *delim = IN;
	      *commands = '\0';
	      sptr++;
	      while(*sptr == ' ')
		sptr++;

	      *strptr = sptr;
	      parse(*strptr);
	      end = 1;	      
	    }
	    
	  if(*sptr == '|')
	    {
	      *delim = PIPE;
	      *commands = '\0';
	      sptr++;
	      while(*sptr == ' ')
		sptr++;

	      *strptr = sptr;
	      end = 1;	      
	    }	    
	  
	  if(*sptr == '&')
	    *delim = BACKGROUND;
	
      sptr++;
    }
  while(((*sptr == ' ') || (*sptr == '\n')) && (!end))
    {
      *sptr = '\0';
      sptr++;
    }
    commands++;
  }

  *commands = '\0';
  //return command;
}


void parse(char *strptr)
{
  while((*strptr != ' ') && (*strptr != '\n'))
	strptr++;

  *strptr = '\0';
}

void op(char** commands, int delim, char** strptr)
{

  int pid, pidx;
  FILE *fp;
  int m = NONE;
  int command;
  int status, statusx;
  char *commandsx[CLENGTH];
  char* strptrx = NULL;
  int Pipe[2];

  if(delim==PIPE)
    {
      if(pipe(Pipe))
	{
	  printf("ERROR: Exec failed.\n");
	  exit(-1);
	}
      split(*strptr, commandsx, &m, &strptrx);
    }

  pid = fork();
  if(pid<0)
    {
       printf("ERROR: Exec failed.\n");
       exit(-1);
    }
  else if(pid==0)
    {
      if(delim==OUT)
	{
	  fp = fopen(*strptr, "w+");
	  dup2(fileno(fp), 1);	
	}
      if(delim==IN)
	{
	  fp = fopen(*strptr, "r");
	  dup2(fileno(fp), 0);
	}
      if(delim==PIPE)
	{
	  close(Pipe[0]);
	  dup2(Pipe[1], fileno(stdout));
	  close(Pipe[1]);
	}

      int rv = execvp(*commands, commands);
      if(rv==-1)
	printf("ERROR: Execution failed.\n");
    }
  else
    {
      if(delim==BACKGROUND);
      else if(delim==PIPE)
	{
	  wait(&status);
	  pidx = fork();
	  /*if(pidx < 0)
	    {
	      printf("ERROR: Fork failed.\n");
	      exit(-1);
	    }
	    else*/ if(pidx==0)
	    {
	      close(Pipe[1]);
	      dup2(Pipe[0], fileno(stdin));
	      close(Pipe[0]);
	      int rv = execvp(*commandsx, commandsx);
	      if(rv==-1)
		printf("ERROR: Execution failed.\n");
	    }
	  else
	    {
	      //wait(NULL);
	      //waitpid(pid, &status, 0);
	      //waitpid(pidx, &statusx, 0);
	      close(Pipe[0]);
	      close(Pipe[1]);
	    }

	}
      else
	wait(&status);
        wait(NULL);
    }
}


/*void parse(char* in, char** arguments)
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
*/

/*int argCount(char* buffer)
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
*/
