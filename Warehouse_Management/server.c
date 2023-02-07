#include "header.h"

int main(int argc, char **argv)
{
  int serv_sock;
  int clnt_sock;
  struct sockaddr_in serv_addr;
  struct sockaddr_in clnt_addr;
  struct sigaction act;
  int addr_size, str_len, state;
  pid_t pid;
  int fd1[2], fd2[2], fd3[2];
  double temperature[1] = {0.0};
  double temperatures = 0.0;
  char message[BUFSIZE] = "";
  int n = 2;           //몇대의 클라이언트

  if(argc!=2){
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }

  act.sa_handler=z_handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags=0;
  state=sigaction(SIGCHLD, &act, 0);                              /* 시그널 핸들러 등록 */
  if(state != 0) {
    puts("sigaction() error");
    exit(1);
  }
  
  serv_sock=socket(PF_INET, SOCK_STREAM, 0);   
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family=AF_INET;
  serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
  serv_addr.sin_port=htons(atoi(argv[1]));

  if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
    error_handling("bind() error");

  if(listen(serv_sock, 5)==-1)
    error_handling("listen() error");

  if(pipe(fd1) == -1 || (pipe(fd2) == -1) || (pipe(fd3) == -1))
    error_handling("pipe() error");
  
  pid=fork();

  if(pid==0) {

    while(1) {

      for(int i=0;i<n;i++) {
        read(fd1[0], temperature, sizeof(message));
        temperatures += temperature[0];
      }
 
      printf("temperatures average : %.2f\n", temperatures/n);
      if (temperatures/n < 13) strcpy(message,"hitter on!");
      else if(13 <= temperatures/n && temperatures/n < 15) strcpy(message,"nothing!");
      else strcpy(message,"aircondition on!");
      printf("to client : %s\n", message);

      for(int i=0;i<n;i++) {
        write(fd2[1], message, sizeof(message));
      }

      temperatures = 0.0;
    }
    return 0;
  }

  while(1)
  {   
    addr_size=sizeof(clnt_addr);    
    clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr, &addr_size);
    
    if(clnt_sock==-1)
      continue;
    if( (pid=fork()) == -1) {                
      close(clnt_sock);
      continue;
    }

    if(pid==0) {     /* 자식 프로세스의 경우 */    

      close(serv_sock);        
            
      while(1) {
        str_len=read(clnt_sock, temperature, BUFSIZE);
        write(fd1[1], temperature, str_len);

        read(fd2[0], message, sizeof(message));
        write(clnt_sock, message, sizeof(message));
      }

      close(clnt_sock);
      return 0;
    }
    else close(clnt_sock);
  }

  close(serv_sock);
  return 0;
}
