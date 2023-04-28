    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <string.h>
    #include <sys/select.h>

    void ResetString(char str[]){
        for(int i=0; i<strlen(str); i++){
            str[i] = 0;
        }
    }

    int main()
    {
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    bind(listener, (struct sockaddr *)&addr, sizeof(addr));

    listen(listener, 5);

    fd_set fdread;
        
    int clients[64];
    char acc[64][20];
    for(int i = 0; i< 65; i++) ResetString(acc[i]);
    int num_clients = 0;  
    char buf[256];
    char clientNames[][20] = {"abc", "def", "ghi"};
    int soten = sizeof(clientNames) / sizeof(clientNames[0]);
    //for(int i = 0; i<soten; i++) printf("%s %d bytes ", clientNames[i], strlen(clientNames[i]));
    char temp[20];
    ResetString(temp);


    while (1)
    {
        // Xóa tất cả socket trong tập fdread
        FD_ZERO(&fdread);

        // Thêm socket listener vào tập fdread
        FD_SET(listener, &fdread);
        int maxdp = listener + 1;

        // Thêm các socket client vào tập fdread
        for (int i = 0; i < num_clients; i++)
        {
            FD_SET(clients[i], &fdread);
            if (maxdp < clients[i] + 1)
                maxdp = clients[i] + 1;
        }

        // Chờ đến khi sự kiện xảy ra
        int ret = select(maxdp, &fdread, NULL, NULL, NULL);

        if (ret < 0)
        {
            perror("select() failed");
            return 1;
        }

        // Kiểm tra sự kiện có yêu cầu kết nối
        if (FD_ISSET(listener, &fdread))
        {
            int client = accept(listener, NULL, NULL);
            printf("Ket noi moi: %d\n", client);
            clients[num_clients++] = client;
        }

        // Kiểm tra sự kiện có dữ liệu truyền đến socket client
        for (int i = 0; i < num_clients; i++)
            if (FD_ISSET(clients[i], &fdread))
            {
                ret = recv(clients[i], buf, sizeof(buf), 0);
                if (ret <= 0)
                {
                    // TODO: Client đã ngắt kết nối, xóa client ra khỏi mảng
                    if(strcmp(acc[i], temp) != 0) strcpy(clientNames[i], acc[i]);
                    ResetString(acc[i]);
                    continue;
                }
                buf[ret-1] = 0 ;

                printf("Du lieu nhan tu %d: %s [%d bytes]\n", clients[i], buf, strlen(buf));
                
                if(strcmp(acc[i], temp) == 0){
                    //printf("acc name hien tai (%s)\n", acc[i]);
                    for(int j =0;j< soten; j++){
                        int t = strcmp(buf, clientNames[j]);
                        //printf("t %d\n", t);
                        if( t == 0)  {
                            char success[20];
                            sprintf(success,"Client %d dang nhap thanh cong\n", clients[i]);
                            send(clients[i], success, strlen(success), 0);
                            printf("Client %d dang nhap thanh cong\n", clients[i]);
                            strcpy(acc[i], clientNames[j]);
                            ResetString(clientNames[j]);
                            break;
                        }
                    }
                    continue;
                }

                for (int j = 0; j < num_clients; j++){
                    if(j==i) continue;
                    if(strcmp(acc[j], temp) == 0) continue;
                    char from[20];
                    sprintf(from, "From %s : %s", acc[i], buf);
                    send(clients[j], from, strlen(from), 0);
                }

            }
    }

    close(listener);    

    return 0;
}