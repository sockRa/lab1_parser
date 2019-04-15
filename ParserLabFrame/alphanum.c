#include <stdio.h>
#include <ctype.h>

int main(){

    int var = '=';
    if(isalnum(var))
        printf("Alphanum");
    else
        printf("Not alphanum");
}