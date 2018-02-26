#include<signal.h>
#include<stdio.h>



int main(int argc, int argv[]){
  if(argc != 2){
    printf("error in number of arguments");
    return -1;
  }

  for(int i=0; i<1000; i++){
    printf("%d", getpid());
    
  }

}
