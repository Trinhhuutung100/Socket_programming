#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int main()
{
   // tạo socket nghe
   int server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

   // tạo cấu trúc địa chỉ cho socket
   struct sockaddr_in addr;
   addr.sin_family = AF_INET;
   addr.sin_addr.s_addr = htonl(INADDR_ANY);
   addr.sin_port = htons(9000);

   // Gắn cấu trúc địa chỉ với socket
   bind(server, (struct sockaddr *)&addr, sizeof(addr));

   // chuyển socket sang trạng thái nghe
   listen(server, 5);

   // tạo cấu trúc đại chỉ cho client
   struct sockaddr_in clientAddr;
   int clientAddrLen = sizeof(clientAddr);

   // chấp nhận kết nối
   int client = accept(server, (struct sockaddr *) &clientAddr, &clientAddrLen);

   char buf[256];
   int ret=1;
   // trung chuyển phần dữ liệu dư qua vòng lặp
   char *temp ="0123456789";
   int sum = 0;

   while(1){ 
      ret =  read(client, buf, sizeof(buf));
      if(ret<=0) break;
      // bỏ qua 10 ký tự đầu là "0123456789"
        if(strstr(temp, "0123456789")!=0) temp +=10;

        // lưu dữ liệu dư
        char *s = temp;

        // tính độ dài dữ liệu dư
        int count = 0;
        while (*temp != '\0') {
            count++;
            temp++;
        }

        // chỉ giữ 9 ký tự cuối phàn dư để nối vào buf
        if(count>=10){
            s = s + count - 9;
            count = 9;
        }
        printf("Chuỗi dư : %s\n", s);
        printf("Độ dài chuỗi dư : %d \n", count);

        // nối dữ liệu dư vào con trỏ p
        char *p = 0;
        p = realloc(p, count);
        memcpy(p, s, count);

        // xử lý xâu nhận được
        if( buf[strlen(buf)-1] == '\n' ) buf[strlen(buf)-1] = 0; 
        printf("Chuỗi nhận được : \n%s\n", buf);
        printf("Độ dài chuỗi nhận được : %d\n", strlen(buf));

        // nối dữ liệu file vào con trỏ p
        p = realloc(p, count + strlen(buf));
        memcpy(p + count, buf, strlen(buf));
        printf("Chuỗi sau khi được nối : \n%s\n", p);
        
        // tính số lần xuất hiện của chuỗi "0123456789" trong p, sau đó gắn lần xuất hiện cuối cùng/ dữ liệu dư vào temp
        int n=0;
        char *back = 0;
        temp = p;
        while(p = strstr(p, "0123456789")){ // vòng lặp cuối p sẽ = null nên phải gán vào temp
            back = p;
            p++;
            n++;
        }
        //printf("back : %s\n", back);
        if(back!=NULL) temp = back;
        printf("Số lần xuất hiện : %d\n\n", n);
        //printf("temp la [%s]\n\n", temp);
        sum +=n;
   }

   printf("Tổng số lần xuất hiện xâu kí tự [0123456789] : %d\n", sum);

   close(client);
   close(server);
}