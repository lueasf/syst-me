#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> // fork

// 1 -
// réaliser une chaine de processus comme suit:
// un père qui crée n fils à la suite
// en attendant leur fin et sans que ceux-ci n'en créent d'autres.

void chaine(int n){
    for (int i=0; i<n; i++){
        if (fork==0){
            printf("Je suis le fils %d de %d", getpid(), getppid());
            exit(0); // pour éviter que les fils ne créent d'autres fils
        } else {
            wait(NULL); // pour attendre la fin de chaque fils
        }
    }
}

// 2 -
// réaliser une chaine de processus comme suit:
// un père crée un fils, qui crée un fils, qui crée un fils, etc. n fois

void arbre(int n){
    for (int i=0; i<n; i++){
        if (fork()==0){
            printf("Je suis le fils %d de %d", getpid(), getppid()); 
        } else {
            exit(0); // dès qu'un parent crée un fils, il se termine
        }
    }
}

// 3 -
// écrire un programme qui execute une commande Unix qu'on lui passe en argument
 
int main(int argc, char * argv[]){
    if (argc < 2){
        printf("Au moins 1 args\n");
        exit(1);
    }
    execvp(argv[1],argv+1); // car argv[0] est le nom du programme et donc argv[1] est le premier argument
    // ici la commande à exec. argv+1 est un pointeur sur le premier argument
    perror("echec");
    return 1;
}