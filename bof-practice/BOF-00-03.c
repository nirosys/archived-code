/* BO-00-03.c
 * * * * * * * * * * * 
 * nirosys@0xcd80.org
 * * * * * * * * * * * 
 * Now make the saved-EIP your HITME.
 *
 *  Remember, during the execution of main
 *  the stack looks something like,
 * [..blah..][ EIP ][ EBP ][ main's local variables ]
 *
 * So, you'll be aiming, a little bit farther
 * than you would have if HITME was still there.
 * 
 * If you don't go far enough, go too far, or
 * mess the byte-ordering of your new EIP, you
 * will most definately crash the program. So
 * don't be surprised if you segfault a few
 * times.
 */
#include <stdio.h>


/* The function that you want to execute.. */
void target( void ) {
   printf("You Got It!!\n");
   exit(1);
}

int main( int argc, char **argv ) {
    char in[10];
    strcpy(in,argv[1]);
}
