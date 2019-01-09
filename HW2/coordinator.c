#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Author Patrick Keleher

 int main(int argc, char const *argv[]) {
    int pidcheck;
    int control;
    int i =2;

    while(i < argc){
        if((pidcheck = fork()) ==0){
            execlp("checker", argv[1], argv[i],NULL);
        }
            printf("Coordinator: forked process with ID %d.\n" ,pidcheck);
            printf("Coordinator: waiting for process [%d].\n" ,pidcheck);


            wait(&control);


            if(WIFEXITED(control)){
                printf("Coordinator: child process %d returned %d.\n", pidcheck,WEXITSTATUS(control));

        }
        i++;
    }

 printf("Coordinator: exiting.\n");
   return 0;
 }
