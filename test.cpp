#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv){
	int status;
	int childpid;

	char *cat_args[] = {"ls", "-ltr", NULL};
	char *grep_args[] = {"grep","3376", NULL};

	//create one pipe to send the output of "ls" to "grep"
	int pipes[2];
	pipe(pipes);

	//fork first child (to execute cat)
	if((childpid = fork()) == -1){
		perror("Error creating a child process");
		exit(1);
	}
	if(childpid == 0){
       	//replace cat's stdout with the write file descriptor of 1st pipe
		dup2(pipes[1], 1);
		close(pipes[0]);
		close(pipes[1]);
		execvp(*cat_args, cat_args);
		exit(0);
	}
	else{
	//we're in the parent
	//replace grep's stdin with read end of 1st pipe
		dup2(pipes[0], 0);
		close(pipes[0]);
		close(pipes[1]);
		execvp(*grep_args, grep_args);

	}
	return(0);
}
