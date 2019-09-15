#include <iostream>
#include <winsock2.h>
#include <cstdio>
#include <inaddr.h>
#pragma comment(lib,"ws2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
        return -1;
    }
    SOCKET networkCatchAppSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    SOCKADDR_IN networkCatchAppAddr;

    networkCatchAppAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    networkCatchAppAddr.sin_family = AF_INET;
    networkCatchAppAddr.sin_port = htons(8000);//use 8000 port get message from app

    bind(networkCatchAppSock,(SOCKADDR*)&networkCatchAppAddr, sizeof(SOCKADDR));

    SOCKET networkCatchPhySock = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP);
    SOCKADDR_IN networkCatchPhyAddr;

    networkCatchPhyAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    networkCatchPhyAddr.sin_family = AF_INET;
    networkCatchPhyAddr.sin_port = htons(7000);//use 7000 port get message from phy

    bind(networkCatchPhySock, (SOCKADDR*)&networkCatchPhyAddr, sizeof(SOCKADDR));

    //set app layer addr
    SOCKADDR_IN appLayerAddr;
    appLayerAddr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
    appLayerAddr.sin_family = AF_INET;
    appLayerAddr.sin_port = htons(12345);//set appLayer port as 12345
    //set phy layer addr
    SOCKADDR_IN phyLayerAddr;
    phyLayerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    phyLayerAddr.sin_family = AF_INET;
    phyLayerAddr.sin_port = htons(9999);//set phyLayer port as 9999
    //select element
    timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    fd_set rfd;
    int appLayerAddrLen = sizeof(appLayerAddr);
    int phyLayerAddrLen = sizeof(phyLayerAddr);

    while(1)
    {

        FD_ZERO(&rfd);
        FD_SET(networkCatchAppSock,&rfd);
        FD_SET(networkCatchPhySock,&rfd);
        int nRet = select(0,&rfd,NULL,NULL,&timeout);//check port readable
        std::cout << "justice" <<std::endl;
        if(nRet==SOCKET_ERROR)
        {
            printf("error\n");
            break;
        }
        else if(FD_ISSET(networkCatchAppSock,&rfd))
        {

            char appStr[100] = {0};
            int recvApp = recvfrom(networkCatchAppSock,appStr,100,0,(SOCKADDR*)&appLayerAddr,&appLayerAddrLen);
            std::cout << "get:"<< appStr << std::endl;
            sendto(networkCatchAppSock,appStr,strlen(appStr)+1,0,(SOCKADDR*)&phyLayerAddr, sizeof(phyLayerAddr));
        }
        else if (FD_ISSET(networkCatchPhySock,&rfd))
        {
            char phyStr[100] = {0};
            int recvPhy = recvfrom(networkCatchPhySock,phyStr,100,0,(SOCKADDR*)&phyLayerAddr,&phyLayerAddrLen);
            sendto(networkCatchPhySock,phyStr,strlen(phyStr),0,(SOCKADDR*)&appLayerAddr, sizeof(appLayerAddr));
        }
        else if(nRet==0)
        {
            //std::cout << "timeout" <<std::endl;
        }
    }
    closesocket(networkCatchAppSock);
    closesocket(networkCatchPhySock);
    WSACleanup();
    return 0;
}