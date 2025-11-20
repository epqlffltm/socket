#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define OPSZ 4

void error_handling(char *message);
int calculate(int opnum, int opnds[], char op);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    char opinfo[BUF_SIZE];
    int result = 0, opnd_cnt = 0;
    int recv_cnt, recv_len;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    if (argc != 2)
    {
        printf("Usage : %s <PORT> \n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));   // thons → htons

    // band → bind, serv_adt → serv_adr
    if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    clnt_adr_sz = sizeof(clnt_adr);

    for (int i = 0; i < 5; i++)
    {
        opnd_cnt = 0;
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
        if (clnt_sock == -1)
            error_handling("accept() error");

        // 클라이언트가 먼저 1바이트로 피연산자 개수 전송
        read(clnt_sock, &opnd_cnt, 1);

        recv_len = 0;
        // 피연산자(opnd_cnt * 4바이트) + 연산자(1바이트) 만큼 수신
        while ((opnd_cnt * OPSZ + 1) > recv_len)
        {
            recv_cnt = read(clnt_sock, &opinfo[recv_len], BUF_SIZE);  // recv_cnt-read → =
            if (recv_cnt == -1)
                error_handling("read() error");
            else if (recv_cnt == 0)
                break;
            recv_len += recv_cnt;
        }

        // opinfo에는 [피연산자들(4바이트씩)] [마지막 1바이트 = 연산자]
        result = calculate(opnd_cnt, (int *)opinfo, opinfo[recv_len - 1]);

        write(clnt_sock, (char *)&result, sizeof(result));
        close(clnt_sock);
    }

    close(serv_sock);
    return 0;
}

int calculate(int opnum, int opnds[], char op)
{
    int result = opnds[0];

    switch (op)          // swich → switch
    {
    case '+':
        for (int i = 1; i < opnum; i++)
            result += opnds[i];
        break;

    case '-':
        for (int i = 1; i < opnum; i++)
            result -= opnds[i];
        break;

    case '*':
        for (int i = 1; i < opnum; i++)
            result *= opnds[i];
        break;
    }
    return result;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
