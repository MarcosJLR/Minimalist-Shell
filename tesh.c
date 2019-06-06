#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

int splitStr(char *s, const char *delim, char ***splitArr){
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
	
	for(i = 0; isspace(s[i]); i++);
	
	for(j = n-1; isspace(s[i]); j--);
	
	for(int k = i; k <= j; k++)
		s[k-i] = s[k];
	
	s[j+1] = '\0';
}

void getIOFiles(char *cmd, char *in, char *out){
	int n = strlen(cmd);
	int inPos = -1, outPos = -1;
	in[0] = out[0] = '\0';
	for(int i = 0; i < n; i++){
		if(s[i] == '<'){ 
			if(outPos != -1)
				out[outPos] = '\0';
			inPos = 0;
			outPos = -1;
		}
		else if(s[i] == '>'){
			if(inPos != -1)
				in[inPos] = '\0';
			inPos = -1;
			outPos = 0;
		}
		else{
			if(inPos != -1)
				in[inPos++] = s[i];
			else if(outPos != -1)
				out[outPos++] = s[i];
		}
	}
	
	if(inPos != -1)
		in[inPos] = '\0';
	if(outPos != -1)
		out[outPos] = '\0';

	trim(in);
	trim(out);
}

void execCommand(char *cmd, int inFD, int outFD){
	if(fork() == 0){
		char inFile[PATH_MAX], outFile[PATH_MAX];
		char *realCmd = strtok(cmd, "<>");
		char **argv = NULL;

		getIOFiles(cmd, inFile, outFile);
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

		splitStr(realCmd, " \n", &argv);

		
	}
}

void execCommandLine(char *cmd){
	char **arr = NULL;
	int n = splitStr(cmd, "|", &arr);
	int inputFD = STDIN_FILENO;

	for(int i = 0; i < n; i++){
		if(i == n-1)
			execCommand(arr[i], inputFD, STDOUT_FILENO);
		else{
			int fd[2];
			pipe(fd);
			execCommand(arr[i], inputFD, fd[1]);
			close(fd[1]);
			inputFD = fd[0];
		}

		if(inputFD != STDIN_FILENO)
			close(inputFD);
	}

	for(int i = 0; i < n; i++)
		wait(NULL);
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