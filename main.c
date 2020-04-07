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

typedef struct CompareDatas {
	char *tokenName;
	char *processStr;
} CompareData;

int compareProcessOutputToToken(const char ideTokens[], char *processStr, char *tokenResult, CompareData *compareData)
{
	tokenResult = strstr(processStr, ideTokens);
	if(tokenResult != NULL)
	{
		compareData->tokenName  = malloc(strlen(ideTokens) + 1);
		compareData->processStr = malloc(strlen(processStr) + 1);
		strcpy(compareData->tokenName, ideTokens);
	        strcpy(compareData->processStr, processStr);
		return 0;	
	}
	return 1;	
}

int main()
{
	FILE *psOutP;
	char processData[PROCESS_DATA_LEN];
	struct ProcessData processDataArray[IDE_NAME_LEN];
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
		// CompareData struct is a temporary storage for the current path string
		// being tested. With each iteration it will reallocate the path value to the
		// same address.
		int isAssigned;
		CompareData compareResults;

		for(int i = 0; i < SIZE_OF_IDES(ides); i++)
		{
			isAssigned = compareProcessOutputToToken(ides[i], processData, hasToken, &compareResults);
			if(isAssigned != 1) 
			{
				for(int i = 0; i < strlen(compareResults.processStr); i++)
		       		{
					if (compareResults.processStr[i] != ' ')
			       		{
						strncat(pid, compareResults.processStr, 1);
					} 
					else 
					{
						strcpy(processDataArray[counter].fullStr, compareResults.processStr);
        				        strcpy(processDataArray[counter].pid, pid);
						counter += 1;
						break;
					}
				}
			}

		}
				if(strlen(processDataArray[counter].fullStr) != 0) break;
	}
	
	if(SIZE_OF_PROCESS_DATA_ARRAY(processDataArray) > 0)
	{
		for(int i = 0; i < SIZE_OF_PROCESS_DATA_ARRAY(processDataArray); i++) 
		{
			if(strlen(processDataArray[i].pid))
			{
				pclose(psOutP);
        			strncat(killCommand, processDataArray[i].pid, 20);
				printf("now ------> %s\n", killCommand);
        			system(killCommand);

			}
		}
	}
		
	return 0;
}
