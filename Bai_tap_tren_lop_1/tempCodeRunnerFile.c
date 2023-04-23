p = realloc(p + strlen(p), strlen(buf));
        memcpy(p + strlen(p), buf, strlen(buf));
        int n=0;
        while(p = strstr(p, "0123456789")){
            p++;
            n++;
        }