#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int main()
{
   int server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

   struct sockaddr_in addr;
   addr.sin_family = AF_INET;
   addr.sin_addr.s_addr = htonl(INADDR_ANY);
   addr.sin_port = htons(9000);

   bind(server, (struct sockaddr *)&addr, sizeof(addr));

   listen(server, 5);

   struct sockaddr_in clientAddr;
   int clientAddrLen = sizeof(clientAddr);

   int client = accept(server, (struct sockaddr *) &clientAddr, &clientAddrLen);

   char buf[256];
   int ret;

   ret =  read(client, buf, sizeof(buf));
   buf[ret] = 0;
   int pos = 0;
   char may[64];
   strcpy(may, buf);
   pos +=strlen(may) + 1;
   printf("Ten may : %s\n", may);
   int num  = (ret - pos)/3;
   char o;
   short int kt;
   for(int i = 0; i<num ; i++) {
       o = buf[pos];
       pos++;
       memcpy(&kt, buf + pos, sizeof(kt));
       pos+=sizeof(kt);
       printf("o %c : %hd GB\n", o, kt);
   }

   close(client);
   close(server);

}
