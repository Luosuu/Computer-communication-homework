#include <iostream>
#include <cstdio>
#include <winsock2.h>
#include <inaddr.h>
#pragma comment(lib,"ws2_32.lib")

int main() {
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)
    {
        return -1;
    }

    SOCKET appLayerSock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

    SOCKADDR_IN appLayerAddr;
    appLayerAddr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
    appLayerAddr.sin_family = AF_INET;
    appLayerAddr.sin_port = htons(12345);//set appLayer port as 12345

    bind(appLayerSock,(SOCKADDR*)&appLayerAddr,sizeof(appLayerAddr));//设置监听

    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 5000;
    fd_set rfd;
    int len = sizeof(appLayerAddr);

    while(1)
    {
        FD_ZERO(&rfd);
        FD_SET(appLayerSock,&rfd);
        int nRet = select(0,&rfd,NULL,NULL,&timeout);
        if(nRet==SOCKET_ERROR)
        {
            printf("error\n");
            break;
        }
        //设置一个输入模块,送到网络层
        //*
        //*
        else if(FD_ISSET(appLayerSock,&rfd)){
            //get message from network layer
            char netMessage[100] = {0};
            int retval = recvfrom(appLayerSock, netMessage,5,0,(SOCKADDR*)&appLayerAddr,&len);
            if(retval>0){
                std::cout << "Successfully get message from network layer" << std::endl;
            }
        }
    }


    std::cout << "Hello, World!" << std::endl;
    return 0;
}