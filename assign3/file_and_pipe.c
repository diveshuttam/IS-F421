//Divesh Uttamchandani
//2016A7PS0045P
//Network Programming assignment 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#define PIPE_BUF 4096

int N;
void pipe_solve(int n, char * data){
  while(n>1){
    int p[2];
    pipe(p);
    pid_t ret=fork();
    int status;
    if(ret>0) //inside parent
    {
      close(p[0]);//close unused read end
      for(long long i=0;i<1024*1024*1024;i+=PIPE_BUF){
        printf("process %d writing to semaphore\n",N-n+1);
        fflush(stdout);
        write(p[1],data+i,PIPE_BUF);
      }
      free(data);
      wait(&status);
      exit(0);
    }
    if(ret==0) //inside child
    {
      n--;
      close(p[1]);//close unused write end
      //though inintially though of reading in data1 but the program goes
      //of memory because of this
      //char* data1=malloc(sizeof(char)*1024*1024*1024);
      for(long long i=0;i<1024*1024*1024; i+=PIPE_BUF){
        printf("process %d reading from semaphore\n", N-n+1);
        fflush(stdout);
        //reading in paralle1 memory locations data in next process
        //not affect the original memory as copy on write
        read(p[0],data+i,PIPE_BUF);
        //read(p[0],data1+i,PIPE_BUF);
        //data=data1;
      }
    }
  }
}

void parent_handler(int dummy){//sigusr2 goes to parent
  fflush(stdout);
}

void child_handler(int dummy){//sigusr1 goes to child
  fflush(stdout);
}

void file_solve(int n, char *data){
  //syncronizinig io using signals
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
  while(n>1){
    int status;
    pid_t ret=fork();
    if(ret>0) //inside parent
    {
      char filename[15];
      sprintf(filename,"tmp%d.txt",getpid());
      FILE *tmp=fopen(filename, "w");
      printf("in process %d, writing to file\n",N-n+1);
      fflush(stdout);
      fprintf(tmp,"%s",data);
      fclose(tmp);
      sigprocmask(SIG_BLOCK, &maskboth, NULL); //block both the signals don't save prev
      kill(ret, SIGUSR2);
      sigsuspend(&masknone);
      printf("child of %d finished reading, deleting file\n", N-n+1);
      fflush(stdout);
      remove(filename);
      wait(&status);
      exit(0);
    }

    if(ret==0) //inside child
    {
      sigsuspend(&masknone);
      char filename[15];
      sprintf(filename,"tmp%d.txt",getppid());
      FILE *tmp=fopen(filename, "r");
      n--;
      //though inintially though of reading in data1 but the program goes
      //of memory because of this
      //char* data1=malloc(sizeof(char)*1024*1024*1024);
      printf("in process %d, reading from file\n", N-n+1);
      fflush(stdout);
      fscanf(tmp,"%s",data);
      fclose(tmp);
      sigprocmask(SIG_BLOCK, &maskboth, NULL); //block both the signals don't save prev
      kill(getppid(), SIGUSR1);
      //sigsuspend(&masknone);
    }
  }
}

void solve(int n){
  printf("generating 1GB data\n");
  fflush(stdout);
  char* data=malloc(sizeof(char)*1024*1024*1024);
  long long int i=0;
  for(i=0;i<1024*1024*1024-1;i++){
    data[i]='a';
  }
  data[i]='\0';

  printf("starting data transfer through pipe\n");
  fflush(stdout);
  time_t t=time(NULL);
  pipe_solve(n,data);
  time_t t1=time(NULL);
  printf("time elapsed %d\n",t1-t);
  fflush(stdout);

  printf("starting data transfer through files\n");
  fflush(stdout);
  t=time(NULL);
  file_solve(n,data);
  t1=time(NULL);
  printf("time elapsed %d\n",t1-t);
  fflush(stdout);
 fflush(stdout);
}

int main (int argc, char* argv[])
{
  if(argc!=2){
    printf("invalid number of arguments\n");
    exit(1);
  }
  N=atoi(argv[1]);
  solve(N);
}
