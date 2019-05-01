/* payday.c
 * nirosys at 0xcd80 dot org
 * * * * * * * * * * * * * *
 * This is a light sniffer, mainly
 * for those who already know what
 * it is they're looking for.
 *
 * Currently it prints out a Hex 
 * and Ascii dump of the payload
 * of the packet. Both UDP and 
 * TCP packets supported.
 *
 * The --log option will dump all
 * payloads into a file of the name,
 * <src ip>:<src port>-<dst ip>:<dst port>
 * * * * * * * * * * * * * 
 * TODO:
 *   * Cleanup the code a bit. It's messy..
 *   * Regex support?.. maybe.. yes..no?
 * * * * * * * * * * * * *
 * compile:
 *    cc -o payday payday.c -lpcap
 * * * * * * * * * * * * *
 * Useful filters
 *  Capture IRC traffic... (change port for whatever service..)
 *   ./payday --bpf="tcp and port 6667 and tcp[13] & 0x08 != 0"
 *
 *  Log all HTTP sessions..
 *   ./payday --bpf="tcp and port 80" --log --snap=1024
 */
#include <getopt.h>
#include <pcap.h>
#define __FAVOR_BSD
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

extern int errno;

#define IP_TO_DOT(buff,x) sprintf(buff,"%u.%u.%u.%u",x&0x000000ff,\
		(x&0x0000ff00)>>8,(x&0x00ff0000)>>16,(x&0xff000000)>>24);
				 
void usage( int, char ** );
void packet_dump( u_char *, const struct pcap_pkthdr *, const u_char * );
void printDump( const u_char *, const struct pcap_pkthdr *, const u_char * );
void loggerFunc( const u_char *, const struct pcap_pkthdr *, u_char * );

/* main 
 * * * * * * * * * * * * * * * * * * * * * */
int main( argc, argv )
    int argc;
    char **argv;
{
   int  opt;
   unsigned int snap = 128;
   char dev[11]="\0", 
        *bpf=NULL,*p,
        errbuff[PCAP_ERRBUF_SIZE]; 
   pcap_t	*pcap;
   void	(*printer)( u_char *, struct pcap_pkthdr *, u_char * ) = NULL;
   struct bpf_program bpf_prog;
   struct option long_opts[] = {
      { "help",0, NULL, 'h' },
      { "dev", 1, NULL, 'd' },
      { "bpf", 1, NULL, 'b' },
      { "snap",1, NULL, 's' },
      { "log", 0, NULL, 'l' }
   };

   while ( (opt = getopt_long(argc,argv,"h",(struct option *)&long_opts,NULL)) != -1) {
      switch (opt) {
         case 'h': usage(argc,argv);
         case 'd':
                   snprintf(dev,11,"%s",optarg);
                   break;
         case 'b': 
                   bpf = (char *)strdup((char *)optarg);
                   break;
         case 's':
                   snap = strtol(optarg,(char **)NULL,0);
                   if (errno == ERANGE) snap = 128;
                   break;
         case 'l':
                   printf("log not implemented yet..\n");
                   printer = loggerFunc;
                   break;
      }
   }
   if (!dev[0]) { /* Grab new device */
      p = pcap_lookupdev(errbuff);
      if (!p) {
         fprintf(stderr,"pcap_lookupdev: %s\n",errbuff);
         exit(0);
      }
      snprintf(dev,11,"%s",p);
   }
   printf("Opening on %s..\n",dev);
   if (!(pcap = pcap_open_live(dev,snap,1,0,errbuff)) ) {
      fprintf(stderr,"pcap_open_live: %s\n",errbuff);
      if (bpf) free(bpf);
      exit(0);
   }
   if (bpf) {
      printf("Using bpf: %s\n",bpf);
      if (pcap_compile(pcap,&bpf_prog,bpf,0,0) == -1) {
         fprintf(stderr,"invalid bpf.\n");
         free(bpf);
         exit(0);
      }
      pcap_setfilter(pcap,&bpf_prog);
      free(bpf);
   }
   pcap_loop(pcap,0,packet_dump,(u_char *)printer); /* File Output ? */
   pcap_close(pcap);
}

/* packet_dump
 * * * * * * * * * * * * * * * * * * * */
