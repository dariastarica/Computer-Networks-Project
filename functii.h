#include "pch.h"

void find(char* path, char* currentPath, char result[]){
    char* adresa;
    char nextPath[300];
    DIR* currentDir;
    int folder=0;
    struct dirent* d;
    struct stat st;
    if((adresa = strstr(currentPath,path))!=NULL){
        if(*(adresa-1) == '/' && *(adresa+strlen(path))==0){
            //printf("%s\n",currentPath);
            strcpy(result,currentPath);
            return;
        }
        
    }
    stat(currentPath,&st);
    if((st.st_mode & S_IFMT) == S_IFDIR){
        folder=1;
    }
    else 
        return;
    currentDir=opendir(currentPath);
    if(currentDir == NULL)
        return;
    while(d=readdir(currentDir)){
        d->d_name;
        if(strcmp(d->d_name,".")==0 || strcmp(d->d_name,"..")==0)
            continue;
        memset(nextPath,0,300);
        strcpy(nextPath,currentPath);
        strcat(nextPath,"/");
        strcat(nextPath,d->d_name);
        find(path,nextPath,result);
    }
}

void pwd(char result[])
{
    char cwd[PATH_MAX];
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       strcpy(result,cwd);
   } else {
       perror("getcwd() error");
   }
}

void ls(char result[])
{
    struct dirent* d;
    char currentPath[PATH_MAX],path[256];
    DIR* currentDir;
    strcpy(path,getcwd(currentPath, sizeof(currentPath)));
    currentDir=opendir(path);
    while(d=readdir(currentDir)){
        d->d_name;
        if(strcmp(d->d_name,".")==0 || strcmp(d->d_name,"..")==0)
            continue;
        strcat(result,d->d_name);
        strcat(result,"\n");
        if(strcmp(d->d_name,".")==0 || strcmp(d->d_name,"..")==0)
            continue;
    }
    
}