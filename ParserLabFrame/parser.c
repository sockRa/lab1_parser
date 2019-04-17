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
static int  lookahead = 0;
static int  is_parse_ok = 1;
static int  assignFlag = 0;
static int  compareFlag = 0;
toktyp op,arg1,arg2,returnType,compareType;




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
      }
   }

/**********************************************************************/
/* The grammar functions                                              */
/**********************************************************************/


/*Function protoypes*/
static void expr();


static void syntaxErrorExpected(char *arg1){

   printf("\nSYNTAX:   Symbol expected %s found %s ",arg1,get_lexeme());
}

static void printRemainder(){
   printf("\nSYNTAX:   Extra symbols after end of parse! \n ");
   printf("         ");
   while(lookahead != '$'){
      printf("%s ",get_lexeme());
      lookahead = get_token();
   }
}

static void program_header()
{
   if (DEBUG) printf("\n *** In  program_header");

   if(lookahead != program)
      syntaxErrorExpected("program");
   match(program);

   if(lookahead != id){
      printf("\nSYNTAX:   ID expected found  %s ",get_lexeme());
      addp_name("???");
   }else
      addp_name(get_lexeme());

   match(id);
   
   if(lookahead != '(')
      syntaxErrorExpected("(");
   match('(');

   if(lookahead != input)
      syntaxErrorExpected("input"); 
   match(input);

   if(lookahead != ',')
      syntaxErrorExpected(",");
   match(',');

   if(lookahead != output)
      syntaxErrorExpected("output");
   match(output); 

   if(lookahead != ')')
      syntaxErrorExpected(")");
   match(')');

   if(lookahead != ';')
      syntaxErrorExpected(";"); 
   match(';');
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
   else if(lookahead == boolean)
      match(boolean);
   else
      printf("\nSYNTAX:   Type name expected found %s ",get_lexeme());

   if (DEBUG) printf("\n *** Out  type");
}

static void id_list(){
   if (DEBUG) printf("\n *** In  id_list");

      
   if(lookahead != id)
      printf("\nSYNTAX    ID expected found %s",get_lexeme());
   else{
         if(find_name(get_lexeme()))
            printf("\nSEMANTIC: ID already declared: %s", get_lexeme());
         else
            addv_name(get_lexeme());
      match(id);
   }
      

   if(lookahead == ','){
      match(',');
      id_list();
   }

   if (DEBUG) printf("\n *** Out  id_list");
}

static void var_deck(){
   if (DEBUG) printf("\n *** In  var_deck");
   id_list();

   if(lookahead == ':')
      match(':');
   else
      syntaxErrorExpected(":");

   type();

   if(lookahead == ';')
      match(';');
   else
      syntaxErrorExpected(";");

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
   else
      syntaxErrorExpected("var");
   var_dec_list();

   if (DEBUG) printf("\n *** out  var_part\n");
   
}

/*##############  STAT_PART ###########*/
static void operant(){
   if (DEBUG) printf("\n *** In  operant");

   if(lookahead == id){
      if(!find_name(get_lexeme()))
         printf("\nSEMANTIC: ID NOT declared: %s",get_lexeme());

      if(arg1 != 0)
         arg2 = get_ntype(get_lexeme());
      else
         arg1 = get_ntype(get_lexeme());

      match(id);
      
   }
   else if(lookahead == number)
      match(number);
   else{
      printf("\nSYNTAX:   Operand Expected ");
      returnType = undef;
   }
   
   if(!arg1)
      arg1 = integer;
   if(!arg2)
      arg2 = integer;

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
      //op = lookahead;
      match('*');
      term();
   }
   if (DEBUG) printf("\n *** Out  term");
}

static void expr(){
   if (DEBUG) printf("\n *** In  expr");
   term();
   if(lookahead == '+'){
      op = lex2tok(get_lexeme());
      match('+');
      expr();
   }
   if(!op)
      op = '+';
   
   if (DEBUG) printf("\n *** Out  expr");
}

static void assign_stat(){
   if (DEBUG) printf("\n *** In  assign_stat");

   if(!compareType && find_name(get_lexeme()))
      compareType = get_ntype(get_lexeme());

   if(compareType == predef || compareType == 0)
      compareType = error;
   
   if(isalnum(*get_lexeme()) && key2tok(get_lexeme()) == id && isalpha(*get_lexeme())){
      if(!find_name(get_lexeme()))
      printf("\nSEMANTIC: ID NOT declared: %s",get_lexeme());
   }
   else
      printf("\nSYNTAX:   ID expected found %s",get_lexeme());
   
   match(id);
   if(strcmp(get_lexeme(),":=") != 0){
      syntaxErrorExpected(":=");

      if(isalnum(*get_lexeme()) == 0)
         assignFlag = 1;
   }
   match(assign);
   expr();



   if(assignFlag)
      returnType = error;
   else if(returnType != undef)
      returnType = get_otype(op,arg1,arg2);

   if(key2tok(get_lexeme()) != id && compareFlag)
      compareType = returnType; 

   if(compareType != returnType)
      printf("\nSEMANTIC: Assign types: %s := %s",tok2lex(compareType),tok2lex(returnType));
   
   compareFlag = 1;

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
   
   if(lookahead != begin)
      syntaxErrorExpected("begin");
   match(begin);

   stat_list();

   if(lookahead != end)
      syntaxErrorExpected("end");
   match(end);

   if(lookahead != '.')
      syntaxErrorExpected(".");
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

   if(strcmp(get_lexeme(),"$") == 0){
      printf("\nSYNTAX:   Input file is empty ");
      return is_parse_ok;
   }

   program_header();               // call the first grammar rule
   var_part();                     // call the second grammar rule
   stat_part();                    // call the third grammar rule
   if(lookahead != '$')            // Print all unmatched symbols in program
      printRemainder();
   return is_parse_ok;             // status indicator
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/

