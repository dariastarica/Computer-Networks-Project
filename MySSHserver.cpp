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

static void* thread_main( void* arg);
struct thread_inf
{
	int sockd;
    int index;
};

pthread_t threads[MAXNR];

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
    memset(msg,0, 256);
    read(ld.sockd,msg,256);
    printf("Threadul %d a afisat %s\n", ld.index, msg);
    fflush(stdout);
    close(ld.sockd);
    return nullptr;
}