/**********************************************************************/
/* lab 1 DVG C01 - Lexer OBJECT                                       */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "keytoktab.h"

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define BUFSIZE 1024
#define LEXSIZE   30
static char buffer[BUFSIZE];
static char lexbuf[LEXSIZE];
static int  pbuf  = 0;               /* current index program buffer  */
static int  plex  = 0;               /* current index lexeme  buffer  */
static int  flag  = 0;               /* if get_prog has been called */

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
/**********************************************************************/
/* buffer functions                                                   */
/**********************************************************************/
/**********************************************************************/
/* Read the input file into the buffer                                */
/**********************************************************************/

static void get_prog()
{

   //ANVÄND BARA DENNA VID DEBUG
   FILE *fp = fopen("TestSuite/testm.pas", "r");
   ///////////////////////////////////////

   // FILE *fp = stdin;

   if(fp == NULL){
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }

   size_t newLen = fread(buffer, sizeof(char), BUFSIZE, fp);
   if(ferror(fp) != 0)
      fputs("Error while reading the file",stderr);
   else{
      buffer[newLen++] = '$';
      buffer[newLen++] = '\0';
   }
   
   fclose(fp);
}     

/**********************************************************************/
/* Display the buffer                                                 */
/**********************************************************************/  

static void pbuffer()
{   
   printf("\n________________________________________________________");
   printf("\n THE PROGRAM TEXT");
   printf("\n________________________________________________________\n");
   printf("%s\n",buffer);
   printf("________________________________________________________");
}

/**********************************************************************/
/* Copy a character from the program buffer to the lexeme buffer      */
/**********************************************************************/

static void get_char()
{  

   
   lexbuf[plex] = buffer[pbuf];
   plex++;
   pbuf++;
}

/**********************************************************************/
/* End of buffer handling functions                                   */
/**********************************************************************/

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Return a token                                                     */
/**********************************************************************/
int get_token()
{  
   int token;
   memset(lexbuf, '\0', LEXSIZE);
   plex = 0;
   if(!flag){
      get_prog();
      pbuffer();
      flag = 1;
   }
   
         //Jump over space
      while(buffer[pbuf] == ' ' || buffer[pbuf] == '\n' || buffer[pbuf] == '\r')
         pbuf++;

      //Om siffra, retunera number
      if(isdigit(buffer[pbuf])){
         get_char();
         return number;
      }
      //Om alphanum, kolla om det tillhör keyword eller inte
      else if(isalnum(buffer[pbuf])){
            while(isalnum(buffer[pbuf]))  //Hämtar in hela ordet
               get_char();
            
            if(buffer[pbuf + 1] == ':' || buffer[pbuf + 1] == '+' || buffer[pbuf + 1] == '*')
               return id;
            
            else if(strcmp(lexbuf,"input") == 0 || strcmp(lexbuf,"output") == 0)
               return key2tok(lexbuf);

            else if((buffer[pbuf] == '(' || buffer[pbuf] == ',' || buffer[pbuf] == ':' || buffer[pbuf] == ')'))
               return id;
            else
               return key2tok(lexbuf);
            

      }else{
         get_char();
         if(lexbuf[plex - 1] == ':' && buffer[pbuf] == '='){
            get_char();
            return assign;
         }
      }

      token = lex2tok(lexbuf);
   
   return token; 

}

/**********************************************************************/
/* Return a lexeme                                                    */
/**********************************************************************/
char * get_lexeme()
{  
   return lexbuf;
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/