void packet_dump( user, pkthdr, pkt )
   u_char *user;
   const struct pcap_pkthdr *pkthdr;
   const u_char *pkt;
{
   char   buff[128], *data=NULL,*p,*q,*s;
   struct iphdr  *iph;
   struct tcphdr *tcph;
   struct udphdr *udph;
   void (*printer)( const u_char *, const struct pcap_pkthdr *, u_char *) = user;

   if (*((u_short *)(&pkt[12])) == 0x0008) {  /* IP Traffic? */ 
      iph = (struct iphdr *)((char *)pkt + 14);
      if (iph->protocol == 0x06) { 
         tcph = (struct tcphdr *)((char *)iph + (iph->ihl * 4));
         data = (char *)tcph + (tcph->th_off * 4);
      }
      else if (iph->protocol == 0x11) {
         udph = (struct udphdr *)((char *)iph + (iph->ihl * 4));
         data = (char *)udph + sizeof(struct udphdr);
      }
      else printf("unknown proto: 0x%.2x\n",iph->protocol);
      if (data) {
         if (!printer) {
            IP_TO_DOT(buff,iph->saddr);
            IP_TO_DOT(buff+25,iph->daddr);
            printf("------------------------------------------\n");
            printf("To: %s     From: %s\n",buff,buff+25);
            printDump(pkt,pkthdr,data);
         }
         else printer(pkt,pkthdr,data); 
      }
   }
}

/* printDump
 * * * * * * * * * * * * * * * * * * */
void printDump( pkt, pkthdr, data ) 
     const u_char *pkt;
     const struct pcap_pkthdr *pkthdr;
     const u_char *data;
{
   char 	buff[128],*p,*q,*s;

   q = buff; s = (char *)buff + 25;	
   for (p=(char *)data; p-(char *)pkt<pkthdr->caplen; p++) {
      sprintf(q,"%.2x ",(unsigned char)*p);
      sprintf(s,"%c ",( (*p >= 0x20) && (*p <= 0x7e)) ? *p:'.');
      q+=3; s+=2;
      if (((u_int)p+1-(u_int)data) % 8 == 0) {
         *q = ' '; 
         printf("%s\n",buff);
         q = buff;
         s = buff + 25;
      }
   } 
   if (q!=buff) {
      for (; q<buff+25; q++) *q = ' ';
      printf("%s\n",buff);
   }
   if ((u_char *)(pkt + pkthdr->caplen) == data) 
      printf("Empty Payload.\n");
}

/* loggerFunc
 * * * * * * * * * * * * * * * * * * * * * * * */
void loggerFunc( pkt, hdr, data ) 
     const u_char *pkt;
     const struct pcap_pkthdr *hdr;
     u_char *data;
{
   struct iphdr  *ip = (struct iphdr *)(pkt+14);
   struct tcphdr *tcp = (struct tcphdr *)((char *)ip+(ip->ihl * 4));
   u_int32_t     src=ip->saddr,dst=ip->daddr;
   FILE *out;
   char buff[32],*p,*q,*s;

   sprintf(buff,"%u.%u.%u.%u:%u-%u.%u.%u.%u:%u",
         src&0x000000FF,(src&0x0000FF00)>>8,(src&0x00FF0000)>>16,(src&0xFF000000)>>24,
         ntohs(tcp->th_sport),dst&0x000000ff,(dst&0x0000ff00)>>8,(dst&0x00FF00)>>16,
         (dst&0xFF000000)>>24,ntohs(tcp->th_dport));	
   if ( (out = fopen(buff,"a")) == NULL ) {
      printf("Unable to open file.. fix it!\n");
      return;
   }
   if ((u_char *)(pkt + hdr->caplen) >= data) {
      if ( (p = (char *)calloc(1,(hdr->caplen-(data-pkt))+1)) == NULL) {
         fprintf(stderr,"Unable to allocate tmp buffer.\n");
         return;
      }
      for (q=(char *)data,s=p; q-(char *)pkt<hdr->caplen; q++,s++)
         *s = *q;
      *s = 0x0;
      fprintf(out,"%s",p);
      free(p);
   }
   fclose(out);
}

/* usage * * * * * * * * * * * * 
 * * * * * * * * * * * * * * * */
void usage( argc, argv ) 
    int argc;
    char **argv;
{
   printf("%s [options]\n",argv[0]);
   printf("         --dev=[interface]\n"\
          "         --bpf=[filter string]\n"\
          "         --snap=[snap length]\n"\
          "         --log\n");
   exit(0);
}
