#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void execCommandR(char *cmd, char *rest, int inputFD){
	char *nextCmd = strtok_r(rest, "|", &rest);
	if(nextCmd){
		int fd[2];
		pipe(fd);
		if(fork() == 0){

		}
		if(inputFD != STDIN_FILENO) 
			close(inputFD);
		close(fd[1]);

		execCommandR(nextCmd, rest, fd[0]);
	}
	else{

	}
}

void execCommand(char *cmd){
	char *firstCmd = strtok_r(cmd, "|", &cmd);
	execCommandR(firstCmd, cmd, STDIN_FILENO);
}

int main(int argc, char ** argv){
	char prompt[15];
	size_t len = 0:
	char *cmd = NULL;
	FILE *input = stdin;

	if(argc > 1){
		input = fopen(argv[1], "r");
		if(input == NULL){
			fprintf(stderr, "Couldn't open file because %s\n", strerror(errno));
			return 0;
		}
		prompt[0] = '\0';
	}
	else 
		strncpy(prompt, "juasjuas> ", sizeof(prompt));

	while(1){
		printf("%s", prompt);
		
		if(getline(&cmd, &len, input) == -1)
			break;

		if(strcmp(cmd, "exit"))
			execCommand(cmd);
		else
			break;
	}

	return 0;
}