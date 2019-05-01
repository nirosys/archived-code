/* elf2shell.c 
 * nirosys@0xcd80.org
 * Reads the .text section of an elf binary, and converts it into
 * a C-Style char array.
 * * * * * * * * *
 * Idea came from the source zillion provided with his
 * buffer overflow tutorial.
 * * * * * * * * * * * * * * * *
 * TODO:
 *      o Add shellcode testing option.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <elf.h>

#if __x86_64__
#  define ELF_TYPE(x) Elf64_ ## x
#else
#  define ELF_TYPE(x) Elf32_ ## x
#endif

void generateComments( ELF_TYPE(Ehdr) *hdr );

/* main * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int main( int argc , char **argv ) {
   FILE * binary;
   int		off,soff,ssize,null=0;
   struct	stat sbuf;
   char	*buffer,*code;
   ELF_TYPE(Ehdr) *hdr;
   ELF_TYPE(Shdr) *shdr;

   if (argc <= 1) {
      printf("Usage: elf2shell <filename>\n");
      printf(" Where filename is the name of the elf binary\n"\
            " which contains your code.\n");
      exit(1);
   } 
   if ( (binary = fopen(argv[1],"r")) == NULL) {
      /* Unable To Open */
      printf("Unable To Open The File..\n");
      exit(0);
   }
   if (stat(argv[1],&sbuf)) {
      printf("Unable To Stat... Innnntereesstingg...\n");
      fclose(binary);
      exit(0);
   }
   if ( (buffer = malloc(sbuf.st_size)) == NULL) {
      printf("Unable To Allocate Memory... Hmpf.\n");
      fclose(binary);
      exit(0);
   }
   if (fread(buffer,1,sbuf.st_size,binary) != sbuf.st_size) {
      printf("Didn't get the whole file. Bailing\n");
      fclose(binary);
      free(buffer);
      exit(0);
   }
   fclose(binary);
   hdr = (ELF_TYPE(Ehdr) *)buffer;
   if ( ((hdr->e_ident)==ELFMAG) && (hdr->e_type != ET_EXEC)) {
      printf("This isn't an ELF binary..\n");
      free(buffer);
      exit(0);
   }
   for (off=0; off < hdr->e_shnum; off++) {
      shdr = (ELF_TYPE(Shdr) *)(buffer+hdr->e_shoff+(hdr->e_shentsize*off));
      if (shdr->sh_type == SHT_PROGBITS) break;
   }
   if (shdr->sh_type != SHT_PROGBITS) {
      printf(".text not found.. strange.\n");
      free(buffer);
      exit(0);
   }
   soff = shdr->sh_offset;
   ssize = shdr->sh_size;
   /* Strip trailing nops and nulls */
   while ( (buffer[soff+ssize-1] == (char)0x90) || 
         (buffer[soff+ssize-1] == (char)0x00) ) ssize--,null++;
   printf("/* %d bytes */\n",ssize);
   generateComments(hdr);
   printf("char shellcode[] = {\n\t\"");
   for (code=buffer+soff; code<buffer+soff+ssize; code++) {
      if ( (code-(buffer+soff)) % 15 == 0 && code != buffer+soff ) 
         printf("\"\n\t\"");
      if (*code == 0x00) null++; /* Check for inner-nulls */
      printf("\\x%.2X",(unsigned char)*code);
   } 
   printf("\"\n};\n");
   if (null > 0) 
      printf("/* Warning: There are/were null bytes in your shellcode. */\n");
   free(buffer);
}


void generateComments( ELF_TYPE(Ehdr) *hdr ) {
   printf("/* Architecture: ");
   switch (hdr->e_machine) {
      case EM_NONE:  /* No machine */
         printf("N/A"); break;
      case EM_M32:   /* AT&T WE 32100 */
         printf("AT&T WE 32100"); break;
      case EM_SPARC: /* SUN SPARC */
         printf("SUN SPARC"); break;
      case EM_386:   /* Intel 80386 */
         printf("Intel 80386"); break;
      case EM_68K:   /* Motorola m68k family */
         printf("Motorola m68k family"); break;
      case EM_88K:   /* Motorola m88k family */
         printf("Motorola m88k family"); break;
      case EM_860:   /* Intel 80860 */
         printf("Intel 80860"); break;
      case EM_MIPS:  /* MIPS R3000 big-endian */
         printf("MIPS R3000 (big-endian)"); break;
      case EM_S370:  /* IBM System/370 */
         printf("IBM System/370"); break;
      case EM_MIPS_RS3_LE: /* MIPS R3000 little-endian */
         printf("MIPS R3000 (little-endian"); break;
   }
   printf("\t\t*/\n");
}
