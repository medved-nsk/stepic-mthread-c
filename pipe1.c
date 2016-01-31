#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void whowc()
{

}

void fork_last(int fdp[2], char *cmd, char **args)
{
  if (!fork()) {
  close(STDIN_FILENO);
  dup2(fdp[0], STDIN_FILENO);
  close(fdp[1]);
  close(fdp[0]);
  execvp(cmd, args);
  return;
  } else { }
}

void fork_mid(int fdp[2], char *cmd, char **args)
{
  if (!fork()) {
    int *pfc = fdp;
    close(STDOUT_FILENO);
    dup2(fdp[1], STDOUT_FILENO);
    close(STDIN_FILENO);
    pfc+=2;
//    dup2(fdp[0], STDIN_FILENO);
    dup2(*pfc, STDIN_FILENO);
    close(*(pfc+1)); close(*pfc);
    execvp(cmd, args);
    return;
  } else { }
}

void fork_first(int fdp[2], char *cmd, char **args)
{
  if (!fork()) {
  close(STDOUT_FILENO);
  dup2(fdp[1], STDOUT_FILENO);
  close(fdp[1]);
  close(fdp[0]);
  execvp(cmd, args);
  return;
  } else { }
}

char * get_cmd(char *str, int s, int e)
{
  char *c, *cmd=NULL;
  for(c = str+s; c < str+e; c++) {
//    fprintf(stderr, "'%c' ", *c);
    if (*c != ' ' && cmd == NULL)
	{ cmd = c; continue; }
    if (*c == ' ' && cmd != NULL)
	{ *c = 0; break; } 
  } // for
  return cmd;
}

int main(int argc, char **argv)
{
#define MAX_LEN (1024)
#define MAX_CMDS (8)
  char buf[MAX_LEN];
  ssize_t n = read(0, &buf, MAX_LEN); // from STDIN
  if ( n == -1) { perror("read"); return(1);
	  } else printf("read ret: %d\n", n);

  buf[n] = 0;

  char *args[8]; bzero(args, sizeof(char *) * 8);
  printf("%s", buf);
//#define FPID ("/home/box/pid")

  char *cmd; // = buf;
  int i,j; char *c;
  int pfd[2];
  int PFD[8][2]; int *PFC;
  for(i=0; i < 8; i++) {
    pipe(&PFD[i][0]);
    fprintf(stderr, "pfd[%d]: %d %d\n", i, PFD[i][0], PFD[i][1]);
  }

  pipe(pfd);

  int numc = 1; // number of commands
  int ixs[MAX_CMDS]; bzero(ixs, sizeof(int) * MAX_CMDS);
  for(i=0, j=1; i < n; i++) if (buf[i] == '|') {
    numc++;
    buf[i] = ' '; // XX
    ixs[j++] = i;
  }
  ixs[j]=n; for(i=0; i <= j; i++)
  fprintf(stderr, "ixs[%d]=%d\n", i, ixs[i]);
/*
  char *args_who[] = {"wc", NULL };
  char *args_wc[] = {"wc", NULL };

  fork_last(PFD[0], "wc", args_wc);
  fork_first(PFD[0], "who", args_who);
*/
  PFC = PFD[0];
  cmd = NULL; cmd = get_cmd(buf, ixs[j-1], ixs[j]);
  fprintf(stderr, "cmd last: =%s=\n", cmd);
  args[0]=cmd;
//  fork_last(PFD[0], cmd, args); bzero(args, sizeof(char *) * 8);
  fork_last(PFC, cmd, args); bzero(args, sizeof(char *) * 8);
  j--;

// TODO: Pid pair!!
  if (j > 1) {
    cmd = NULL; cmd = get_cmd(buf, ixs[j-1], ixs[j]);
    args[0]=cmd;
    fprintf(stderr, "cmd %i: %s\n", j, cmd);
    fork_mid(PFC, cmd, args);
    j--;
    PFC++;
  }

  if (j = 1) { fprintf(stderr, "j=%d\n", j);
    cmd = NULL; cmd = get_cmd(buf, ixs[j-1], ixs[j]);
    args[0]=cmd;
    fprintf(stderr, "cmd 1st: %s\n", cmd);
//    fork_first(PFD[0], cmd, args);
    fork_first(PFC, cmd, args);
  }

  for(i=0, j=0; i < n; i++) {
	if ( ' ' == buf[i] ) {
		buf[i]=0;
	    if (buf[i+1] == '|') {
  			printf("going to execute: %s\n", cmd);

			if(!fork) { // child
		    close(STDOUT_FILENO);
		    
  			execvp(cmd, args);
	//		execlp(cmd, cmd, NULL);

			return 0;
		} else { // parent

		}
	    }
		break;
	}
  }
  return 0;
}


