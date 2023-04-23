    ret = recv(client, buf, sizeof(buf), 0); // trả về số byte đã nhận, 0 nếu đóng kết nối, -1 nếu bị lỗi
    if (ret <= 0)
    {
        printf("Connection closed\n");
        return 1;
    }

    if (ret < sizeof(buf))
        buf[ret] = 0; // byte cuối của chuỗi là ký tự kết thúc 0
    printf("%d bytes received: %s\n", ret, buf);