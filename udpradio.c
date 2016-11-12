/*
 * File:    udpradio.c
 * Date:    Nov 10, 2016
 *
 * Group:   David Bernstein, Devang Doshi, Jake Mola, Shrey Hitendraprasad Upadhyay
 * Course:  Internet and Higher Level Protocols | Section: 656003 
 *
 * Desc:    The program is developed as part of the class project. 
 *          It is a basic implementation of udp packet radio. This program is written in
 *          C language and is compiled with a gcc, the GNU compiler collection
 *          (GCC) 4.4.7 20120313 (Red Hat 4.4.7-17). It is tested on school's AFS server
 *
 * Usage:   Provide a <hostname> <port> <delay in ms> <filename> input to the command line 
 *          while executing the program.   
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#define BUFSIZE 1024

void error(char *s)
{
  int errno;
  printf(" FATAL ERROR (%s) (errno=%d)\n", s);
  exit(errno);
}

int main(int argc, char **argv) {
    int sockfd, n, bindstatus;
    int serverlen, nr, ns;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    int fd, portno;
    useconds_t delay;
    char buf[BUFSIZE];
    struct addrinfo hints, *svrRes;
    memset(&hints, 0, sizeof hints);            
    hints.ai_family = INADDR_ANY ;              
    hints.ai_socktype = SOCK_DGRAM;
    
    /* check command line arguments */
    if (argc != 5) {
       fprintf(stderr,"usage: %s <hostname> <port> <filename> <delay in ms> \n", argv[0]);
       exit(0);
    }
    
    /* set hostname, portno etc from cmdline arguments */
    hostname = argv[1];
    portno = atoi(argv[2]);
    delay = atoi(argv[4]);
    delay *= 1000;

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(portno);
    // serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_addr.s_addr = inet_addr(hostname);

    printf("> Hostname: %s\n", hostname);

    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    
    /* build the server's Internet address (same as proxy) */
    /* use getaddrinfo or gethostbyname */
    

    /* if(getaddrinfo(hostname, portno, &hints, &svrRes) != 0){
        error(" ERROR Host conversion failed !\n");
    } */

    printf("> Server %x:%d (%d)\n",serveraddr.sin_addr.s_addr,serveraddr.sin_port,portno); 

    /* open file */
    if ((fd = open(argv[3],O_RDONLY)) < 0)
        error(" ERROR can't open file!\n");

    /* send the message to the server */
    for(;;) {
        /* read from file: read(fd,buffer,bytes) */
        nr = read(fd, buf, 1024);
        /* send UDP packet */
        // sendto(sockfd, buf, nr, 0, (struct sockaddr *) &svrRes, sizeof(&svrRes));
        if(nr <= 0) {
            break;
        } else {
            ns = sendto(sockfd, buf, nr, 0, (struct sockaddr *) &serveraddr, sizeof(serveraddr));

        }
        
        // printf("nr = %d\n", nr);
        // printf("ns = %d\n", ns);
        /* wait for <delay> msec */

        usleep(delay);       

        /* break when done reading file */
        
    }

    printf(" done\n");
    return 0;
}