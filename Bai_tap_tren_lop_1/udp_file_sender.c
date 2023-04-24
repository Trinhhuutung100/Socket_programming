#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char * argv[])
{
    int sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    char buf[256];
    FILE *f  = fopen(argv[3], "r");

    sendto(sender, argv[3], strlen(argv[3]), 0, (struct sockaddr *)&addr, sizeof(addr));

    while (!feof(f))
    {
        fgets(buf, sizeof(buf), f);
        int ret = sendto(sender, buf, strlen(buf), 0, (struct sockaddr *)&addr, sizeof(addr));
        printf("buf : %s\n", buf);
        printf("%d bytes sent.\n\n", ret);
    }

    char closefile[] ="Close";
    sendto(sender, closefile, strlen(closefile), 0, (struct sockaddr *)&addr, sizeof(addr));

    fclose(f);
}