//Author- Divesh Uttamchandani
//Network Programming assignment 2
//2016A7PS045P

#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

int totalN;
void parent_handler(int dummy){//sigusr2 goes to parent
  fflush(stdout);
}

void child_handler(int dummy){//sigusr1 goes to child
  fflush(stdout);
}

void gen(int N){
  //creating masking set once
  sigset_t mask1, mask2, maskboth, masknone;
  sigemptyset(&masknone);
  sigemptyset(&mask1);
  sigemptyset(&mask2);
  sigemptyset(&maskboth);
  sigaddset(&mask1, SIGUSR1);
  sigaddset(&mask2, SIGUSR2);
  sigaddset(&maskboth, SIGUSR1);
  sigaddset(&maskboth, SIGUSR2);

  signal(SIGUSR2, parent_handler);
  signal(SIGUSR1, child_handler);

  sigprocmask(SIG_BLOCK, &maskboth, NULL); //block both the signals don't save prev

  if(N==1){
    for(int i=0;i<1000;i++){
      sigsuspend(&masknone);
      printf("%d\n", getpid());
      sigprocmask(SIG_BLOCK, &maskboth, NULL); //block both the signals don't save prev
      kill(getppid(), SIGUSR2);
    }
    return;
  }

  pid_t pid = fork();

  if (pid<0){
    perror("fork:");
  }
  else if(pid==0){//in child
    if(N>=2)
      gen(N-1);
  }
  else if(pid>0){ //in parent
    for(int i=0;i<1000;i++){
      if(N!=totalN)
      {
        sigprocmask(SIG_BLOCK, &maskboth, NULL); //block both the signals don't save prev
        sigsuspend(&masknone);
      }

      if(N==totalN)
        printf("%d\t",i+1);
      printf("%d\t", getpid());
      fflush(stdout);

      sigprocmask(SIG_BLOCK, &maskboth, NULL); //block both the signals don't save prev
      kill(pid, SIGUSR1);
      sigsuspend(&masknone);

      if(N!=totalN){
        sigprocmask(SIG_BLOCK, &maskboth, NULL); //block both the signals don't save prev
        kill(getppid(), SIGUSR2);
      }
    }
  }
}

int main(int argc, char *argv[]){
  if(argc!=2)
  {
    printf("invalid no of arguments");
    return 0;
  }
  int N=atoi(argv[1]);

  if(N<1){
    printf("invalid N\n");
    return -1;
  }

  if(N==1){ //special case n=1 as no child and parent signalling
    for(int i=0;i<1000;i++){
      printf("%d %d\n",i+1, getpid());
    }
    return 0;
  }

  totalN = N;
  gen(N);
  return 0;
}
