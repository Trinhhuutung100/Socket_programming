#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

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

    // Đọc nội dung từ file để gửi đi
    FILE *fr = fopen(argv[2], "r");
    char buf[2048];
    fscanf(fr,"%[^\n]", buf);
    send(client, buf, strlen(buf), 0);
    fclose(fr);

    // Ghi nội dung nhận được vào file 
    FILE *fw = fopen(argv[3], "w"); 
    int ret;
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
        fprintf(fw,"%s", buf);
    }
    fclose(fw);
    
    close(client);
    close(listener);
}
