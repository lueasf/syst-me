#include <stdlib.h>
#include <errno.h> 
#include <stdio.h>
#include <unistd.h> // fork
#include <sys/wait.h> // wait
#include <signal.h> // signal 
#include <string.h> // memset

// 1 - Programme qui crée 10 processus fils.
int ten(){
    for (int i = 0; i< 10; i++){
        if (fork()==0){
            for (int j = 0; j<10; j++){
                printf("fils %d , pid : %d.\n", i, getpid());
            }
            exit(0);
        }
    }
}



// 2 - écrire un prog qui exec 5 fois au plus une 
// commande Unix qu'on lui passe en paramètre
int main1(int argc, char* argv[]){
    int i, status; 
    if (argc < 2) {exit(1);}
    for (i = 0; i<5; i ++){
        if (fork() == 0){
            execvp(argv[1], argv+1);
        } else{
            // si une erreur survient pendant que le parent attend la fin
            // de l'enfant, on l'affiche. WEIXTSTATUS(status) renvoie le code 
            // de retour de l'enfant.
            if (waitpid(-1, &status, 0) == -1){
                printf("erreur du fils : %d\n", WEXITSTATUS(status));
                exit(1);
            }
        }
    }
    return 0;
}


// 3 - Écrire un programme qui ne se termine qu'au 5eme Ctrl-C
volatile sig_atomic_t count = 0; // volatile indique que la valeur de count peut être modifiée par un signal

void handler(int sig){
    count ++;
    if (count == 5){
        exit(0);
    }
}

// int main(){
//     struct sigaction nvt, old;
//     memset(&nvt, 0, sizeof(nvt));
//     nvt.sa_handler = handler;
//     sigaction(SIGINT, &nvt, &old);
//     while (1);
// }
