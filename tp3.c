// POSIX
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> // pour pthread_create, pthread_join ...
#include <sys/types.h>
#include <unistd.h>

// 1 et 2 - Écrire un prog ou des th sont créés, affichent dson pid et le th principal attend leur fin.

// cette fonction est appelée par le thread. pthread_self() retourne l'identifiant du thread.
void * helloworld(void*  arg){
    printf("Yo, pid : %d, pthread_self= %p\n", getpid(), (void*) pthread_self());
    return NULL;
}

int main(int argc, char ** argv){
    int i, nb;
    nb = atoi(argv[1]); // arg1 est le nombre de threads à créer
    pthread_t * threads = malloc(nb * sizeof(pthread_t));

    for (i = 0; i < nb; i++){
        pthread_create(&threads[i], NULL, helloworld, NULL); 
        // (adresse du thread, , fonction que le th exec, arg passé à la fnc du th)
    }
    for (i = 0; i < nb; i++){
        pthread_join(threads[i], NULL); // attend la fin d'un th i
    }
    free(threads);
    return 0;
}
// ./tp3 2
// Yo, pid : 141074, pthread_self= 0x7959cfc006c0
// Yo, pid : 141074, pthread_self= 0x7959cf2006c0
// Ils ont le meme pid car ce sont des threads du meme processus.
// Ils partagent le meme espaces mémoire.



// 3 - modifier le prgm pour que chaque thread affiche un numéro unique.
// correspondant à l'ordre de création du thread.
// vérifier que le th affiche bien le num, meme si l'exec des th est parallèle.
 
// -> il suffit de rajouter un tableau d'int agrs de taille nb.



// 4 - Chaque th doit dans une boucle, ajouter 1 000 000 son numéro à une variable globale.
// -> Pour cela on utilise une variable globale et on la protège avec un mutex.
// un mutex est un verrou qui permet de protéger une ressource partagée.
pthread_mutex_t mutex;
int somme_glob = 0;

void * helloworld2(void * arg) {
    int ordre = *(int *) arg; // Récupère le numéro d'ordre du thread
    for (int i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&mutex); // Bloque l'accès à la section critique
        somme_glob += ordre; 
        pthread_mutex_unlock(&mutex); // Libère l'accès à la section critique
    }
    return NULL;
}

int main(int argc, char** argv) {
    int i, nb;
    nb = atoi(argv[1]);
    pthread_t * threads = malloc(nb * sizeof(pthread_t));
    int * args = malloc(nb * sizeof(int));

    pthread_mutex_init(&mutex, NULL); // avant la créa des th

    for (i = 0; i < nb; i++) {
        args[i] = i; // Stocke le numéro d'ordre pour chaque thread
        pthread_create(&threads[i], NULL, helloworld, &args[i]);
    }
    for (i = 0; i < nb; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Somme globale : %d\n", somme_glob);

}



// 5 - Modifier le progm pour ne plus utiliser de variable globale.
struct threadargs {
    int i;                  // Numéro unique du thread (indice)
    int *somme;             // Pointeur vers la variable partagée (somme)
    pthread_mutex_t *mutex; // Pointeur vers le mutex pour protéger la section critique
};

void *helloworld(void *arg) {
    struct threadargs *ta = (struct threadargs *) arg; // Cast de l'argument
    int i;

    for (i = 0; i < 1000000; i++) { // Chaque thread effectue 1 000 000 incréments
        pthread_mutex_lock(ta->mutex);    // Verrouillage du mutex
        *ta->somme += ta->i;             // Modifie la variable partagée
        pthread_mutex_unlock(ta->mutex); // Libération du mutex
    }

    return NULL;
}

int main(int argc, char** argv) {
    int i, nb;
    struct threadargs *args;
    int somme = 0;
    pthread_mutex_t mutex;
    pthread_t *threads;

    nb = atoi(argv[1]);
    threads = malloc(nb * sizeof(pthread_t));
    args = malloc(nb * sizeof(struct threadargs));
    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < nb; i++) {
        args[i].i = i;
        args[i].somme = &somme; // Pointeur vers la variable partagée
        args[i].mutex = &mutex; // Pointeur vers le mutex
        pthread_create(&threads[i], NULL, helloworld, &args[i]);
    }

    for (i = 0; i < nb; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("somme=%d\n", somme);
    return 0;
}