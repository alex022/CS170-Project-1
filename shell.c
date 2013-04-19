#include <stdio.h>
#include <stdlib.h>

int main(void)
{

  char command[20];

  while(1)
    {
      printf("sish:> ");
      fgets(command, 20, stdin);
      
      

      if(strcmp(command, "exit")==0)
	break;


      printf("\n");
    }


  return 0;
}
