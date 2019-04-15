/**********************************************************************/
/* lab 1 DVG C01 - Driver OBJECT                                      */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "parser.h"
#include "symtab.h"

/**********************************************************************/
/* The main function (the driver)                                     */
/**********************************************************************/
int main()
{
   parser() ? printf(" \n PARSE SUCCESSFUL! \n")
            : printf(" \n PARSE FAILED! \n");

   printf("________________________________________________________");
   p_symtab();
   return 1;
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/

