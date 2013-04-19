#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{

  char command[20];

  while(1)
    {
      printf("sish:> ");
      fgets(command, 20, stdin);
      
      

      if(strncmp(command, "exit", 4)==0)
	break;


      printf("\n");
    }


  return 0;
}
