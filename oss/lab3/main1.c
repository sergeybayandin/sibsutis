#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define PROCESSES_COUNT  4
#define MAX_COMMAND_SIZE 64

int main()
{
	
	pid_t* processes;
	if ((processes = malloc(PROCESSES_COUNT * sizeof *processes)) == NULL) {
		return EXIT_FAILURE;
	}
	*processes = getpid();
	for (size_t i = 1; i < PROCESSES_COUNT; ++i) {
		processes[i] = fork();
		if (processes[i] == 0)
			waitpid(*processes, NULL, 0);
	}
	if (getpid() != *processes) 
		return EXIT_SUCCESS;
	system("pstree | grep prog1"); puts("");
	char command[MAX_COMMAND_SIZE];
	for (size_t i = 0; i < PROCESSES_COUNT; ++i) {
		sprintf(command, "ls /proc/%d/task/*", processes[i]);
		printf("pid %d:\n", processes[i]); system(command); puts("");
	}
	return EXIT_SUCCESS;
}
