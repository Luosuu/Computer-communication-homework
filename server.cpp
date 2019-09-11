#include <iostream>
#include <cstdio>
#include <winsock2.h>
#include <inaddr.h>
#pragma comment(lib, "ws2_32.lib")
#include <ctime>

int main() {
    WSADATA wsaData;

    int err = WSAStartup(MAKEWORD(2,2),&wsaData);
    if(err!=0)
    {
        return -1;
    }

    SOCKET serverSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    SOCKADDR_IN serverAddr;
    serverAddr.sin_addr.S_un.S_addr=INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);

    bind(serverSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));

    char recvData[80];

    SOCKADDR_IN remoteAddr;
    int len = sizeof(remoteAddr);

    char *sendBuf = "Hello, client. Gotten your message";
    sendto(serverSock, sendBuf, strlen(sendBuf) + 1, 0, (SOCKADDR *) &remoteAddr, len);

    srand(time(NULL));
    int sendNum = 0;
    while(sendNum<=20) {
        int recvNum = recvfrom(serverSock, recvData, 80, 0, (SOCKADDR *) &remoteAddr, &len);
        if (recvNum > 0) {
            recvData[recvNum] = 0x00;
            printf("Successfully get the message: %s\n", recvData);
        }

        //generate a random number
        int randomSum = rand()%500+1+ atoi(recvData);
        //int randomSum = atoi(recvData);
        //int recvRandom = atoi(recvData);
        if(randomSum>100)
        {
            char  sendData[5] ={0};
            itoa(randomSum, sendData, 10);
            sendto(serverSock, sendData, strlen(sendData) + 1, 0, (SOCKADDR *) &remoteAddr, len);
        }

        sendNum++;
    }
    closesocket(serverSock);
    WSACleanup();
    return 0;
}
