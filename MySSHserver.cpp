#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <stdbool.h>
#include <time.h>
#define PORT 2024
#define MAXNR 1000

//////threads
static void* thread_main( void* arg);
struct thread_inf
{
	int sockd;
    int index;
};
pthread_t threads[MAXNR];

////////variabile
int login=0,ok=0,folder=0;
int lenght;
char sConsolaUsername[256],comanda[256],sFisierUsername[256],sFisierParola[256],sConsolaParola[256];
char path[256],startPath[256];
char t[256];

///////functii
void myfind(char* path, char* currentPath);
void mystat(char* path);
int main ()
{
    struct sockaddr_in server;	// structura folosita de server
    struct sockaddr_in from;
    int sd;			
    
    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
    	perror ("[server]Eroare la socket().\n");
    	return errno;
    }
    bzero (&server, sizeof (server));
    bzero (&from, sizeof (from));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons (PORT);

    int stat=1;
    setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&stat,4);

    if (bind (sd, (sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
    	perror ("[server]Eroare la bind().\n");
    	return errno;
    }

    /* punem serverul sa asculte daca vin clienti sa se conecteze */
    if (listen (sd, 5) == -1)
    {
    	perror ("[server]Eroare la listen().\n");
    	return errno;
    }
    thread_inf* data;
    int i=0;

	while(1)
	{
		int lg =sizeof(from);
		int client;
		if((client=accept(sd, (sockaddr*) &from, (socklen_t*) &lg ))==-1)
			continue;
		data= new thread_inf;
        data->sockd=client;
        data->index=i; 
        pthread_create(&threads[i++],NULL, thread_main, data);

	}
}				/* main */

void* thread_main( void* arg)
{
    char msg[256];
    thread_inf ld;
    ld=*((thread_inf*)arg);
    while(!login)
    {
        read(ld.sockd,&lenght,sizeof(int));
        memset(sConsolaUsername,0, sizeof(sConsolaUsername));
        read(ld.sockd,sConsolaUsername,lenght);
            FILE* fin=fopen("login.txt","r");
            while(!feof(fin)){
                fgets(sFisierUsername,300,fin);
                fgets(sFisierParola,300,fin);
                sFisierUsername[strlen(sFisierUsername)-1]=0;
                sFisierParola[strlen(sFisierParola)-1]=0;
                if(strcmp(sConsolaUsername,sFisierUsername)==0){
                     login=1;
                     break;
                    }
            }
            fclose(fin);
            write(ld.sockd,&login,sizeof(int));
    }
    login=0;
    while(!login)
    {
        read(ld.sockd,&lenght,sizeof(int));
        memset(sConsolaParola,0, sizeof(sConsolaParola));
        read(ld.sockd,sConsolaParola,lenght);
        if(strcmp(sConsolaParola,sFisierParola)==0)
                     login=1;
        
        write(ld.sockd,&login,sizeof(int));
    }

    while(!ok)
    {
        read(ld.sockd,&lenght,sizeof(int));
        memset(comanda,0,sizeof(comanda));
        read(ld.sockd,comanda,lenght);
        memset(path,0,sizeof(path));
        read(ld.sockd,&lenght,sizeof(int));
        read(ld.sockd,path,lenght);
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
            write(ld.sockd,&ok,sizeof(int));
    }
    close(ld.sockd);
    return nullptr;
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