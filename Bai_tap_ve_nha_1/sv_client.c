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
    // tạo socket client
    int sv_client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // trả về mô tả socket
    
    // Khai báo cấu trúc đại chỉ client
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    // Yêu cầu thiết lập liên kết từ client
    int ret = connect(sv_client, (struct sockaddr *)&addr, sizeof(addr)); // trả về 0 hoặc 1
    if (ret == -1)
    {
        perror("connect() failed");
        return 1;
    }

    // NHập và gửi dữ liệu liên tục cho đến khi đóng kết nối
    char mssv[128], hoten[128], ngaysinh[128], gpa[128];
    char data[10240];

    while(1)
    {
        printf("Nhập MSSV : ");
        fgets(mssv, sizeof(mssv), stdin);
        if (strncmp(mssv, "exit", 4) == 0){
            printf("Ket noi bi dong\n");
            break;
        }
        mssv[strlen(mssv)-1]=0;

        printf("Nhập họ tên : ");
        fgets(hoten, sizeof(hoten), stdin);
        if (strncmp(hoten, "exit", 4) == 0){
            printf("Ket noi bi dong\n");
            break;
        }
        hoten[strlen(hoten)-1]=0;

        printf("Nhập ngày sinh : ");
        fgets(ngaysinh, sizeof(ngaysinh), stdin);
        if (strncmp(ngaysinh, "exit", 4) == 0){
            printf("Ket noi bi dong\n");
            break;
        }
        ngaysinh[strlen(ngaysinh)-1]=0;

        printf("Nhập điểm trung bình các môn học : ");
        fgets(gpa, sizeof(gpa), stdin);
        if (strncmp(gpa, "exit", 4) == 0){
            printf("Ket noi bi dong\n");
            break;
        }
        gpa[strlen(gpa)-1]=0;

        sprintf(data, "%s %s %s %s", mssv, hoten, ngaysinh, gpa);

        printf("Dữ liệu được gửi đi : \n%s\n", data);

        send(sv_client, data, strlen(data),0);
    }

    close(sv_client);
}