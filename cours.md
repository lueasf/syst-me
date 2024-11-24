# Programmation système

## Fork
La commande fork() crée un processus "fils" du processus appelant, le "père",
avec le même programme que le "père".
La valeur renvoyée par fork() est:
- le pid du processus fils pour le processus père
- 0 pour le processus fils 
On peut retrouver le pid du processus père avec la commande getppid().
En cas d'échec, aucun processus n'est créée et la valeur -1 est renvoyée.

>Le fils fait une copie exacte du parent au moment du fork, y compris les variables, mais il ne rééxécute pas le code avant le fork. Le père reprend l'exécution après le fork et le fils commence. Le code du fils, c'est comme le code du père mais après le fork. 

>Les variables du fils sont des copies des variables du père au moment du fork, dans des nouveaux emplacements mémoire.

>Si dans un programme, il y a n fork, il y a 2^n processus créés.

```
int main(){
    fork();
    fork();
    printf("Hello\n");
    exit(0);
}
``` 
affiche la même chose que le programme suivant:
``` 
int main(){
    int i;
    for (i=0; i<2; i++){
        fork();
    }
    printf("Hello\n");
    exit(0);
}
``` 

Tout processus dont le père est mort sont adoptés par le processus init (pid = 1).

## Boucle for et while
>Lorsque fork est executé dans une boucle for, le processus hérite de l'état de la boucle dans laquelle le parent était au moment du fork. 

Ex: si la parent était à l'itération i = 1 dans la boucle for, le fils commencera aussi à la deuxième itération de la boucle for, c'est à dire i = 1.

## Erreurs
la librairie errno.h contient la variable errno qui est une variable globale qui contient le code d'erreur de la dernière fonction qui a échoué. 



# EXEC
La famille de primitives **exec** permet de créer un processus pour éxécuter un programme déterminé.