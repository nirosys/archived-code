/* change3.c
 * * * * * * * * *
 * nirosys@0xcd80.org
 * * * * * * * * * * * * * * * * * * * * *
 * This could be an actual usage in the
 * real world.
 * 
 * Pretty much the same as change2.c, only
 * now you can see where something like this
 * could be used. :) (No more HITMEs, now 
 * we play with real code)
 */
#include <stdio.h>


/* The function that normally executes.. */
void foo( void ) {
   printf("Nope.. Sorry, Try Again.\n");
}

/* The function that you want to execute.. */
void target( void ) {
   printf("You Got It!!\n");
}

int main( int argc, char **argv ) {
    char buffer[1024]; /* IGNORE ME */
    void (*HITME)() = foo;
    char in[10];

    printf("HITME = 0x%.8x\n",HITME);
    strcpy(in,argv[1]);
    printf("HITME = 0x%.8x\n",HITME);
    HITME();
}
