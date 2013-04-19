#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLENGTH 1024

int main(void)
{
	char input[CLENGTH];

  	while(1)
       {
      	printf("sish:> ");
      	fgets(input, CLENGTH, stdin);
	input[strlen(input)-1] = '\0';

      
      	if(strcmp(input, "exit") == 0)
	  break;


       
    }

  return 0;
}
