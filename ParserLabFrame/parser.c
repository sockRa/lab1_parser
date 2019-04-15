/**********************************************************************/
/* lab 1 DVG C01 - Parser OBJECT                                      */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "keytoktab.h"               /* when the keytoktab is added   */
#include "lexer.h"                   /* when the lexer     is added   */
#include "symtab.h"                  /* when the symtab    is added   */
#include "optab.h"                   /* when the optab     is added   */

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define DEBUG 0
static int  lookahead=0;
static int  is_parse_ok=1;

/**********************************************************************/
/* RAPID PROTOTYPING - simulate the token stream & lexer (get_token)  */
/**********************************************************************/
/* define tokens + keywords NB: remove this when keytoktab.h is added */
/**********************************************************************/
// enum tvalues { program = 257, id, input, output, var, integer, begin,
//  define, digit, end };
/**********************************************************************/
/* Simulate the token stream for a given program                      */
/**********************************************************************/
// static int tokens[] = {program, id, '(', input, ',', output, ')', ';',
// var, id, ',', id, ',', id, ':', integer, ';', begin, id, assign, id, '+',
// id, '*', number, end, '.', '$' };

// /**********************************************************************/
// /*  Simulate the lexer -- get the next token from the buffer          */
// /**********************************************************************/
// static int pget_token()
// {  
//    // static int i=0;
//    // if (tokens[i] != '$') return tokens[i++]; else return '$';
//    static int i = 0, t = 0;
   
//    for(i = 0; t != '$'; i++)
//       printf("\n token is %4d, lexeme is %10s ", t = get_token(), get_lexeme());
// }

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/

/**********************************************************************/
/* The Parser functions                                               */
/**********************************************************************/
static void match(int t)
{
   if(DEBUG) printf("\n *** In  match expected %4s found %4s",
                   tok2lex(t), get_lexeme());
   if (lookahead == t) lookahead = get_token();
   else {
      is_parse_ok=0;
      printf("\n *** Unexpected Token: expected %4s found %4s (in match)",
              tok2lex(t), get_lexeme());
      }
   }

/**********************************************************************/
/* The grammar functions                                              */
/**********************************************************************/


/*Function protoypes*/
static void expr();
//////////////////////

static void program_header()
{
   if (DEBUG) printf("\n *** In  program_header");
   match(program);  addp_name(get_lexeme()); 
   match(id); match('('); match(input);
   match(','); match(output); match(')'); match(';');
   if (DEBUG) printf("\n *** Out  program_header\n");
}
   



/*##############  VAR_PART ###########*/
static void type(){
   if (DEBUG) printf("\n *** In  type");
   setv_type(lookahead);
   if(lookahead == integer)
      match(integer);
   else if(lookahead == real)
      match(real);
   else
      match(boolean);

   if (DEBUG) printf("\n *** Out  type");
}

static void id_list(){
   if (DEBUG) printf("\n *** In  id_list");
   addv_name(get_lexeme());
   match(id);
   if(lookahead == ','){
      match(',');
      id_list();
   }

   if (DEBUG) printf("\n *** Out  id_list");
}

static void var_deck(){
   if (DEBUG) printf("\n *** In  var_deck");
   id_list();
   match(':');
   type();
   match(';');

   if (DEBUG) printf("\n *** Out  var_deck");
}

static void var_dec_list(){
   if (DEBUG) printf("\n *** In  var_dec_list");
   var_deck();
   if(lookahead == id)
      var_dec_list();
   
   if (DEBUG) printf("\n *** Out  var_dec_list");
}

static void var_part(){
   if (DEBUG) printf("\n *** In  var_part");
   if(lookahead == var)
      match(var);
   var_dec_list();

   if (DEBUG) printf("\n *** out  var_part\n");
   
}

/*##############  STAT_PART ###########*/
static void operant(){
   if (DEBUG) printf("\n *** In  operant");

   if(lookahead == id)
      match(id);
   else
      match(number);
   
   if (DEBUG) printf("\n *** Out  operant");
}

static void factor(){
   if (DEBUG) printf("\n *** In  factor");

   if(lookahead == '('){
      match('(');
      expr();
      match(')');
   }
   else
      operant();

   if (DEBUG) printf("\n *** Out  factor");
}

static void term(){
   if (DEBUG) printf("\n *** In  term");
   factor();
   if(lookahead == '*'){
      match('*');
      term();
   }
   if (DEBUG) printf("\n *** Out  term");
}

static void expr(){
   if (DEBUG) printf("\n *** In  expr");
   term();
   if(lookahead == '+'){
      match('+');
      expr();
   }
   if (DEBUG) printf("\n *** Out  expr");
}

static void assign_stat(){
   if (DEBUG) printf("\n *** In  assign_stat");
   match(id);
   match(assign);
   expr();
   if (DEBUG) printf("\n *** Out  assign_stat\n");
}

static void stat(){
   if (DEBUG) printf("\n *** In  stat");
   assign_stat();
   if (DEBUG) printf("\n *** Out  stat");
}

static void stat_list(){
   if (DEBUG) printf("\n *** In  stat_list");
   stat();
   if(lookahead == ';'){
      match(';');
      stat_list();
   }
   if (DEBUG) printf("\n *** Out  stat_list");
}

static void stat_part(){
   if(DEBUG) printf("\n *** In  stat_part");
   match(begin);
   stat_list();
   match(end);
   match('.');
   if (DEBUG) printf("\n *** Out  stat_part\n");
}
   
/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/

int parser()
{
   if (DEBUG) printf("\n *** In  program");
   lookahead = get_token();        // get the first token
   program_header();               // call the first grammar rule
   var_part();                     // call the second grammar rule
   stat_part();                    // call the third grammar rule
   return is_parse_ok;             // status indicator
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/

