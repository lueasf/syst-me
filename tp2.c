#include <stdlib.h>
#include <errno.h> 
#include <stdio.h>
#include <unistd.h> // fork et autres apels système POSIX
#include <sys/wait.h> // wait
#include <signal.h> // signal

// 1 - créer des pipes
void pipex(){
    int fd[2];
    char message[] = "Bonjour papa !";
    char buf[20]; 

    pipe(fd);
    write(fd[1], message, sizeof(message));
    read(fd[0], buf, sizeof(message));
    printf("buffer = %s\n", buf);
    close(fd[0]);
    close(fd[1]);
}

// 2 - L'OS attache une zone de stockage à chaque tube. 
// Determinons sa taille

void pipex2(int argc, char *argv[]) {
    char buff[102400];

    int fd[2];
    long int ecrit;
    long int n=atoi(argv[1]);
    pipe(fd);
    ecrit = write(fd[1],buff,n);
    printf("J’ai ´ecrit %lu octets (de %lu, manque %lu)\n",ecrit,n,n-ecrit);
}
/*Explication : 
Le pipe a une capacité limitée. Sur certains sys, la taille max du buff est de 65536 octets.
on lance la commande : gcc ex2.c && ./a.out 20000
*/


// 3 - émettre des mails depuis des programmes
// la commande : echo coucou | login@localhost 
int mail1() {
    int p[2];
    pipe(p);
    if (fork() != 0) {
        int date = time(NULL);
        write(p[1],"coucou", 7);
        close(p[0]); close(p[1]);
    } else { /* Rediriger l’entree sur le pipe et lancer mail */
    dup2(p[0], 0);
    close(p[0]); 
    close(p[1]);
    execlp("mail", "mail","mquinson@localhost", NULL);
    }
    wait(NULL);
    fprintf(stderr,"Message parti\n");
}

// 4 - mettre une alarme pr que le prog envoie un msg ttes les 5s ?
/* 
struct sigaction sig,old; // struct de signaux
sig.sa_handler = &mail;  // mail est appelé à chaque signal
sigaction(SIGALRM,&sig,&old);  // associe le sig a la fonction mail
while (1) {  // prgm infini
    alarm(5);
    pause();
}
*/


// 5 - réimplémenter popen

int main(){
    pipex();
    return 0;
}