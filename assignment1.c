#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    /* variables used to identify the different processes */ 
    pid_t pid_1, pid_2, pid_3;

    printf("parent (PID %d): process started\n", getpid());

    /* creates child 1 */
    printf("parent (PID %d): forking child_1\n", getpid());
    pid_1 = fork();

    /* checks if the fork was successful and forces the parent process to wait until child_1 terminates */
    if(pid_1 > 0){
        printf("parent (PID %d): fork successful for child_1 (PID %d)\n", getpid(), pid_1);
        printf("parent (PID %d): waiting for child_1 (PID %d) to complete\n", getpid(), pid_1);
        wait(NULL);
    }

    /* checks that child_1 has started and forks child_1.1 from child_1 */
    else if(pid_1 == 0){
        printf("child_1 (PID %d): process started from parent (PID %d)\n", getpid(), getppid());
        printf("child_1 (PID %d): forking child_1.1\n", getpid());
        pid_2 = fork();
        wait(NULL);
    }

    /* forks child_2 from parent (after child_1 terminates) */
    if(pid_1 > 0){
        printf("parent (PID %d): forking child_2\n", getpid());
        pid_2 = fork();

        /* checks that the fork was successful and forces the parent to wait until child_2 terminates (if necessary) */
        if(pid_1 > 0 && pid_2 > 0){
            printf("parent (PID %d): fork successful for child_2 (PID %d)\n", getpid(), pid_2);
            wait(NULL);
        }

        /* checks that child_2 is running and makes it execute external program 2 with command-line argument S */
        else if(pid_1 > 0 && pid_2 == 0){
            printf("child_2 (PID %d): process started from parent (PID %d)\n", getpid(), getppid());
            printf("child_2 (PID %d): calling an external program [./external_program2.out]\n", getpid());
            execl("./external_program2.out", argv[3], NULL);
        }
    }

    /* checks that child_1.1 is running and makes it execute external program 1 with command-line argument N */
    if(pid_1 == 0 && pid_2 == 0){
        printf("child_1.1 (PID %d): process started from child_1 (PID %d)\n", getpid(), getppid());
        printf("child_1.1 (PID %d): calling an external program [./external_program1.out]\n", getpid());
        execl("./external_program1.out", argv[1], NULL);
    }

     /* forks child_1.2 from child_1 (after child_1.1 terminates)
        also forces child 1 to wait until child 1.2 terminates before terminating */
    else if(pid_1 == 0 && pid_2 > 0){
        printf("child_1 (PID %d): completed child_1.1\n", getpid());
        printf("child_1 (PID %d): forking child_1.2\n", getpid());
        pid_3 = fork();
        wait(NULL);
    }

    /* checks that child_1.2 is running and makes it execute external program 1 with command-line argument M */
    if(pid_1 == 0 && pid_2 > 0 && pid_3 == 0){
        printf("child_1.2 (PID %d): process started from child_1 (PID %d)\n", getpid(), getppid());
        printf("child_1.2 (PID %d): calling an external program [./external_program1.out]\n", getpid());
        execl("./external_program1.out", argv[2], NULL);
    }

    /* checks that child_1.2 has terminated */
    else if(pid_1 == 0 && pid_2 > 0 && pid_3 > 0){
        printf("child_1 (PID %d): completed child_1.2\n", getpid());
    }

    /* checks that parent will terminate as expected */
    if(pid_1 > 0 && pid_2 > 0){
        printf("parent (PID %d): completed parent\n", getpid());
    }
}
