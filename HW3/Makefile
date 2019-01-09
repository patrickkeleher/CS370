EXEC1= coordinator
EXEC2= checker
GCC = gcc
GCC_FLAGS = -std=c11 -g -Wall
all: coordinator checker
coordinator: coordinator.o
	$(GCC) $(GCC_FLAGS) coordinator.o -o $(EXEC1)
checker: checker.o
	$(GCC) $(GCC_FLAGS) checker.o -o $(EXEC2)
clean:
	rm -f *.o *~ $(EXEC1) $(EXEC2)
