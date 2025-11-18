#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len;

    if (argc != 3)
    {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    // 1) 소켓 생성
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    // 2) 서버 주소 정보 초기화
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); // IP 주소
    serv_addr.sin_port = htons(atoi(argv[2]));      // 포트 번호

    // 3) 서버에 연결 요청
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");

    // 4) 서버가 보내준 데이터 읽기
    str_len = read(sock, message, sizeof(message) - 1);
    if (str_len == -1)
        error_handling("read() error");

    // 문자열 끝에 널문자 추가
    message[str_len] = '\0';

    printf("Message from server: %s\n", message);

    // 5) 소켓 닫기
    close(sock);

    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputs("\n", stderr);
    exit(1);
}
