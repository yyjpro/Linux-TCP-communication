#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(){
    //1. create socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1){
        perror("socket");
        exit(-1);
    }

    //2. bind IP and port of localhost
    struct sockaddr_in saddr; //store information into sockaddr_in
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(9998);
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }

    //3. listen the client
    ret = listen(lfd, 8);
    if(ret == -1){
        perror("listen");
        exit(-1);
    }

    //4.accept connection of client
    struct sockaddr_in clientaddr;
    int len = sizeof(clientaddr);
    //return an cfd used for communication with client
    int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &len);
    if(cfd == -1){
        perror("accept");
        exit(-1);
    }
    
    //output client info
    char clientIP[16];
    inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, clientIP, sizeof(clientIP));
    unsigned short clientPort = ntohs(clientaddr.sin_port);
    printf("client ip is %s, port is %d\n", clientIP, clientPort);

    //5.communication using cfd with client
    char recvBuf[1024] = {0};
    while(1){
        memset(recvBuf, 0, sizeof recvBuf);
        //get info from client
        int num = read(cfd, recvBuf, sizeof(recvBuf));
        if(num == -1){
            perror("read");
            exit(-1);
        }else if(num > 0){
            printf("recv client data: %s\n", recvBuf);
        }else if(num == 0){
            //represent client is broken
            printf("client closed...\n");
            break;
        }

        //Reveive the message from client, and back the same
        char *data = recvBuf;
        write(cfd, data, strlen(data));
    }

    //close the file descriptor
    close(cfd);
    close(lfd);

    return 0;
}