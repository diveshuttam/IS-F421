//Divesh Uttamchandani
//2016A7PS0045P
//Network Programming Sem 2, 2017-18- Assignment 1

//the include directives
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/wait.h>

static int N;
void generate_tree(int n);

int main()
{
  int n;
  printf("Enter n:");
  scanf("%d",&n);
  N=n;
  printf("Level\t\tpid\t\tppid\t\tPosition\n");
  generate_tree(n); //generate_tree is NOT called recursively
                    //it only serves for factoring the logic out.
  return 0;
}

//generates the tree
void generate_tree(int n)
{
  int i,pos=0,parent_pos=0;
  for(i=0;i<n;i++)
  {
    pid_t pid=fork();
    switch(pid)
    {
      case -1:
        perror("fork");
        break;

      case 0: //in child
        parent_pos=pos;
        pos=i+1;
        n--;  //We want one less child at this level

        printf("%d\t\t%d\t\t%d\t\t%d\n",N-n,getpid(),getppid(),pos);

        i=-1;     //child will initially have same i as its number s

        continue; //i will become zero after this call. Still in loop so increment
                  //continues

      default : //in parent parent has nothing to print inside the loop
        break;
    }
  }

  if(n==0) //leaf node. this won't be a parrent. 
           //As per the question the parent should print "all childern exited"
  {
    //process has no children
    //not printing anything here
    exit(0);
  }

  int status;
  while ((wait(&status)) > 0); //wait for all child processes to terminate
  printf("\nAll children Exited\n");
}
