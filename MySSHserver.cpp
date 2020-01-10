#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
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
int login=0;
int lenght;
char sConsola[256],sFisier[256];

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
        memset(sConsola,0, sizeof(sConsola));
        read(ld.sockd,sConsola,lenght);
            FILE* fin=fopen("login.txt","r");
            while(!feof(fin)){
                fgets(sFisier,300,fin);
                sFisier[strlen(sFisier)-1]=0;
                if(strcmp(sConsola,sFisier)==0){
                     login=1;
                     break;
                    }
            }
            fclose(fin);
            write(ld.sockd,&login,sizeof(int));
    }
    close(ld.sockd);
    return nullptr;
}