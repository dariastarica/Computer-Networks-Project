#include "pch.h"
#include <string.h>

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
    }
}

void cd (char* path,char result[])
{
    strcpy(result,"Directorul curent a fost schimbat in ");
    strcat(result,path);
    chdir(path);
}

void cript(char text[])
{ 
    int lg;
    lg=strlen(text);
    for (int i=0;i<lg;i++) 
    { 
        if (text[i]<='Z' && text[i]>='A') 
            text[i]=(char)(((int)text[i]-65+4)%26+65);
        else if(text[i]<='z' && text[i]>='a')
                text[i]=(char)(((int)text[i]-97+4)%26 +97);
    } 
}

 void decript(char text[])
{
    int lg;
    lg=strlen(text);
    for (int i=0;i<lg;i++) 
    { 
        if (text[i]<='Z' && text[i]>='A') 
            text[i]=(char)(((int)text[i]-65-4)%26+65);
        else if(text[i]<='z' && text[i]>='a')
                text[i]=(char)(((int)text[i]-97-4)%26 +97);
    } 
}

void touch(char nume[],char result[])
{
    FILE* fileptr;
    char numefis[256];
    pwd(numefis);
    fileptr=fopen("numefis","w");
    if(!fileptr)
        strcpy(result,"Eroare la crearea fisierului!");
    strcpy(result,"Fisier creat!");
}