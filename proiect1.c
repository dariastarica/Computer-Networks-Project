#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <stdbool.h>
#include <time.h>


int fifo1[2],fifo2[2],sockt[2],p[2];
int login=0,lenght,ok=0,folder=0;
char startPath[300];
char sFisier[300],sConsola[300],comanda[300],path[300];
char t[300];
pid_t pid;
/////////// functii
void myfind(char* path, char* currentPath);
void mystat(char* path);
///////////

int main(int argc, char* argv[])
{
    ///////protocoalele de comunicatii
    mkfifo("fifo1",666);
    mkfifo("fifo2",666);
    socketpair(AF_UNIX,SOCK_STREAM,0,sockt);
    pipe(p);
    ///////
    pid=fork();
    if(pid>0){
        //parinte
        fifo1[1]=open("fifo1",O_WRONLY);
        fifo2[0]=open("fifo2",O_RDONLY);
        while (!login)
        {
            printf("login : ");
            memset(sConsola,0,sizeof(sConsola));
            scanf("%s",sConsola);
            lenght=strlen(sConsola);
            write(fifo1[1],&lenght,sizeof(int));
            write(fifo1[1],sConsola,lenght);
            read(fifo2[0],&login,sizeof(int));
            if(!login)
                printf("Username Incorect \n");
        }
        printf("Logat!\n");
        close(sockt[1]);
        close(p[1]);
        while(ok==0){
            memset(comanda,0,sizeof(comanda));
            scanf("%s",comanda);
            lenght=strlen(comanda);
            write(sockt[0],&lenght,sizeof(int));
            write(sockt[0],comanda,lenght);
            if(strcmp(comanda,"myfind")==0 || strcmp(comanda,"mystat")==0){
                memset(path,0,sizeof(path));
                scanf(" %s",path);
                lenght=strlen(path);
                write(sockt[0],&lenght,sizeof(int));
                write(sockt[0],path,lenght);
            }
            read(p[0],&ok,sizeof(int));
        }
        
    }
    else if(pid==0){
        //copil
        fifo1[0]=open("fifo1",O_RDONLY);
        fifo2[1]=open("fifo2",O_WRONLY);
        while(!login){
            read(fifo1[0],&lenght,sizeof(int));
            memset(sConsola,0,sizeof(sConsola));
            read(fifo1[0],sConsola,lenght);
            FILE* fin=fopen("login.txt","r");
            while(!feof(fin)){
                fgets(sFisier,300,fin);
                sFisier[strlen(sFisier)-1]=0;
                if(strcmp(sConsola,sFisier)==0){
                     login=1;
                     break;
                    }
            }
            write(fifo2[1],&login,sizeof(int));
        }
        close(sockt[0]);
        close(p[0]);
        while(ok==0){
            read(sockt[1],&lenght,sizeof(int));
            memset(comanda,0,sizeof(comanda));
            read(sockt[1],comanda,lenght);
            if(strcmp(comanda,"myfind")==0 || strcmp(comanda,"mystat")==0){
                memset(path,0,sizeof(path));
                read(sockt[1],&lenght,sizeof(int));
                read(sockt[1],path,lenght);

            }
            /*comanda[strlen("mystat")]=0;
            char* parametru = (char*)malloc(300);
            strcpy(parametru, comanda+7);*/
            //printf("%s %s\n",comanda,path);
            if (strcmp(comanda,"quit")==0){
                    ok=1;
                }
            if(strcmp(comanda,"myfind")==0){
                strcpy(startPath,"/home/daria");
                myfind(path,startPath);
            }
            if(strcmp(comanda,"mystat")==0){
                mystat(path);
            }
            write(p[1],&ok,sizeof(int));
            }
                
        }
    else{
        perror("Eroare la fork");
        exit(1);
    }
        return 0;
}

void myfind(char* path, char* currentPath){
    char* adresa;
    char nextPath[300];
    DIR* currentDir;
    struct dirent* d;
    struct stat st;
    if((adresa = strstr(currentPath,path))!=NULL){
        if(*(adresa-1) == '/' && *(adresa+strlen(path))==0){
            printf("%s\n",currentPath);
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
        myfind(path,nextPath);
    }
}

void mystat(char* path){
    struct stat st;
    char perm[10]="---------"; 
    if(stat(path,&st)!=0){
        return; 
    }
    printf("file type: ");
    long time;
    struct tm timestamp;
    switch(st.st_mode & S_IFMT)
    {
        case S_IFDIR : printf("Director\n"); break;
        case S_IFREG : printf("Fisier obisnuit\n"); break;
        case S_IFLNK : printf("Link\n"); break;
        case S_IFIFO : printf("FIFO\n"); break;
        case S_IFSOCK: printf("Socket\n"); break;
        default: printf("Unknown file type");
    }
    printf("file size: %lld\n",(long long)st.st_size);

    if(S_IRUSR & st.st_mode) perm[0]='r';
    if(S_IWUSR & st.st_mode) perm[1]='w';
    if(S_IXUSR & st.st_mode) perm[2]='x';
    if(S_IRGRP & st.st_mode) perm[3]='r';
    if(S_IWGRP & st.st_mode) perm[4]='w';
    if(S_IXGRP & st.st_mode) perm[5]='x';
    if(S_IROTH & st.st_mode) perm[6]='r';
    if(S_IWOTH & st.st_mode) perm[7]='w';
    if(S_IXOTH & st.st_mode) perm[8]='x';
    printf("permissions: %s\n",perm);

    time=st.st_atime;
    localtime_r(&time,&timestamp);
    strftime(t,300, "%c", &timestamp);
    printf("%s\n",t);
    fflush(stdout);
}