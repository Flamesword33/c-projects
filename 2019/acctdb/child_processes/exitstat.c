#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

void exit_status(int status)
{
	if (WIFEXITED(status))
		printf("normal termination - exit status = %d\n",
			   WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		printf("abnormal termination - signal number = %d", WTERMSIG(status));

#ifdef  WCOREDUMP
		if (WCOREDUMP(status))
			printf(" (core dumped)");
#endif

		printf("\n");
	}
	else if (WIFSTOPPED(status))
		printf("child stopped - signal number = %d\n", WSTOPSIG(status));
}
