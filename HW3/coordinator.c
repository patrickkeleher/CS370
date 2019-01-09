#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdio.h>
// Built off the last version
// Author Patrick Keleher

int main(int argc, char* argv[]){
	int share_memory_id[4];
	int pipes[2];
	char pipeArg[50];
	int cID[4];
  int command1 = atoi(argv[1]);
 int i=2;
	while(i < argc){

		if(pipe(pipes) == -1){
            perror("pipe");
            return 1;
        }

		share_memory_id[i-2] = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
		sprintf(pipeArg, "%d", pipes[0]);

		if ((cID[i-2] = fork()) == 0){
			close(pipes[1]);
			execlp("./checker", argv[1], argv[i], pipeArg, NULL);
		}
		else {
			printf("Coordinator: forked process with ID %d.\n", cID[i-2]);
			close(pipes[0]);
			write(pipes[1], &share_memory_id[i-2], sizeof(int));
			close(pipes[1]);
			printf("Coordinator: wrote shm ID %d to pipe (%d bytes)\n", share_memory_id[i-2], (int) sizeof(share_memory_id[i-2]));
		}
		i++;
	}
       int j=0;
	while(j<4){
		printf("Coordinator: waiting on child process ID %d...\n", cID[j]);
		waitpid(cID[j], NULL, 0);

		int *shared_ptr = shmat(share_memory_id[j], NULL, 0);

		if (shared_ptr == (void *) -1){
            perror("BAD CALL TO MEMORY");
            return 1;
        }

		int shared_value;

		memcpy(&shared_value, shared_ptr, 1);

		int shmdt_return = shmdt(shared_ptr);

		if (shmdt_return == -1){
			perror("BAD RETURN");
            return 1;
        }

        if (shmctl(share_memory_id[j], IPC_RMID, NULL) == -1){
            perror("BAD CALL");
            return 1;
        }
     int commandx= atoi(argv[j+2]);
		if (shared_value == 0){
			printf("Coordinator: result %d read from shared memory: %d is not divisible by %d.\n", shared_value, commandx, command1);
        }
		else{
			printf("Coordinator: result %d read from shared memory: %d is divisible by %d.\n", shared_value, commandx, command1);
        }
  j++;
	}

	printf("Coordinator: exiting.\n");

	return 0;
}
