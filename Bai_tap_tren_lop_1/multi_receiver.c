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
    int receiver = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

    bind(receiver, (struct sockaddr *)&addr, sizeof(addr));

    struct sockaddr_in sender_addr;
    int sender_addr_len = sizeof(sender_addr);
    char buf[256];
    int count = 0;

    FILE *f;
    
    while (1)
    {
        int ret = recvfrom(receiver, buf, sizeof(buf), 0, (struct sockaddr *)&sender_addr, &sender_addr_len);
        if (ret < sizeof(buf)) buf[ret] = 0;
        if(strncmp(buf, "gui moi", 7)==0) {
            count++;
            char file_name[256];
            int ret = recvfrom(receiver, file_name, sizeof(file_name), 0, (struct sockaddr *)&sender_addr, &sender_addr_len);
            file_name[ret] = 0;
            char new_name[256];
            sprintf(new_name, "new_%s_%d", file_name, count);
            printf("ten file duoc ghi du lieu : %s\n\n", new_name);
            f = fopen(new_name, "a");
            continue;
        }
        printf("%d bytes received %s: %s\n\n", ret, inet_ntoa(sender_addr.sin_addr), buf);
        fprintf(f, "%s", buf);
    }

    fclose(f);
}