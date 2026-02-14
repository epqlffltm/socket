#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <winsock2.h>
#include <cstring>

void error_handling(const char* message);

int main(int argc, char* argv[])
{
    WSADATA wsa_data;
    SOCKET h_socket;
    SOCKADDR_IN serv_addr;

    char message[30];
    int str_len = 0;

    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " <IP> <port>\n";
        exit(1);
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
        error_handling("WSAStartup() error!");

    h_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (h_socket == INVALID_SOCKET)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(h_socket, (SOCKADDR*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
        error_handling("connect() error");

    str_len = recv(h_socket, message, sizeof(message) - 1, 0);
    if (str_len == -1)
        error_handling("recv() error");

    message[str_len] = '\0'; // ✅ 문자열 끝 표시
    std::cout << "Message from server: " << message << std::endl;

    closesocket(h_socket);
    WSACleanup();
    return 0;
}

void error_handling(const char* message)
{
    std::cerr << message << std::endl;
    exit(1);
}
