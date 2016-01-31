#include <signal.h>
#include <stdio.h>
#include <unistd.h>


void  handler(int sig)
{

 return;
}

int main(int argc, char **argv)
{
  struct sigaction sa = {
	0, //&handler;
	SIGINT | SIGTERM,
	0
  };
  sa.sa_handler=SIG_IGN;

  struct sigaction osa;

//   
//  signal(SIGINT, &handler);
  sigaction(SIGINT, &sa , 0);
  sigaction(SIGTERM, &sa , 0);

  pid_t pid=getpid();
  printf("pid: %d\n", pid);

//#define FPID ("/home/box/pid")
#define FPID ("./pid-file")
  FILE *file=fopen(FPID, "w");
  if (NULL == file) {
	perror("fopen");
	return(1);
  }
  int rc = fprintf(file, "%d\0", pid);
  fflush(file);
  printf("%d\n", rc);
  for(;;) usleep(5000);

  fclose(file);
  return 0;
}
