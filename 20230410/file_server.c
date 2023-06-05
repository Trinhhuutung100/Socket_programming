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
    addr.sin_port = htons(9000);

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

    char filename[256];
    int name_size; // kích thước tên file
    recv(client, &name_size, sizeof(int), 0);

    // nhận từ client tên file
    recv(client, filename, name_size, 0);
    filename[name_size] = 0;

    char buf[2048]; 
    sprintf(buf, "new_%s", filename); // ghi vào buf tên file
    FILE *f = fopen(buf, "wb"); //
    
    int ret;

    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;
        fwrite(buf, 1, ret, f);
    }

    fclose(f);
    
    close(client);
    close(listener);
}
