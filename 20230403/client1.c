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
    // tạo socket client
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // trả về mô tả socket
    
    // Khai báo cấu trúc đại chỉ client
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000);

    // Yêu cầu thiết lập liên kết từ client
    int ret = connect(client, (struct sockaddr *)&addr, sizeof(addr)); // trả về 0 hoặc 1
    if (ret == -1)
    {
        perror("connect() failed");
        return 1;
    }

    // Nhận dữ liệu từ server thông qua socket client trung gian
    char buf[256];
    ret = recv(client, buf, sizeof(buf), 0); // trả về số byte đã nhận, 0 nếu đóng kết nối, -1 nếu bị lỗi
    if (ret <= 0)
    {
        printf("Connection closed\n");
        return 1;
    }

    if (ret < sizeof(buf))
        buf[ret] = 0; // byte cuối của chuỗi là ký tự kết thúc 0
    printf("%d bytes received: %s\n", ret, buf);

    // NHập và gửi dữ liệu liên tục cho đến khi đóng kết nối
    while (1)
    {
        printf("Enter string:");
        fgets(buf, sizeof(buf), stdin);

        send(client, buf, strlen(buf), 0); // gửi thông qua socket

        if (strncmp(buf, "exit", 4) == 0)
            break;
    }

    close(client);
}