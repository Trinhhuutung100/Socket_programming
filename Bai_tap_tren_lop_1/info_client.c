#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void clean_stdin()
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

int main()
{
    int client = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP); 

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port  = htons(9000);

    connect(client, (struct sockaddr *)&addr, sizeof(addr));

    char may[64];
    printf("Nhap ten may : ");
    scanf("%s", may);
    clean_stdin();

    char buf[256];
    strcpy(buf,may);
    int pos = strlen(may);
    buf[pos] = 0;
    pos++;

    int n;
    printf("Nhap so o dia : ");
    scanf("%d",&n);
    clean_stdin();

    char o;
    short int kt;

    for(int i=0; i<n; i++){
        printf("Nhap ten o dia : ");
        scanf("%c", &o);
        clean_stdin();
        printf("Nhap kich thuoc o : ");
        scanf("%hd", &kt);
        clean_stdin();
        buf[pos] = o;
        pos++;
        memcpy(buf + pos, &kt, 2);
        pos+=2;
    }

    printf("Buffer size : %d\n", pos);
    // printf("Buffer : %s0%s\n", buf, buf + 1 + strlen(may));

    write(client, buf, pos);

    close(client);

}