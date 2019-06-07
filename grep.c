#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <sys/file.h>

#define MAX (1<<11)

int main(int argc, char **argv){
	if(argc<2){
		printf("No se dio string a buscar.\n");
		return 1;
	}
	char search[MAX];
	char files[argc-1][MAX];
	int flag[3],i,j,k;
	flag[0]=0;
	flag[1]=0;
	flag[2]=0;
	j=0;
	for(i=1;i<argc;i++){
		if(argv[i][0]=='-'){
			k=1;
			while(argv[i][k]!='\0'){
				if(argv[i][k]=='i'){flag[0]=1;}
				else if(argv[i][k]=='v'){flag[1]=1;}
				k++;
			}
		}
		else if(flag[2]){
			strcpy(files[j],argv[i]);
			j++;
		}
		else{
			strcpy(search,argv[i]);
			flag[2]=1;
		}

	}
	if(j==0){
		char *input = NULL;
		size_t len = 0;
		while(getline(&input, &len, stdin) != -1){
			if(flag[0]){
				if(strcasestr(input,search)>0){
					if(!flag[1]) printf("%s",input);
				}
				else if(flag[1]) printf("%s",input); 
			}
			else{
				if(strstr(input,search)>0){
					if(!flag[1]) printf("%s",input);
				}
				else if(flag[1]) printf("%s",input); 
			}
		}
	}
	else{
		FILE *file;
		char *line = NULL;
		size_t len = 0;
		for(i=0;i<j;i++){
			file=fopen(files[i], "r");
			while(getline(&line, &len, file) != -1){
				if(flag[0]){
					if(strcasestr(line,search)>0){
						if(!flag[1]){
							if(j==1) printf("%s",line);
							else printf("%s:\t%s",files[i],line);
						}
					}
					else if(flag[1]){
						if(j==1) printf("%s",line);
						else printf("%s:\t%s",files[i],line);
					}
				}
				else{
					if(strstr(line,search)>0){
						if(!flag[1]){
							if(j==1) printf("%s",line);
							else printf("%s:\t%s",files[i],line);
						}
					}
					else if(flag[1]){
						if(j==1) printf("%s",line);
						else printf("%s:\t%s",files[i],line);
					}
				}
			}
			fclose(file);
		}
	}
}