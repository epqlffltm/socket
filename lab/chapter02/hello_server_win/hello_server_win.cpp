#include <iostream>
#include <cstdlib>
#include <winsock2.h>
#include <cstring>



void error_handling(const char* message);

int main(int argc, char* argv[])
{
    WSADATA wsa_data;
    SOCKET h_serv_sock, h_clnt_sock;
    SOCKADDR_IN serv_adr, clnt_adr;

    int sz_clnt_addr;
    const char message[] = "Hello World!";

    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <port>" << std::endl;
        exit(1);
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
        error_handling("WSAStartup() error!");

    h_serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (h_serv_sock == INVALID_SOCKET)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(h_serv_sock, (SOCKADDR*)&serv_adr, sizeof(serv_adr)) == SOCKET_ERROR)
        error_handling("bind() error");

    if (listen(h_serv_sock, 5) == SOCKET_ERROR)
        error_handling("listen() error");

    sz_clnt_addr = sizeof(clnt_adr);
    h_clnt_sock = accept(h_serv_sock, (SOCKADDR*)&clnt_adr, &sz_clnt_addr);
    if (h_clnt_sock == INVALID_SOCKET)
        error_handling("accept() error");

    send(h_clnt_sock, message, strlen(message), 0);

    closesocket(h_clnt_sock);
    closesocket(h_serv_sock);
    WSACleanup();

    return 0;
}

void error_handling(const char* message)
{
    std::cerr << message << std::endl;
    exit(1);
}
