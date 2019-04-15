#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFSIZE 1024
#define LEXSIZE 30

int main(){
    static char buff[BUFSIZE];
    static char lexbuf[LEXSIZE];
    static int pbuf = 0;
    static int plex = 0;
    // char filename[25];
    // scanf("%s",filename);
    char ch;
    // printf("Program name: %s",filename);
    FILE *fp = fopen("testok1.pas","r");

    // fp = fopen("testok1.pas","r");

    if(fp == NULL){
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    size_t newLen = fread(buff, sizeof(char),BUFSIZE,fp);
    if(ferror(fp) != 0)
        fputs("Error reading file",stderr);
    else
        buff[newLen++] = '\0';

    printf("%s\n",buff);
    fclose(fp);

    //GET CHAR

    while(buff[pbuf] != ' '){
        lexbuf[plex] = buff[pbuf];
        plex++;
        pbuf++;
    }

    //Reset lexbuf
    memset(lexbuf,'\0',LEXSIZE);

    printf("Hej");
    
        
    
}