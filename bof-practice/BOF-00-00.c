/* change.c
 * * * * * * * * * * * *
 * nirosys@0xcd80.org
 * * * * * * * * * * * * 
 * This program demonstrates
 * data corruption from a buffer
 * overflow.
 *
 * Notice that HITME sits just above
 * in on the stack. By overflowing in
 * you can manipulate the value of HITME. 
 *
 * Use that to set the value of HITME to
 * 0xdeadbeef.
 *
 * This is a very important idea. 
 * Once your goal becomes executing shellcode,
 * HITME is no longer the variable you're 
 * aiming for, but the register EIP which
 * is stored on the stack just after the
 * the local variables for the main function
 * (In this case).
 * * * * * * * * * * * * */

int main( int argc, char **argv ) {
    char buffer[1024]; /* Padding, to make things nicer */
    int  HITME = 0x00000000;
    char in[10]; 
    
    printf("HITME = 0x%.8x\n",HITME);
    strcpy(in,argv[1]);
    printf("HITME = 0x%.8x\n",HITME);
    if (HITME != 0xdeadbeef) {
      printf("Sorry. Try Again.\n");
      exit(1);
    }
    printf("You got it!\n");
} 
