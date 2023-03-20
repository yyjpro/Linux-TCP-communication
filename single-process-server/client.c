/* 
  TCP client
*/

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(){
    //1. create socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1){
        perror("socket");
        exit(-1);
    }

    //2. connect to server
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, "192.168.241.128", &serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(9998);
    //fd is the file descriptor used for communication with server
    int ret = connect(fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if(ret == -1){
        perror("connect");
        exit(-1);
    }

    //3.communictaion using fd with server
    char recvBuf[1024] = {0};
    while(1){
        //send info to server
        char data[1024];
        memset(data, 0, sizeof(data));
        printf("please input your data:\n");
        scanf("%[^\n]%*c", data);//scanf can read the space and throw \n
        write(fd, data, strlen(data));

        memset(recvBuf, 0, sizeof(recvBuf));
        int len = read(fd, recvBuf, sizeof(recvBuf));
        if(len == -1){
            perror("read");
            exit(-1);
        }else if(len > 0){
            printf("server successfully receive the data: %s\n", recvBuf);
        }else if(len == 0){
            //represent server is broken
            printf("server closed...\n");
            break;
        }
    }

    //close file descriptor
    close(fd);

    return 0;
}