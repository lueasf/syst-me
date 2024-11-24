#include <stdio.h>
#include <fcntl.h> // O_RDONLY pour ouvrir un fichier en lecture seule
#include <errno.h>


// 1 - tableau d'affichage à remplir.
int main(){
    int fd1, fd2;
    fd1 = open("toto.txt", O_RDONLY, 0); 
    close(fd1);
    fd2= open("titi.txt", O_RDONLY, 0);
    printf("fd2 = %d (errno=%d)\n", fd2, errno);
    return 0;
}

// toto.txt 	titi.txt	   fd2	errno
// Oui	        Oui	            3	   0
// Oui	        Non	            -1	ENOENT (2 : No such file or directory)
// Non	        Oui	            3	   0
// Non	        Non	            -1	ENOENT 


// 2 - Il y a un programme, dire ce qu'il affiche.
// Il y a ecrit "LAMBDA" dans le fichier toto.txt
int main() {
    int fd1, fd2; char c;
    fd1=open("toto.txt", O_RDONLY, 0);
    fd2=open("toto.txt", O_RDONLY, 0);
    read(fd1, &c, 1); // lit le premier octet (ici L)
    read(fd2, &c, 1);
    printf("c = %c\n", c);
    return 0;
}
// Le programme renvoie c = L car meme si les deux
// descripteurs sont ouverts sur le même fichier, ils
// ont chacun leur propre pointeur de lecture.

// 3 - meme question
int main(){
    int fd; char c;
    fd=open("toto.txt", O_RDONLY, 0);
    if (fork() == 0) {
        read(fd, &c, 1);
        return 0;
    }
    wait(NULL);
    read(fd, &c, 1);
    printf("c = %c\n", c);
    return 0;
}
// c = A, car le desc de fichier est paratagé entre le processus parent
// et l'enfant après le fork().

// 3 - meme question
#include <fcntl.h> /* O_RDONLY */
int main(){
    int fd1, fd2; char c;
    fd1=open("toto.txt", O_RDONLY, 0);
    read(fd1, &c, 1);
    fd2 = dup(fd1);
    read(fd2, &c, 1);
    printf("c = %c\n", c);
    return 0;
}
// c = A, car le descripteur de fichier dupliqué pointe sur le même fichier
// En effet, la fonction dup() crée une copie du desc fd1, mais les deux
// descripteurs pointent sur le même fichier.

// 4 - Quels sont les appels syst`emes faits par le shell
// lorsque l’on tape cat < toto ?
int rep(){
    int fd;
    if ( ! fork()) { // s'exec que dans le fils
        fd=open("toto.txt",O_RDONLY,0);  // return un descr de fichier (fd) qui pointe vers le fichier ouver
        dup2(fd,0) ; // redirige l'entrée standard vers le fich ouvert par fd
        close(fd); // ferme le fichier
        execlp("cat","cat",NULL); // remplace le fils par le programme cat
    }
}
// en effet, à la base, les progs lisent sur stdin depuis le clavier.
// cat text.txt, lit le fichier text.txt et affiche son contenu sur stdout
// ici dup2(fd,0) redirige l'entrée standard vers le fichier toto.txt
// c'est à dire que cat lit depuis stdin, en fait il lit le CONTENU de toto.txt.

// 5 - cat toto : ici cat oivre directement le fichier lui-même via
// un appel système open() et lit son contenu via read().


// 6 - implémenter popen() qui permet de lire le résultat d'une commande shell
// ex de popen : FILE *f = popen("ls -l", "r"); // ouvre un pipe pour lire le résultat de ls -l
int mypopen(char *cmd){
    int fd[2];
    // fd[0] : Desc de fich pr la lecture.
    // fd[1] : Desc de fich pr l'écriture. 
    // permet d'envoyer des data du proc fils au proc père via le pipe.
    pipe(fd); 
    if (fork() > 0) { /* père */
        close(fd[1]);
        return (fd[0]); 
        // Le père retourne le desc de lecture, ça permet au parent d’accéder aux data envoyées par le fils.
    } else { /* fils */
        close(fd[0]);
        dup2(fd[1],1);
        close(fd[1]);
        execlp("sh","sh","-c",cmd,NULL); /* on utilise sh pour parser argv */
    }
}