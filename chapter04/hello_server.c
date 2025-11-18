#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>//리눅스와 같은 POSIX 시스템에서 네트워크 소켓 통신을 위한 주요 함수와 구조체를 선언하는 헤더 파일
#include<unistd.h>//Unix 및 POSIX 호환 시스템에서 저수준 시스템 호출 함수와 관련 상수를 사용하기 위해 포함하는 헤더 파일
#include<arpa/inet.h>//헤더 파일은 IPv4 주소를 인터넷 표준 형식(\(\text{a.b.c.d}\))과 네트워크 바이트 순서(big-endian) 사이에서 변환하는 함수(예: inet_aton, inet_ntoa, inet_pton, inet_ntop)를 사용하기 위해 포함시킵니다. 이 파일은 소켓 프로그래밍에서 주소를 다룰 때 주로 사용됩니다. 

void error_handling(char *message);

int main(int argc, char *argv[])
{
  //int argc = argument count (인자 개수) = 프로그램을 실행할 때 입력받은 문자열의 총 개수
  //char *argv[] = argument vector (인자 목록 배열) = 문자열 배열이고, 각 요소는 *C 문자열(char )
  /*
  argc = 3
  argv[0] = program.exe
  argv[1] = test
  argv[2] = 123
  */

  int serv_sock;//server socket
  int clnt_sock;//client socket

  struct sockaddr_in serv_addr;//서버 주소 구조체
  struct sockaddr_in clnt_addr;//클라이언트 주소 구조체
  socklen_t clnt_addr_size;

  char message[]="hallo wared!";

  if(argc!=2)//argc가 2가 아니면
  {
    printf("Usage : %s<PORT>\n",argv[0]);//program.exe출력
    exit(1);
  }

  serv_sock = socket(PF_INET, SOCK_STREAM, 0);//IPv4
  if (serv_sock == -1)
  error_handling("socket() error");

  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family=AF_INET;
  serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));


  if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
  error_handling("bind() error");

  if(listen(serv_sock,5)==-1)
  error_handling("listen() error");

  clnt_addr_size=sizeof(clnt_addr);
  clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
  if(clnt_sock==-1)
  error_handling("accept() error");

  write(clnt_sock,message,sizeof(message));
  close(clnt_sock);
  close(serv_sock);
  
  return 0;
}

void error_handling(char *message)
{
  fputs(message, stderr);
  fputc('\n',stderr);
  exit(1);
}