# Programmation système

# Introduction
Wikipedia : 1145 serveurs, 30k CPU
OVH : 250k serveurs

# OS
Un **système d'exploitation** est un logiciel intermédiaire entre les apps et le matériel. Il fournit une interface standardisée pour les prog et protège les ressources (processeur, mémoire, périphériques) des accès nn autorisés.

L'OS fournit deux interfaces :
- **API (Application Programming Interface)** : interface de programmation permettant aux applications d'interagir avec l'OS via des appels système (comme `read()` et `write()`).
- **Interface de commande** : interface utilisateur sous forme de commande (ex. `ls`, `cp` en UNIX) ou graphique.
 
Le shell (coquille) englobe l'OS.
tty : teletype désigne des terminaux virtuels sur linux.

# POSIX
**UNIX** est une famille de systèmes d'exploitation reconnue pour sa simplicité et son code source libre, avec une standardisation assurée par **POSIX** pour garantir la compatibilité entre systèmes UNIX.

# Fichiers
Dans l'API, un fichier est représenté par un descripteur de fichier, un entier positif (petits entiers, ds des tables du noyau). De plus, chaque fichiers sur disque est associé à un inode, qui contient les métadonnées du fichier (taille, propriétaire, permissions, ...).
3 FONCTIONS : 
`read()`, `lseek()`, `write()` pour manipuller les fichiers
`dup()`, `dup2()` pour dupliquer des descripteurs de fichiers
rappel, les descripteurs 0,1 et 2 sont pris par défaut pour stdin, stdout et stderr.
Meme si les descripteurs décrivent les memes fichiers, ils ont des pointeurs de lecture/écriture différents.
Le descripteur de fichier est partagé entre le processus parent et l’enfant après le fork(), ce qui entraine un 
décalage de pointeur après lecturee par le fils ou père. 
La fonction dup(fd1) ne crée pas un nouveau pointeur de lecture. Elle crée un nouveau descripteur qui partage le même pointeur de lecture que l'original.(TD 2)

la différence enter `dup()` et `dup2()` est que `dup2()` prend un deuxième argument qui est le descripteur de fichier que l'on veut utiliser. Si le descripteur est déjà utilisé, il est fermé avant d'être utilisé.

| Commande       | Qui ouvre le fichier ?       | Où `cat` lit-il ?       |
|----------------|------------------------------|-------------------------|
| `cat toto`     | `cat` ouvre direct le fich.  | Depuis le fich `toto`.  |
| `cat < toto`   | Le **shell** ouvre le fich et redirige `stdin`. | Depuis `stdin` (redirigé vers `toto`). |


# Processus
def : l'entité dynamique qui représente l'exec d'un prog sur un processeur.
!= programme (code + données => passif) tandis qu'un processus est
un programme en cours d'exec.
On peut uliliser le mm prog mais pas le mm processus

Les processus communiquent via des mécanismes d'**IPC (Inter-Process Communication)** : 
- **Signaux** : messages asynchrones pour contrôler les processus.
- **Pipes** et **fichiers** : pour le transfert de données.
- **Mémoire partagée** et **sémaphores** : ressources partagées pour la coordination.

le contexte d'un processeur est l'ens des données qui le caractérise permettant
de reprendre son exec si interrompu.
-context utilisateur 
-contexte materiel : a l'instant donné, on a quoi dans les registres.
-contexte système : les sd du sys pr la realisation du processus

on peut pas savoir si c'est le fils ou le père qui s'exec en premier.

pseudo-parallélisme = chacun son tour un petit peu de processus
certains processus ne se terminent pas avant l'arret de la machine (comme
le daemons/démons : disk and exec monitor.)

Les processus peuvent être dans différents états :
- **Actif** : en exécution.
- **Bloqué** : en attente d'une ressource.
- **Suspendu** : mis en pause volontairement.

Un **processus zombie** est un processus terminé dont le parent n'a pas encore récupéré le code de fin, ce qui consomme des ressources.

# Threads
Un thread est une unité d'execution à l'intérieur d'un processus. un proc peut etre
divisé en pls threads, qui partagent la mm memoire et les mm ressources, mais
qui peuvent exec différentes parties du prog en parallèle.

# Coeur
Un coeur est une partie physique du processeur (CPU)
qui exec des threads.

Chaque cœur peut gérer 2 threads dans les processeurs modernes, 
donc un processeur à 8 cœurs peut gérer 16 threads.

L'hyperthreading est une technologie qui permet à un seul cœur de traiter
deux threads en même temps.

Une primitive ou appel système est une fonction qui permet à un programme
d'interagir avec le noyau du système d'exploitation.
Ex : `fork()`, `getpid()`, ...

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

## Pipes
pipe() crée un canal de communication unidirectionnel entre deux processus (généralement un processus parent et un processus enfant).
C'est comme un tube virtuel.
Les deux extrémités du pipe sont représentées par deux descripteurs de fichier :

    fd[0] : pour lire.
    fd[1] : pour écrire.


Création du pipe :

    Le parent crée un pipe(fd) :
        fd[0] est pour lire.
        fd[1] est pour écrire.

Processus enfant :

    Ferme fd[0] (il n'a pas besoin de lire).
    Écrit ses données dans fd[1].

Processus parent :

    Ferme fd[1] (il n'a pas besoin d'écrire).
    Lit les données envoyées par l'enfant à partir de fd[0].

## Autres fonctions
> Yield :  Lorsqu'un thread appelle yield, il indique au système d'exploitation qu'il est prêt à laisser d'autres threads s'exécuter.
getpid et getppid pour le père
getuid pour obtenir le numéro d'usager (auquel appartient le processus)
pause() bloquer jusqua reception du signal
sleep() stop n secondes
> waitpid()

>popen c'est comme un open avec pipe.

## Erreurs
la librairie errno.h contient la variable errno qui est une variable globale qui contient le code d'erreur de la dernière fonction qui a échoué. 
0 : tout est ok
2 :ENOENT (No such file or directory).

## EXEC
La famille de primitives **exec** permet de créer un processus pour éxécuter un programme déterminé.

# Signaux 
Les **signaux** sont des messages asynchrones pour contrôler les processus, pouvant survenir
à tout moment. Ils sont envoyés par le noyau ou par un autre processus.:
- **SIGINT** : interrompt un processus (ex. : <Ctrl+C>).
- **SIGTERM** : demande de terminaison.
- **SIGKILL** : termine immédiatement un processus (non interceptable).
 
Les signaux facilitent la synchronisation entre processus, permettant, par exemple, à un processus 
fils de signaler au parent la fin de son exécution pour libérer les ressources.

