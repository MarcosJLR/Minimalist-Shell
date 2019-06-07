#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define MAX (1<<10)

int main(int argc, char **argv){
	if(argc<2){
		printf("No se dio string a buscar.\n");
		return 1;
	}
	char files[argc-1][MAX];
	int flag[6],i,j,k;
	for(i=0;i<6;i++){
		flag[i]=0;
	}
	j=0;
	for(i=1;i<argc;i++){
		if(argv[i][0]=='-'){
			k=1;
			while(argv[i][k]!='\0'){
				if(argv[i][k]=='r'){flag[1]=1;}
				else if(argv[i][k]=='w'){flag[3]=1;}
				else if(argv[i][k]=='x'){flag[5]=1;}
				k++;
			}
		}
		else if(argv[i][0]=='+'){
			k=1;
			while(argv[i][k]!='\0'){
				if(argv[i][k]=='r'){flag[0]=1;}
				else if(argv[i][k]=='w'){flag[2]=1;}
				else if(argv[i][k]=='x'){flag[4]=1;}
				k++;
			}
		}
		else{
			strcpy(files[j],argv[i]);
			j++;
		}
	}
	struct stat fileStat;
	for(i=0;i<j;i++){
		if(stat(files[i],&fileStat) < 0){
			printf("Problema abriendo archivo %s",files[i]);
			perror("stat()");
		}
		else{
			if(flag[0] && !flag[1]){
				chmod(files[i], fileStat.st_mode | S_IRUSR | S_IRGRP | S_IROTH);
			}
			if(!flag[0] && flag[1]){
				chmod(files[i], fileStat.st_mode & ~S_IRUSR & ~S_IRGRP & ~S_IROTH);
			}
			if(flag[2] && !flag[3]){
				chmod(files[i], fileStat.st_mode | S_IWUSR | S_IWGRP | S_IWOTH);
			}
			if(!flag[2] && flag[3]){
				chmod(files[i], fileStat.st_mode & ~S_IWUSR & ~S_IWGRP & ~S_IWOTH);
			}
			if(flag[4] && !flag[5]){
				chmod(files[i], fileStat.st_mode | S_IXUSR | S_IXGRP | S_IXOTH);
			}
			if(!flag[4] && flag[5]){
				chmod(files[i], fileStat.st_mode & ~S_IXUSR & ~S_IXGRP & ~S_IXOTH);
			}
		}
	}
}