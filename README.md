# Archived Code

> NOTE: This code is old, and was recovered from web.archive.org from a site
> that was run in ~2003 when I was a much younger person.. I've put it here for
> historical purposes.

## What's here?

There are a few utilities here, mostly with a security spin.
As well as some shellcode, and some buffer overflow examples.

  * **elf2shell.c** - A simple program that will look at an ELF binary, extract
      the .text section, and convert it into a C array for inclusion in exploit
      code.
  * **fence.c** - A trivial use of RPC to spoof messages using the rwall service.
      It shows by example how untrustworthy some of our early protocols were. ;)
  * **dumpmatch.c** - A basic program using a library I wrote for passive
      SYN packet fingerprinting, to detect operating systems in previously
      captured pcaps. I do not have the library anymore however. :(
  * **shellcode/** - A collection of shellcode I wrote, usually both the assembly (.s)
     and the C array (.c).
  * **bof-practice/** - A collection of simple programs vulnerable to buffer
     overflows, used during wargames/practice.
