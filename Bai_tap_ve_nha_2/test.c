#include <stdio.h>
#include <string.h>

void ResetString(char str[]){
    for(int i=0; i<strlen(str); i++){
        str[i] = 0;
    }
}

int main(){
    char acc[][4] = {"abc", "def", "ghi"};
    ResetString(acc[0]);
    char temp[4];
    ResetString(temp);
    if (strcmp(acc[1], acc[1]) == 0) printf("Null\n");
    //printf("%s %s\n", acc[0], acc[1]); 
    char name[64][20];
    // for(int i=0; i< 64; i++) {
    //     ResetString(name[i]);
    //     printf("Chuoi (%s)", name[i]);
    // }
    return 0;
}