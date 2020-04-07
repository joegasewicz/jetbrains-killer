#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_OUTPUT 1000
#define PID_LEN 40
#define PATH_LEN 2500
#define PROCESS_DATA_LEN 1000
#define JETBRAINS_IDES_LEN 10
#define IDE_NAME_LEN 9

#define SIZE_OF_PROCESS_DATA_ARRAY(x) (sizeof(x)/sizeof((x)[0]))
#define SIZE_OF_IDES(i) (sizeof(i)/sizeof((i)[0]))


struct ProcessData {
	char fullStr[MAX_OUTPUT];
	char pid[PID_LEN];
	char path[PATH_LEN];
	bool isKilled; 
};

const char ides[][IDE_NAME_LEN] = {
	"pycharm",
	"webstorm",
	"clion",
	"phpstorm",
	"datagrip",
	"intellij",
	"goland",
	"ryder"
};

typedef struct CompareData {
	char tokenName;
	char processStr;
} comapreData;

void compareProcessOutputToToken(
		const char ideTokens[IDE_NAME_LEN],
		char *processStr,
		char *tokenResult
		)
{
	for(int i = 0; i < sizeof(&ideTokens)/sizeof(ideTokens[0]); i++)
       	{
		tokenResult = strstr(processStr, &ideTokens[i]);
		if(tokenResult != NULL) {
			printf("here------> %s", tokenResult);  
		}
	}	
}

int main()
{
	FILE *psOutP;
	char processData[PROCESS_DATA_LEN];
	struct ProcessData processDataArray[PROCESS_DATA_LEN];
	int counter = 0;
        const char token[10] = "pycharm";
        char *hasToken;
        char pid[30];
	char killCommand[20] = "sudo kill -9 ";

	psOutP = popen("ps -A", "r");
	
	if(psOutP == NULL)
	{
		printf("There are no current Jetbrain IDE's running\n");
		exit(0);
	}
	

	while(fgets(processData, MAX_OUTPUT, psOutP) != NULL)
	{
		hasToken = strstr(processData, token);

		for(int i = 0; i < SIZE_OF_IDES(ides); i++)
		{
			compareProcessOutputToToken(ides[i], processData, hasToken);
		}
		if(hasToken != NULL) 
		{
			// Get PID
			for(int i = 0; i < strlen(processData); i++)
		       	{
				if (processData[i] != ' ')
			       	{
					strncat(pid, &processData[i], 1);
				} 
				else 
				{
					counter += 1;
					strcpy(processDataArray[counter].fullStr, processData);
        			        strcpy(processDataArray[counter].pid, pid);
					break;
				}
			}
		}
		if(strlen(processDataArray[counter].fullStr) != 0) break;
	}
	
	if(SIZE_OF_PROCESS_DATA_ARRAY(processDataArray) > 0)
	{
		pclose(psOutP);
        	strncat(killCommand, processDataArray[1].pid, 20);
        	system(killCommand);
	}
		
	return 0;
}
