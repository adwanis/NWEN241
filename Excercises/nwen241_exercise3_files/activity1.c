#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>

int main()
{

	int pid, ret_exec, status;
	pid=fork();

	switch(pid){

	case -1:
	/* code for case -1 */
		perror("Error");
        exit(EXIT_FAILURE);


	case 0:
	/* code for case 0 */
		ret_exec = execl("/bin/ps", "ps", "-A", NULL);
        if (ret_exec == -1) {
            perror("Error executing exec");
            exit(1);
        }
        exit(EXIT_SUCCESS);


	default:
	/* code for case default */
		pid = wait(&status);
        if (pid == -1) {
            perror("Error waiting for child process");
            exit(EXIT_FAILURE);
        }
        if (WIFEXITED(status)) {
            printf("Parent process ID: %d\n", getpid());
            printf("Child process ID: %d\n", pid);
            printf("Child process termination status: %d\n", WEXITSTATUS(status));
        }

	}

	return 0;

 }