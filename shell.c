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
       
      if(feof(stdin))
	{
	  printf("\n");
	  exit(0);
	}

      split(input, commands, &delim, &strptr);
      if(strcmp(*commands, "cd") == 0)
	{
	  chdir(commands[1]);
	} 
      

      else
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
	      printf("hit\n");
	      if(pipe(Pipe))
		{
		  printf("ERROR: Exec failed.\n");
		  exit(-1);
		}
	       split(strptr, commandsx, &m, &strptrx);
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
		  fp = fopen(strptr, "w+");
		  dup2(fileno(fp), 1);	
		  //	  split(strptr, commandsx, &m, &strptrx);
		}
	      if(delim==IN)
		{
		  fp = fopen(strptr, "r");
		  dup2(fileno(fp), 0);
		  //  split(strptr, commandsx, &m, &strptrx);
		}
	      if(delim==PIPE)
		{
		  close(Pipe[0]);
		  dup2(Pipe[1], fileno(stdout));
		  close(Pipe[1]);
		  //  split(strptr, commandsx, &m, &strptrx);
		}
	      
	      int rv = execvp(*commands, commands);
	      
	      if(rv==-1)
		{
		printf("ERROR: Exec failed.\n");
		exit(-1);
		}
	    }
	  else
	    {
	      if(delim==BACKGROUND);
	      else if(delim==PIPE)
		{
		  printf("PIIIIIIPE\n");
		  wait(&status);
		  pidx = fork();
		  if(pidx==0)
		    {
		      close(Pipe[1]);
		      dup2(Pipe[0], fileno(stdin));
		      close(Pipe[0]);
		      int rv = execvp(*commandsx, commandsx);
		      if(rv==-1)
			{
			  printf("ERROR: Exec failed.\n");
			  exit(-1);
			}		
		    }
		  else
		    {
		      close(Pipe[0]);
		      close(Pipe[1]);
		    }
		  wait(NULL);
		}
	      else 
		waitpid(pid, &status, 0);
	    }
	}
    }
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
}

void parse(char *strptr)
{
  while((*strptr != ' ') && (*strptr != '\n'))
    strptr++;

  *strptr = '\0';
}
