/* dumpmatch.h
 *  _                         _
 * | |_ ___ _ _ ___ ___ ___ _| |___ 
 * |  _|  _| | |   |  _| . | . | -_|
 * |_| |_| |___|_|_|___|___|___|___|
 *
 * nirosys (at) truncode (dot) org
 * 
 * This is an example of using libsynos
 * to perform OS passive OS detection on
 * a pre-existing traffic dump generated
 * by libpcap.
 */
 
 #include <stdio.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in_systm.h>
 #include <netinet/in.h>
 #include <arpa/inet.h>
 #include <pcap.h>
 #include <libsynos.h>
 #include <netinet/ip.h>
 
 /* A BPF to make sure we only examine SYN packets */
 #define SYN_BPF "tcp and tcp[13] == 2"
 
 void synmatch( unsigned char *, const struct pcap_pkthdr *, unsigned char * );
 void usage( void );
 
 /* main
  * * * * * * * * * * * * * * * * * * * * * * * * */
 int main( argc, argv )
     int argc;
	 char **argv;
 {
	 pcap_t		*pcap;
	 char 		errbuff[PCAP_ERRBUF_SIZE];
	 struct bpf_program bpf;
	 struct syn_definition *synhead;

	 if (argc < 3) usage();
	 /* Loading a definition file is as simple as this.. */
	 synhead = syndef_loaddefs(argv[1]);
	 if (synhead == NULL) {
		 fprintf(stderr,"Unable to load SYN Definitions.\n");
		 exit(0);
	 }
	 pcap = pcap_open_offline(argv[2],errbuff);
	 if (pcap == NULL) {
		 fprintf(stderr,"Error: %s",errbuff);
		 exit(0);
	 }
	 if (pcap_compile(pcap,&bpf,SYN_BPF,0,0) == -1) {
		 fprintf(stderr,"Unable to compile bpf.\n");
		 pcap_close(pcap);
		 exit(0);
	 }
	 if (pcap_setfilter(pcap,&bpf) == -1) {
		 fprintf(stderr,"Unable to set filter.\n");
		 pcap_close(pcap);
		 exit(0);
	 }
	 pcap_loop(pcap,0,synmatch,(char *)&synhead);
	 pcap_close(pcap);
	 /* Remember to clean up after we're done. */
	 syndef_unloaddefs(synhead);
 }
 
 /* usage
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 void usage( void ) {
	 printf("Usage: dumpmatch <definition file> <pcap filename>\n");
	 exit(0);
 }
 
 /* synmatch
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 void synmatch( user, hdr, pkt )
      unsigned char *user;
      const struct pcap_pkthdr *hdr;
      unsigned char *pkt;
 {
	 char	os[MAX_OS_LEN];
	 char   ascii[256];
	 struct syn_definition *newsyn;
	 struct syn_definition **synhead = (struct syn_definition **)user;
	 struct ip   *iph = (struct ip *)((char *)pkt+14);
	 
	 if (synos_match(pkt+14,*synhead,os)) 
		 printf("%s - %s\n",inet_ntoa(iph->ip_src),os);
	 else {
		 newsyn = (struct syn_definition *)calloc(sizeof(struct syn_definition),1);
		 synos_build_def(pkt+14,newsyn);
		 *synhead = syndef_insert(*synhead,newsyn);
		 syndef_asciidef(newsyn,ascii,256);
		 if (ascii[0]) 
			 printf("New Definition From %s:\n%s\n",inet_ntoa(iph->ip_src),ascii);
	 }
 }
