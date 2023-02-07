#include "header.h"

void z_handler(int sig)
{
  pid_t pid;
  int rtn;

  pid=waitpid(-1, &rtn, WNOHANG);
  //printf("소멸된 좀비의 프로세스 ID : %d \n", pid);
  //printf("리턴된 데이터 : %d \n\n", WEXITSTATUS(rtn));
 
}

void error_handling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
