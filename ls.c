#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <time.h>

#define MAX_PATH (1<<10)



void listFiles(char *path, int depth, int flag[6]){
	DIR *direc;
	direc = opendir(path);
	if(direc == NULL){
		printf("Failed at opening path: %s\n",path);
		return;
	}
	struct dirent *ent;
	char dir[100][MAX_PATH];
	char aux[MAX_PATH];
	struct stat fileStat;
	int i=0;
	if(flag[5] || flag[4]){
		printf("%s:\n",path);
	}
	while((ent = readdir(direc)) != NULL){
		if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0){
			strcpy(aux,path);
			strcat(aux,ent->d_name);
			if(stat(aux,&fileStat) < 0){
				perror("Problema abriendo archivo");
				return 1;
			}
			if(flag[0]){
				printf("%d\t",fileStat.st_ino);
			}

			printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
			printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
			printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
			printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
			printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
			printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
			printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
			printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
			printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
			printf( (fileStat.st_mode & S_IXOTH) ? "x\t" : "-\t");

			printf("%d\t",fileStat.st_nlink);
			
			if(!flag[2]){
				struct passwd *us = getpwuid(fileStat.st_uid);
				printf("%s\t",us->pw_name);
			}
			if(!flag[1]){
				struct group *gr = getgrgid(fileStat.st_gid);
				printf("%s\t",gr->gr_name);
			}

			if(flag[3]){
				if(fileStat.st_size<1024){
					printf("%d\t",fileStat.st_size);
				}
				else if(fileStat.st_size>800*1024){
					double sz=(float)fileStat.st_size/(1024*1024);
					printf("%0.1lfM\t",sz);
				}
				else{
					double sz=(float)fileStat.st_size/1024;
					printf("%0.1lfK\t",sz);
				}
			}
			else{
				printf("%d\t",fileStat.st_size);
			}

			struct tm *time=localtime(&fileStat.st_mtim);
			int year=1900+time->tm_year;
			int month=1+time->tm_mon;
			printf("%d/%d/%d\t%d:%d\t",year,month,time->tm_mday,time->tm_hour,time->tm_min);			

			printf("%s\n",ent->d_name);
			if(ent->d_type == DT_DIR && flag[4])
			{
				sprintf(dir[i], "%s%s/", path, ent->d_name);
				i++;
			}
		}
	}
	if(flag[4] && !depth){
		for(int j=0;j<i;j++){
			listFiles(dir[j],1,flag);
		}
	}
	closedir(direc);
}

int main(int argc, char **argv)
{
	char dir[argc][PATH_MAX];
	int i,j,k;
	int flag[6];
	for(i=0;i<6;i++){flag[i]=0;}
	j=0;
	for(i=1;i<argc;i++){
		if(argv[i][0]=='-'){
			k=1;
			while(argv[i][k]!='\0'){
				if(argv[i][k]=='i'){flag[0]=1; }
				else if(argv[i][k]=='G'){flag[1]=1; }
				else if(argv[i][k]=='g'){flag[2]=1; }
				else if(argv[i][k]=='h'){flag[3]=1; }
				else if(argv[i][k]=='R'){flag[4]=1; }
				k++;
			}
		}
		else if(argv[i][0]=='/' || argv[i][0]=='~'){
			strcpy(dir[j],argv[i]);
			j++;
		}
	}
	if(j==0){
		if(getcwd(dir[0], sizeof(dir[0])) != NULL) {
			strcat(dir[0],"/");
			listFiles(dir[0],0,flag);
		}
		else {
			perror("getcwd() error");
			return 1;
		}
	}
	else{
		if(j>1){ flag[5]=1;}
		for(i=0;i<j;i++){
			listFiles(dir[i],0,flag);
		}
	}
}