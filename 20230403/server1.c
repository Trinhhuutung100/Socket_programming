#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    // Tạo socket server
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        return 1;
    }

    // Khai báo cấu trúc dịa chỉ cho server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    // Gắn socket với cáu trúc địa chỉ
    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)))
    {
        perror("bind() failed");
        return 1;
    }

    // Chuyên server sang trạng thái nghe, hàng đợi kết nối là 5
    if (listen(listener, 5))
    {
        perror("listen() failed");
        return 1;
    }

    // Khai báo cấu trúc địa chỉ client
    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(addr); // Kích thước cấu trúc địa chỉ

    // Cho server chấp nhận yêu cầu kết nối của client, tạo mới socket client
    int client = accept(listener, (struct sockaddr *)&clientAddr, &clientAddrLen); // trả về là mô tả socket trung gian
    printf("Client IP: %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

    // Gửi dữ liệu cho client
    char *msg = "Hello client\n";
    send(client, msg, strlen(msg), 0);

    char buf[256];

    // Nhận dữ liệu từ client cho đến khi ngắt kết nối
    while (1)
    {
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
        {
            printf("Ket noi bi dong.\n");
            break;    
        }
        if (ret < sizeof(buf))
            buf[ret] = 0; // byte cuối của chuỗi là ký tự kết thúc 0

        printf("%d bytes received\n", ret);
        printf("%s\n", buf);
    }
    
    close(client);
    close(listener);
}
