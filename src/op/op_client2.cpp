/*
2026-02-18
계산기 클라이언트
*/

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "../error_handling.h"

int main(int argc, char *argv[])
{
    // 변수
    const int buf_size = 1024;
    const int opsz = 4;  // int 크기
    int opnd_cnt = 0;
    
    struct sockaddr_in serv_adr;

    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <IP> <port>" << std::endl;
        exit(1);
    }

    // 소켓 생성
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        error_handling("socket");
    }

    // 서버 주소 설정
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    // 연결
    if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
    {
        error_handling("connect");
    }
    else
    {
        std::cout << "Connected to server!" << std::endl;
    }

    // 피연산자 개수 입력
    std::cout << "Operand count: ";
    std::cin >> opnd_cnt;

    // 메시지 버퍼 할당
    std::vector<char> opmsg(1 + opnd_cnt * opsz + 1);
    opmsg[0] = (char)opnd_cnt;

    // 피연산자 입력
    for (int i = 0; i < opnd_cnt; i++)
    {
        int operand;
        std::cout << "Operand " << i + 1 << ": ";
        std::cin >> operand;
        
        // int를 바이트로 복사
        memcpy(&opmsg[1 + i * opsz], &operand, opsz);
    }

    // 연산자 입력
    char op;
    std::cout << "Operator (+,-,*): ";
    std::cin >> op;
    opmsg[1 + opnd_cnt * opsz] = op;

    // 전송
    write(sock, opmsg.data(), opmsg.size());

    // 결과 수신
    int result;
    read(sock, &result, sizeof(result));

    std::cout << "Operation result: " << result << std::endl;
    
    close(sock);

    return 0;
}