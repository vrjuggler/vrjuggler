/* Feb 24, 2010
 * The current version of TrackingTools from NaturalPoint operates on port 1001, 
 * which falls in the restricted port range on Linux systems.  To avoid having to run
 * applications as root, an application like this one may be run as root
 * to redirect traffic to a higher port number (10001 in this example).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXBUFSIZE 65536 /* Max UDP Packet size is 64 Kbyte */
#define MCAST_RECV_ADDR "224.0.0.1"
#define MCAST_SEND_ADDR "224.0.0.1"
#define RECV_PORT 1001
#define SEND_PORT 10001

int main()
{
   int ListenSocket, SendSocket,status;
   struct in_addr MyAddress, MultiCastAddress;
   int one = 1;
   int ttl = 3;
   int socklen = sizeof(struct sockaddr_in);
   int addr_len = sizeof(struct sockaddr);

   /* Listen socket */
   
   /* Create socket */
   ListenSocket = socket(PF_INET, SOCK_DGRAM, 0);
   if (ListenSocket < 0)
   {
      perror("Error creating listen socket"), exit(0);
   }

   /* Create socket address to bind to */
   struct sockaddr_in MySocketAddr;
   memset(&MySocketAddr, 0, sizeof(MySocketAddr));
   MySocketAddr.sin_family = PF_INET;
   MySocketAddr.sin_port = htons(RECV_PORT);
   MySocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);

   /* Bind socket */
   status = bind(ListenSocket, (struct sockaddr *)&MySocketAddr, sizeof(struct sockaddr));
   if (status < 0)
   {
      perror("Error binding listen socket to interface"), exit(0);
   }

   /* Join multicast group */
   struct ip_mreq Mreq;
   Mreq.imr_multiaddr.s_addr = inet_addr(MCAST_RECV_ADDR);
   Mreq.imr_interface.s_addr = INADDR_ANY;
   setsockopt(ListenSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &Mreq, sizeof(Mreq));

   /* Send socket */
   struct sockaddr_in TheirAddress;
   struct sockaddr_in saddr;
   struct in_addr iaddr;
   memset(&saddr, 0, sizeof(struct sockaddr_in));
   memset(&iaddr, 0, sizeof(struct in_addr));

   SendSocket = socket(PF_INET, SOCK_DGRAM, 0);
   if (ListenSocket < 0)
   {
    perror("Error creating send socket"), exit(0);
   }

   saddr.sin_family = PF_INET;
   saddr.sin_port = htons(0); /* Use the first free port */
   saddr.sin_addr.s_addr = htonl(INADDR_ANY); /* bind socket to any interface */
   status = bind(SendSocket, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
   if ( status < 0 )
   {
      perror("Error binding send socket to interface"), exit(0);
   }

   iaddr.s_addr = INADDR_ANY; /* use DEFAULT interface */

   /* Set the outgoing interface to DEFAULT */
   setsockopt(SendSocket, IPPROTO_IP, IP_MULTICAST_IF, &iaddr,
              sizeof(struct in_addr));

   /* Set multicast packet TTL to 3; default TTL is 1 */
   setsockopt(SendSocket, IPPROTO_IP, IP_MULTICAST_TTL, &ttl,
              sizeof(unsigned char));

   /* send multicast traffic to myself too */
   status = setsockopt(SendSocket, IPPROTO_IP, IP_MULTICAST_LOOP,
                       &one, sizeof(unsigned char));

   /* set destination multicast address */
   saddr.sin_family = PF_INET;
   saddr.sin_addr.s_addr = inet_addr(MCAST_SEND_ADDR);
   saddr.sin_port = htons(SEND_PORT);

   while(1)
   {
      unsigned char buffer[MAXBUFSIZE];
      int nDataBytesReceived = recvfrom(ListenSocket,
                                        buffer,
                                        sizeof(buffer),
                                        0,
                                        (struct sockaddr *)&TheirAddress,
                                        &addr_len);

      status = sendto(SendSocket, buffer, nDataBytesReceived, 0,
                      (struct sockaddr *)&saddr,
                      socklen);

   }
}

