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
    FILE *f = fopen("content_string.txt", "r");
    char buf[255];

    // trung chuyển phần dữ liệu dư qua vòng lặp
    char *temp ="0123456789";
    
    // đọc file cho đến khi kết thúc
    while(!feof(f)){

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
        printf("do dai phan du %d \n", count);
        printf("phan du : %s\n", s);

        // nối dữ liệu dư vào con trỏ p
        char *p = 0;
        p = realloc(p, count);
        memcpy(p, s, count);

        // đọc từ file
        fgets(buf, sizeof(buf), f);
        if( buf[strlen(buf)-1] == '\n' ) buf[strlen(buf)-1] = 0; 
        printf("Chuoi doc tu file : %s\n", buf);
        printf("Do dai buf : %d\n", strlen(buf));

        // nối dữ liệu file vào con trỏ p
        p = realloc(p, count + strlen(buf));
        memcpy(p + count, buf, strlen(buf));
        //printf("p la %s\n", p);
        
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
        //printf("So lan xuat hien : %d\n", n);
        //printf("temp la [%s]\n\n", temp);
    }
    fclose(f);

    return 0;

}