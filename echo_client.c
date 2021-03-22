#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <winsock2.h>

#define BUF_SIZE 1024

void ErrorHandling(char *message);

int main(int argc, char *argv[])
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 0), &wsaData);

    SOCKET hSocket;
    char message[BUF_SIZE];
    int str_len;

    struct sockaddr_in serv_adr;

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

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if (connect(hSocket, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
    {
        ErrorHandling("socket() error");
    }
    else
    {
        puts("Connected \n");
    }

    while (1)
    {
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);

        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
        {
            break;
        }

        send(hSocket, message, strlen(message), 0);
        str_len = recv(hSocket, message, strlen(message), 0);
        message[str_len] = 0;
        printf("Message from server: %s", message);
    }

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