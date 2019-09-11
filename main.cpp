#include <iostream>
#include <winsock2.h>
#include <cstdio>
#include <inaddr.h>
#include <time.h>
#include <cstdlib>

#pragma comment(lib,"ws2_32.lib")

int main() {
    WSADATA wsaData;

    int err = WSAStartup(MAKEWORD(2,2),&wsaData);
    if(err!=0)
    {
        return -1;
    }

    SOCKET clientSock = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP);

    SOCKADDR_IN clientAddr;
    clientAddr.sin_port=htons(12345);
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    //bind(clientSock, (SOCKADDR*)&clientAddr, sizeof(SOCKADDR));

    int len=sizeof(clientAddr);
    char *sendBuf = "Hello, server";
    sendto(clientSock,sendBuf,strlen(sendBuf),0,(SOCKADDR*)&clientAddr,len);

//    char recvData[80];
//    int recvNum = recvfrom(clientSock,recvData,80,0,(SOCKADDR*)&clientAddr,&len);
//    printf("test");
//    if(recvNum>0)
//    {
//        printf("Successfully get the message from server:%s",recvData);
//    }

    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 500000;
    int sendNum = 0;
    fd_set rfd;
    while(sendNum<20)
    {
        FD_ZERO(&rfd);
        FD_SET(clientSock,&rfd);
        int nRet = select(0,&rfd,NULL,NULL,&timeout);
        if(nRet==SOCKET_ERROR)
        {
            printf("error\n");
            return -2;
        }
        else if(FD_ISSET(clientSock,&rfd)) {

            char check_str[5] = {0};
            int recvNum = recvfrom(clientSock,check_str,5,0,(SOCKADDR*)&clientAddr,&len);
            int check_num = atoi(check_str);
            if(check_num>100)
            {
                printf("Bigger than 100\n");
            }

            int randomNum = rand() % 500 + 1;
            printf("%d\n", randomNum);

            char sendData[5] = {0};
            itoa(randomNum, sendData, 10);

            sendto(clientSock, sendData, strlen(sendData), 0, (SOCKADDR *) &clientAddr, len);
            sendNum++;
        }
        else if(nRet==0)//timeout
        {
            int randomNum = rand() % 500 + 1;
            char sendData[5] = {0};
            itoa(randomNum, sendData, 10);
            printf("%d\n", randomNum);
            sendto(clientSock, sendData, strlen(sendData), 0, (SOCKADDR *) &clientAddr, len);
            sendNum++;
        }
    }
    closesocket(clientSock);
    WSACleanup();
    return 0;
}