#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> // fork
#include <sys/wait.h> // wait

// 1 - Programme qui crée 1à processus fils.
int main(){
    for (int i = 0; i< 10; i++){
        if (fork()==0){
            for (int j = 0; j<10; j++){
                printf("fils %d , pid : %d.\n", i, getpid());
            }
            exit(0);
        }
    }
}