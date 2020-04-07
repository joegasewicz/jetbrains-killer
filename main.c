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
#define DEBUG true
#define SIZE_OF_PROCESS_DATA_ARRAY(x) (sizeof(x)/sizeof((x)[0]))
#define SIZE_OF_IDES(i) (sizeof(i)/sizeof((i)[0]))


struct ProcessData {
	char fullStr[MAX_OUTPUT];
	char pid[PID_LEN];
	char path[PATH_LEN];
	bool isKilled;
	char ideName[9];
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

int IDE_COUNT = 0;

int compareProcessOutputToToken(const char ideTokens[], char *processStr, char *tokenResult, CompareData *compareData)
{
	tokenResult = strstr(processStr, ideTokens);
	if(tokenResult != NULL)
	{
		IDE_COUNT +=1; 
		compareData->tokenName  = malloc(strlen(ideTokens) + 1);
		compareData->processStr = malloc(strlen(processStr) + 1);
		strcpy(compareData->tokenName, ideTokens);
		// Remove any whitespace from start of the path
		if(processStr[0] == ' ')
		{
			memmove(processStr, processStr+1, strlen(processStr));
		}
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
        char pid[6];
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
						strncat(pid, &compareResults.processStr[i], 1);
					} 
					else 
					{
						strcpy(processDataArray[counter].ideName, compareResults.tokenName);
						strcpy(processDataArray[counter].fullStr, compareResults.processStr);
        				        strcpy(processDataArray[counter].pid, pid);
						counter += 1;
						memset(pid, 0, 6);
						break;
					}
				}
			}

		}	
		if(strlen(processDataArray[counter].fullStr) != 0) break;
	}

	// Close popen proccess
	pclose(psOutP);

	if(DEBUG == true)
	{
		printf("// ----------------------------------- //\n");
		printf("//   	Jetbrains Killer debug mode    //\n");
		printf("// ----------------------------------- //\n");
	}

	if(SIZE_OF_PROCESS_DATA_ARRAY(processDataArray))
	{
		for(int i = 0; i < SIZE_OF_PROCESS_DATA_ARRAY(processDataArray); i++) 
		{
			if(strlen(processDataArray[i].pid))
			{
				char killCommand[20] = "sudo kill -9 ";
				strncat(killCommand, processDataArray[i].pid, 20);
        			system(killCommand);
				if(DEBUG == true)
				{
					printf("| path |\t%s\n", processDataArray[i].fullStr);
					printf("| pid |\t%s\n", processDataArray[i].pid);
					printf("| command |\t%s\n", killCommand);
					printf("| ides open |\t%i\n", IDE_COUNT);
				}


			}
		}
	}
		
	return 0;
}
