#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int splitStr(char *s, const char *delim, char ***splitArr){
	int cnt = 0, n = strlen(s);
	
	for(int i = 0; i < n; i++)
		if(s[i] != delim[0] && s[i] != '\n' && (i == 0 || s[i-1] == delim[0]))
			cnt++;
	
	char **arr = malloc((cnt + 1) * sizeof(char *));

	arr[0] = strtok(s, delim);

	for(int i = 1; i < cnt + 1; i++){
		arr[i] = strtok(NULL, " \n");
		if(arr[i] == NULL) break;
	}

	*splitArr = arr;

	return cnt;
}

void execCommand(char *cmd, int inFD, int outFD){

}

int execCommandLineR(char *cmd, char *rest, int inputFD){
	if(cmd == NULL) return;

	char *nextCmd = strtok_r(rest, "|", &rest);
	if(nextCmd){ 
		int fd[2], ret;
		pipe(fd);
		
		execCommand(cmd, inputFD, fd[1]);

		ret = execCommandLineR(nextCmd, rest, fd[0]);

		free(nextCmd);

		return ret + 1;
	}
	else{
		execCommand(cmd, inputFD, STDOUT_FILENO);
		
		return 1;
	}
}

int execCommandLine(char *cmd){
	char *firstCmd = strtok_r(cmd, "|", &cmd);
	return execCommandLineR(firstCmd, cmd, STDIN_FILENO);
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
			execCommandLine(cmd);
		else
			break;
	}

	return 0;
}