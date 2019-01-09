#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
// Author Patrick Keleher


int main(int argc, char const *argv[]) {
 int command1= atoi(argv[0]);
 int command2= atoi(argv[1]);
 int command3= atoi(argv[1]);

  printf("Checker process [%d]: Starting.\n", getpid());

  if((command2 % command1) ==0){
       printf("Checker process [%d]: %d *IS* divisible by %d.\n", getpid(),command2,command1);
       printf("Checker process %d: Returning 1.\n",getpid());
       return 1;
    }


    else{
        printf("Checker process [%d]: %d *IS NOT* divisible by %d.\n", getpid(), command2,command1);
         printf("Checker process %d: Returning 0.\n",getpid());
        return 0;
    }


  return 0;
}
