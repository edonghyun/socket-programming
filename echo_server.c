#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 1024

void ErrorHandling(char *message);

int main(int argc, char *argv[])
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 0), &wsaData);

    SOCKET hServSock, hClntSock;
    char message[BUF_SIZE];
    int str_len, i;

    SOCKADDR_IN serv_adr, clnt_adr;
    int clntAdrSize;

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        ErrorHandling("WSAStartup() error");
    }

    hServSock = socket(AF_INET, SOCK_STREAM, 0);
    if (hServSock == INVALID_SOCKET)
    {
        ErrorHandling("socket() error");
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(hServSock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
    {
        ErrorHandling("bind() error");
    }

    if (listen(hServSock, 5) == -1)
    {
        ErrorHandling("bind() error");
    }

    clntAdrSize = sizeof(clnt_adr);

    for (i = 0; i < 5; i++)
    {

        hClntSock = accept(hServSock, (SOCKADDR *)&clnt_adr, &clntAdrSize);
        if (hServSock == INVALID_SOCKET)
        {
            ErrorHandling("accept() error");
        }
        else
        {
            printf("Connected client %d \n ", i + 1);
        }

        while ((str_len = recv(hClntSock, message, BUF_SIZE, 0)) != 0)
        {
            send(hClntSock, message, str_len, 0);
        }

        close(hClntSock);
    }

    close(hServSock);
    WSACleanup();
    return 0;
}

void ErrorHandling(char *message)
{
    printf("error cod %d\n", WSAGetLastError());
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}