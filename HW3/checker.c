#include <unistd.h>
#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
// Build off the last version
// Author Patrick Keleher
int main(int argc, char *argv[]){
	int share_memory_id;
int command1= atoi(argv[0]);
	int command2= atoi(argv[1]);
	int command3 = atoi(argv[2]);
	printf("Checker process [%d]: starting.\n", getpid());

	read(command3, &share_memory_id, sizeof(int));
	close(command3);

	printf("Checker process [%d]: read %d bytes containing shm ID %d\n", getpid(), (int) sizeof(share_memory_id), share_memory_id);

	int *shared_ptr = shmat(share_memory_id, NULL, 0);

	if (shared_ptr == (void *) -1){
		perror("BAD MEMORY");
        return 1;
    }

	if ( command2 % command1 == 0 ){
		printf("Checker process [%d]: %d *IS* divisible by %d.\n", getpid(), command2, command1);
		memset(shared_ptr, 1, sizeof(int));
		printf("Checker process [%d]: wrote result (1) to shared memory.\n", getpid());
	}
	else {
		printf("Checker process [%d]: %d *IS NOT* divisible by %d.\n", getpid(), command2, command1);
		memset(shared_ptr, 0, sizeof(int));
		printf("Checker process [%d]: wrote result (0) to shared memory.\n", getpid());
	}

	int sharereturn = shmdt(shared_ptr);

	if (sharereturn == -1){
		perror("BAD RETURN");
        return 1;
    }

	return 0;
}
