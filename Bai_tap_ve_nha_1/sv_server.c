#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int main(int argc, char *argv[])
{
    // tạo socket server
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        return 1;
    }

    // Khai báo cấu trúc địa chỉ server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

    // Gắn socket với cấu trúc địa chỉ
    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)))
    {
        perror("bind() failed");
        return 1;
    }

    // chuyển server sang trạng thái nghe, hàng đợi kết nối là 5
    if (listen(listener, 5))
    {
        perror("listen() failed");
        return 1;
    }

    // cấu trúc địa chỉ client
    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(addr);

    // Chấp nhận kết nối từ client
    int client = accept(listener, (struct sockaddr *)&clientAddr, &clientAddrLen);
    printf("Client IP: %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

    // Ghi nội dung nhận được vào file 
    FILE *f = fopen(argv[2], "a"); 
    char buf[10240];
    char data[10240];
    int ret;
    time_t t = time(NULL);
    struct tm tm;
    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
        {
            printf("Ket noi bi dong.\n");
            break;    
        }
        if (ret < sizeof(buf))
            buf[ret] = 0; // byte cuối của chuỗi là ký tự kết thúc 0
        printf("%d bytes received\n", ret);
        printf("%s\n", buf);

        tm = *localtime(&t);
        sprintf(data,"%s %d-%02d-%02d %02d:%02d:%02d %s", inet_ntoa(clientAddr.sin_addr), tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, buf);

        printf("Dữ liệu được ghi vào trong log : \n%s\n", data);

        fprintf(f,"%s\n", data);
    }
    fclose(f);
    
    close(client);
    close(listener);
}
