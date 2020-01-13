#include"pch.h"
#include "functii.h"

int port=2024;
int login=0,ok=0,folder=0;
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
            cript(sConsolaUsername);
            lenght=strlen(sConsolaUsername);
            write(sd,&lenght,sizeof(int));
            write(sd,sConsolaUsername,lenght);
            read(sd,&login,sizeof(int));
            if(!login){
                printf("Username Incorect \n");
            }
        }
    login=0;
    while(!login)
    {
      printf("Parola: ");
      memset(sConsolaParola,0,sizeof(sConsolaParola));
      scanf("%s",sConsolaParola);
      cript(sConsolaParola);
      lenght=strlen(sConsolaParola);
      write(sd,&lenght,sizeof(int));
      write(sd,sConsolaParola,lenght);
      read(sd,&login,sizeof(int));
      if(!login)
          printf("Parola Incorecta \n");
    }
    printf("Logat!\n");
    while(scanf("%s",comanda)){
            lenght=strlen(comanda);
            write(sd,&lenght,sizeof(int));
            write(sd,comanda,lenght);
            if(strcmp(comanda,"find")==0 || strcmp(comanda,"mystat")==0){
                memset(path,0,sizeof(path));
                memset(result,0,sizeof(result));
                scanf(" %s",path);
                lenght=strlen(path);
                write(sd,&lenght,sizeof(int));
                write(sd,path,lenght);
                //afisarea resultatului
                read(sd,result,256);
                printf("%s\n",result);
            }
             else if(strcmp(comanda,"pwd")==0)
            {
              memset(result,0,sizeof(result));
              read(sd,result,256);
              printf("%s\n",result);
            }
            else if(strcmp(comanda,"quit")==0)
              break;
            else if(strcmp(comanda,"ls")==0)
            {
              memset(result,0,sizeof(result));
              read(sd,result,256);
              result[strlen(result)-1]=0;
              printf("%s\n",result);
            }
            else if(strcmp(comanda,"cd")==0)
            {
              memset(path,0,sizeof(path));
              memset(result,0,sizeof(result));
              scanf(" %s",path);
              lenght=strlen(path);
              write(sd,&lenght,sizeof(int));
              write(sd,path,lenght);
              read(sd,result,256);
              printf("%s\n",result);
            }
            else if(strcmp(comanda,"touch")==0)
            {
              memset(path,0,sizeof(path));
              memset(result,0,sizeof(result));
              scanf(" %s",path);
              lenght=strlen(path);
              write(sd,&lenght,sizeof(int));
              write(sd,path,lenght);
              read(sd,result,256);
              printf("%s\n",result);
            }
            else if(strcmp(comanda,";")==0)
                    continue;
            else if(strcmp(comanda,"&&")==0)
                    ;
            else if(strcmp(comanda,"||")==0)
                    ;
            else if(strcmp(comanda,"|")==0)
                    ;
            else if(strcmp(comanda,">")==0)
                    ;
            else if(strcmp(comanda,"<")==0)
                    ;
            else printf("Comanda Incorecta\n");
            memset(comanda,0,sizeof(comanda));
        }
  close (sd);
}