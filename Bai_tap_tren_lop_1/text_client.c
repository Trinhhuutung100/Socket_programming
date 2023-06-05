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
    // tạo socket cho client
    int client = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP); 

    // tạo cấu trúc địa chỉ cho socket
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port  = htons(9000);

    // kết nối với server
    connect(client, (struct sockaddr *)&addr, sizeof(addr));

    FILE *f = fopen("content_string.txt", "r");
    char buf[256];
    
    // đọc file cho đến khi kết thúc
    while(!feof(f)){

        fgets(buf, sizeof(buf), f);
        if( buf[strlen(buf)-1] == '\n' ) buf[strlen(buf)-1] = 0; 
        printf("Chuoi doc tu file : %s\n", buf);
        printf("Do dai buf : %d\n\n", strlen(buf));
        write(client, buf, strlen(buf));

    }

    close(client);
}
