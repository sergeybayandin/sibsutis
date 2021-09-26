#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main()
{
	pid_t child_pid;
	if ((child_pid = fork()) == 0) {
		while (1);
	} else {
		system("ps | grep \"prog\"");
		kill(getpid(), SIGTERM);
	}
	return EXIT_SUCCESS;
}
