#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// 에러 발생 시 메시지 출력 후 프로그램 종료
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sock;     // 서버 소켓 (손님 받을 문)
    int clnt_sock;     // 클라이언트 소켓 (손님 전용 좌석)

    struct sockaddr_in serv_addr; // 서버 주소 구조체
    struct sockaddr_in clnt_addr; // 클라이언트 주소 구조체
    socklen_t clnt_addr_size;

    char message[] = "hello world"; // 클라이언트에게 보낼 메시지

    // 프로그램 실행 시 포트를 제대로 입력했는지 체크
    // ex) ./server 9000
    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    /*
    -----------------------------------------------------
    1) socket() : 서버 소켓 생성
    PF_INET      → IPv4
    SOCK_STREAM  → TCP 방식 (연결형)
    0            → 기본 프로토콜(TCP)
    -----------------------------------------------------
    */
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    /*
    -----------------------------------------------------
    2) 서버 주소 설정
    sin_family      : 주소 체계 (IPv4)
    sin_addr.s_addr : 어떤 IP에서 접속해도 허용(INADDR_ANY)
    sin_port        : 포트 번호 (네트워크 바이트 순서로 변환)
    -----------------------------------------------------
    */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // 모든 IP 허용
    serv_addr.sin_port = htons(atoi(argv[1]));      // 포트 번호

    /*
    -----------------------------------------------------
    3) bind()
    소켓에다가 "주소 + 포트"를 연결시키는 작업.
    즉, "이 소켓은 이 위치에서 서비스합니다" 등록하는 과정.
    -----------------------------------------------------
    */
    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    /*
    -----------------------------------------------------
    4) listen()
    서버가 클라이언트 연결 요청을 기다릴 수 있게 전환.
    5는 대기열 크기.
    -----------------------------------------------------
    */
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    /*
    -----------------------------------------------------
    5) accept()
    - 실제 클라이언트가 접속할 때까지 블로킹(멈춤)
    - 손님 전용 소켓(clnt_sock)이 새로 생성됨
    -----------------------------------------------------
    */
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1)
        error_handling("accept() error");

    /*
    -----------------------------------------------------
    6) 클라이언트에게 메시지 전송
    write(손님소켓, 보낼메시지, 메시지크기)
    -----------------------------------------------------
    */
    write(clnt_sock, message, sizeof(message));

    /*
    -----------------------------------------------------
    7) 소켓 닫기
    서버 소켓, 클라이언트 소켓 전부 닫아야 함.
    -----------------------------------------------------
    */
    close(clnt_sock);
    close(serv_sock);

    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputs("\n", stderr);
    exit(1);
}