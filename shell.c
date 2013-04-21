#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLENGTH 1024

int main(void)
{
	char input[CLENGTH];
	char command[5] = "";
	char* split;

	while(1)
	{
      	printf("sish:> ");
		fgets(input, CLENGTH, stdin);
		input[strlen(input)-1] = '\0';

		//split = strtok(input, " ");
		//if(split != NULL)
			//strcpy(command, split);

    	if(strcmp(input, "exit") == 0)
	  			break;

	if(strcmp(input, "lm") == 0){
			printf("ERROR: exec failed\n");
			continue;
	}

		system(input); //Works for the most part and is extremely convenient, but we need 					   //to deal with spaces and output adjustments. Hope the prof lets 						   //us use this function.
    }

  return 0;
}
