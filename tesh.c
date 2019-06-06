#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <sys/wait.h>

int splitStr(char *s, const char *delim, char ***splitArr){
	if(s == NULL) return 0;

	int cnt = 0, n = strlen(s);
	
	for(int i = 0; i < n; i++)
		if(s[i] != delim[0] && s[i] != '\n' && (i == 0 || s[i-1] == delim[0]))
			cnt++;
	
	char **arr = malloc((cnt + 1) * sizeof(char *));

	arr[0] = strtok(s, delim);

	for(int i = 1; i < cnt + 1; i++){
		arr[i] = strtok(NULL, delim);
		if(arr[i] == NULL) break;
	}

	*splitArr = arr;

	return cnt;
}

void trimStr(char *s){
	int i,j,n = strlen(s);

	if(n == 0) return;

	for(i = 0; isspace((unsigned char) s[i]); i++);
	
	for(j = n-1; isspace((unsigned char) s[i]); j--);
	
	for(int k = i; k <= j; k++)
		s[k-i] = s[k];
	
	s[j-i] = '\0';
}

void getIOFiles(char *cmd, char *in, char *out){
	int n = strlen(cmd);
	int inPos = -1, outPos = -1;
	in[0] = out[0] = '\0';

	for(int i = 0; i < n; i++){
		if(cmd[i] == '<'){ 
			if(outPos != -1)
				out[outPos] = '\0';
			inPos = 0;
			outPos = -1;
		}
		else if(cmd[i] == '>'){
			if(inPos != -1)
				in[inPos] = '\0';
			inPos = -1;
			outPos = 0;
		}
		else{
			if(inPos != -1)
				in[inPos++] = cmd[i];
			else if(outPos != -1)
				out[outPos++] = cmd[i];
		}
	}
	
	if(inPos != -1)
		in[inPos] = '\0';
	if(outPos != -1)
		out[outPos] = '\0';

	trimStr(in);
	trimStr(out);
}

void execCommand(char *cmd, int inFD, int outFD){
	if(fork() == 0){
		char inFile[PATH_MAX], outFile[PATH_MAX];
		getIOFiles(cmd, inFile, outFile);

		char *realCmd = strtok(cmd, "<>");
		char **argv = NULL;

		if(inFile[0] != '\0'){
			FILE *in = fopen(inFile, "r");
			inFD = fileno(in);
		}
		if(outFile[0] != '\0'){
			FILE *out = fopen(outFile, "w");
			outFD = fileno(out);
		}

		dup2(inFD, STDIN_FILENO);
		dup2(outFD, STDOUT_FILENO);

		if(splitStr(realCmd, " \n", &argv) == 0){
			exit(0);
		}

		if(strcmp(argv[0], "ls") == 0){
			char aux[] = "./myls";
			argv[0] = aux;
			execvp(argv[0], argv);
		}
		else if(strcmp(argv[0], "grep") == 0){
			char aux[] = "./mygrep";
			argv[0] = aux;
			execvp(argv[0], argv);
		}
		else if(strcmp(argv[0], "chmod") == 0){
			char aux[] = "./mychmod";
			argv[0] = aux;
			execvp(argv[0], argv);
		} 
		else{
			fprintf(stderr, "tesh: %s: command not found\n", argv[0]);
			exit(0);
		}

		exit(0);
	}
	else
		wait(NULL);
}

void execCommandLine(char *cmd){
	char **arr = NULL;
	int n = splitStr(cmd, "|", &arr);
	int inputFD = STDIN_FILENO;
	for(int i = 0; i < n; i++){
		if(i == n-1){
			execCommand(arr[i], inputFD, STDOUT_FILENO);
			if(inputFD != STDIN_FILENO)
				close(inputFD);
		}
		else{
			int fd[2];
			pipe(fd);
			execCommand(arr[i], inputFD, fd[1]);
			
			if(inputFD != STDIN_FILENO)
				close(inputFD);
			
			close(fd[1]);
			
			inputFD = fd[0];
		}		
	}
}

int main(int argc, char ** argv){
	char prompt[15];
	size_t len = 0;
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

		if(strcmp(cmd, "exit\n"))
			execCommandLine(cmd);
		else
			break;
	}

	return 0;
}