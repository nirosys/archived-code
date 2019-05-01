/* change2.c
 * * * * * * * * * * * *
 * nirosys@0xcd80.org
 * * * * * * * * * * * * 
 * This program demonstrates
 * data corruption from a buffer
 * overflow.
 *
 * The object is to overwrite
 * HITME with the address of
 * target.
 * 
 * This will require some
 * debugger work. Preferebly
 * gdb ;)
 * * * * * * * * * * * * */

void target( void ) {
   printf("In Target.\n");
}

int main( int argc, char **argv ) {
    char buffer[1024]; /* IGNORE ME */
    int  HITME = 0x00000000;
    char in[10]; 
    
    printf("HITME = 0x%.8x\n",HITME);
    strcpy(in,argv[1]);
    printf("HITME = 0x%.8x\n",HITME);
    if (HITME != target) {
      printf("Sorry. Try Again.\n");
      exit(1);
    }
    printf("You got it!\n");
} 
