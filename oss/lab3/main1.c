#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define PROCESSES_COUNT  4
#define MAX_COMMAND_SIZE 64

int main()
{
	pid_t ppid, pid;
	ppid = getpid();
	for (size_t i = 1; i < PROCESSES_COUNT; ++i) {
		pid = fork();
		if (pid == 0) {
			waitpid(ppid, NULL, 0);
			break;
		} else {
			printf("pid: %d\n", pid);
		}
	}
	if (getpid() != ppid) 
		return EXIT_SUCCESS;
	system("pstree | grep prog1"); puts("");
	char command[MAX_COMMAND_SIZE];
	sprintf(command, "cat /proc/%d/task/%d/children", (int)ppid, (int)ppid);
	printf("ppid: %d\n", ppid);
	system(command); puts("");
	while (1);
	return EXIT_SUCCESS;
}
