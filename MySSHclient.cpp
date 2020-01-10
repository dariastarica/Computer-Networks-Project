#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>


int port=2024;
int login=0;;
int lenght;
char sConsola[256];

int main (int argc, char *argv[])
{
  int sd;			
  struct sockaddr_in server;	
  char msg[256];		

  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_port = htons (port);
  
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }
  while (!login)
        {
            printf("login : ");
            memset(sConsola,0,sizeof(sConsola));
            scanf("%s",sConsola);
            lenght=strlen(sConsola);
            write(sd,&lenght,sizeof(int));
            write(sd,sConsola,lenght);
            read(sd,&login,sizeof(int));
            if(!login)
                printf("Username Incorect \n");
        }
        printf("Logat!\n");

  close (sd);
}