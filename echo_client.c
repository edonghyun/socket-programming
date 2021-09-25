#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <winsock2.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

void ErrorHandling(char *message);

int main(int argc, char *argv[])
{
    WSADATA wsaData;
    SOCKET hSocket;

    char opmsg[BUF_SIZE];
    int opndCnt, i;
    char result[BUF_SIZE];

    SOCKADDR_IN servAdr;
    if (argc != 3)
    {
        printf("Usage : %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        ErrorHandling("WSAStartup() error");
    }

    hSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (hSocket == INVALID_SOCKET)
    {
        ErrorHandling("socket() error");
    }

    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = inet_addr(argv[1]);
    servAdr.sin_port = htons(atoi(argv[2]));

    if (connect(hSocket, (struct sockaddr *)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
    {
        ErrorHandling("socket() error");
    }
    else
    {
        puts("Connected \n");
    }

    fputs("Operand count: ", stdout);
    scanf("%d", &opndCnt);
    opmsg[0] = (char)opndCnt;

    for (i = 0; i < opndCnt; i++)
    {
        printf("Operand %d: ", i + 1);
        scanf("%d", (int *)&opmsg[i * +1]);
    }

    fgetc(stdin);
    fputs("Operator: ", stdout);
    scanf("%c", &opmsg[opndCnt * OPSZ + 1]);
    send(hSocket, opmsg, opndCnt * OPSZ + 2, 0);
    recv(hSocket, result, BUF_SIZE, 0);

    printf("Operation result: %s \n", result);
    closesocket(hSocket);

    close(hSocket);
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