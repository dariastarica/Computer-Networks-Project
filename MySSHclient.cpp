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
int login=1,ok=0,folder=0;
int lenght;
char sConsolaUsername[256],sConsolaParola[256];
char comanda[256],path[256],result[256];

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
            memset(sConsolaUsername,0,sizeof(sConsolaUsername));
            scanf("%s",sConsolaUsername);
            if(strcmp(sConsolaUsername,"ssh-d")){
                printf("ssh-d pls\n");
                continue;
            }
            memset(sConsolaUsername,0,sizeof(sConsolaUsername));
            scanf("%s",sConsolaUsername);
            lenght=strlen(sConsolaUsername);
            write(sd,&lenght,sizeof(int));
            write(sd,sConsolaUsername,lenght);
            read(sd,&login,sizeof(int));
            if(!login){
                printf("Username Incorect \n");
            }
        }
    login=1;
    while(!login)
    {
      printf("Parola: ");
      memset(sConsolaParola,0,sizeof(sConsolaParola));
      scanf("%s",sConsolaParola);
      lenght=strlen(sConsolaParola);
      write(sd,&lenght,sizeof(int));
      write(sd,sConsolaParola,lenght);
      read(sd,&login,sizeof(int));
      if(!login)
          printf("Parola Incorecta \n");
    }
    printf("Logat!\n");
    while(1){
            memset(comanda,0,sizeof(comanda));
            scanf("%s",comanda);
            lenght=strlen(comanda);
            write(sd,&lenght,sizeof(int));
            write(sd,comanda,lenght);
            if(strcmp(comanda,"find")==0 || strcmp(comanda,"mystat")==0){
                memset(path,0,sizeof(path));
                scanf(" %s",path);
                lenght=strlen(path);
                write(sd,&lenght,sizeof(int));
                write(sd,path,lenght);
            }
            read(sd,result,256);
            printf("%s\n",result);
            //fflush(stdout);
            break;
        }
  close (sd);
}