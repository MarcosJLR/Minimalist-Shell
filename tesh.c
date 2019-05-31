#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

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
		
	}

	return 0;
}