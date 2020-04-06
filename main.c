#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_OUTPUT 1000
#define PID_LEN 40
#define PATH_LEN 2500
#define PROCESS_DATA_LEN 30000

struct ProcessData {
	char fullStr[MAX_OUTPUT];
	char pid[PID_LEN];
	char path[PATH_LEN];
	bool isKilled; 
};


int main()
{
	FILE *psOutP;
	char processData[1000];
	struct ProcessData processDataArray[1000];
	int counter = 0;
        char *c;
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

		
	pclose(psOutP);
	strncat(killCommand, processDataArray[1].pid, 20);
	system(killCommand);
	return 0;
}
