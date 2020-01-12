#include "pch.h"

void cript(char text[])
{ 
    int lg;
    lg=strlen(text);
    // traverse text 
    for (int i=0;i<lg;i++) 
    { 
        // apply transformation to each character 
        // Encrypt Uppercase letters 
        if (text[i]<='Z' && text[i]>='A') 
            text[i]=(char)(((int)text[i]-65+4)%26+65);
  
    // Encrypt Lowercase letters 
    else if(text[i]<='z' && text[i]>='a')
          text[i]=(char)(((int)text[i]-97+4)%26 +97);
    } 
}

char text[256],cripttext[256];
int main()
{
    scanf("%s",text);
    cript(text);
    printf("%s\n",text);
    //fflush(stdout);
}
