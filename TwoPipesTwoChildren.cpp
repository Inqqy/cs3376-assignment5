#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv){
	int status;
	int childpid;
	int childpid2;

	char *cat_args[] = {"ls", "-ltr", NULL};
	char *grep_args[] = {"grep","3376", NULL};
	char *wc_args[] = {"wc","-l",NULL};

	//create one pipe to send the output of "ls" to "grep"
	int pipes[2];
	pipe(pipes);
	
	//create a pipe to send output of "grep" to "wc"
	int pipes2[2];
	pipe(pipes2);

	int pipes3[2];
	pipe(pipes3);

	//fork first child (to execute cat)
	if((childpid = fork()) == -1){
		perror("Error creating a child process");
		exit(1);
	}
	if(childpid == 0){
	//we're in the child
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
	dup2(pipes[0],0);
	close(pipes[0]);
	close(pipes[1]);

	//fork second child (to execute grep)
	if((childpid2 = fork()) == -1){
		perror("Error creating a child process");
		exit(1);
	}
	if(childpid2 == 0){
	//we're in the child
	//replace grep's stdout with the file descriptor of 2nd pipe
		dup2(pipes2[1], 1);
		close(pipes2[0]);
		close(pipes2[1]);
		execvp(*grep_args, grep_args);
		exit(0);
	}
	else{
		dup2(pipes2[0],0);
		close(pipes2[0]);
		close(pipes2[1]);
		execvp(*wc_args, wc_args);
		

	}
	}
	return(0);
}
