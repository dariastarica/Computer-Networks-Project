#include "pch.h"
#include "functii.h"
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
int login=0,ok=0;
int lenght;
char sConsolaUsername[256],comanda[256],sFisierUsername[256],sFisierParola[256],sConsolaParola[256];
char path[256],startPath[256];
char t[256];
char result[256];

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

    while(1)
    {
        read(ld.sockd,&lenght,sizeof(int));
        memset(comanda,0,sizeof(comanda));
        read(ld.sockd,comanda,lenght);
        if (strcmp(comanda,"quit")==0){
                    break;
                }
        if(strcmp(comanda,"find")==0){
            memset(path,0,sizeof(path));
            read(ld.sockd,&lenght,sizeof(int));
            read(ld.sockd,path,lenght);
            strcpy(startPath,"/home/daria");
            find(path,startPath,result);
            write(ld.sockd,result,256);
            }
        if(strcmp(comanda,"pwd")==0){
            pwd(result);
            write(ld.sockd,result,256);
        }
        if(strcmp(comanda,"ls")==0)
        {
            ls(result);
            write(ld.sockd,result,256);
        }
        if(strcmp(comanda,"cd")==0)
        {
            memset(path,0,sizeof(path));
            read(ld.sockd,&lenght,sizeof(int));
            read(ld.sockd,path,lenght);
            cd(path,result);
            write(ld.sockd,result,256);
        }
        if(strcmp(comanda,"touch")==0)
        {
            memset(path,0,sizeof(path));
            read(ld.sockd,&lenght,sizeof(int));
            read(ld.sockd,path,lenght);
            touch(path,result);
            write(ld.sockd,result,256);
        }
        if(strcmp(comanda,"rm")==0)
        {
            memset(path,0,sizeof(path));
            read(ld.sockd,&lenght,sizeof(int));
            read(ld.sockd,path,lenght);
            rm(path,result);
            write(ld.sockd,result,256);
        }
        if(strcmp(comanda,">")==0)
        {
            read(ld.sockd,&lenght,sizeof(int));
            read(ld.sockd,result,lenght);

            memset(path,0,sizeof(path));
            read(ld.sockd,&lenght,sizeof(int));
            read(ld.sockd,path,lenght);
            maimare(path,result);
            write(ld.sockd,result,256);
        }
    }
    close(ld.sockd);
    return nullptr;
}
