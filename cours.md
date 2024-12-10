# Programmation système


# Introduction
Wikipedia : 1145 serveurs, 30k CPU
OVH : 250k serveurs

# OS
Un **système d'exploitation** est un logiciel intermédiaire entre les apps et le matériel. Il fournit une interface standardisée pour les prog et protège les ressources (processeur, mémoire, périphériques) des accès nn autorisés.

L'OS fournit deux interfaces :
- **API** : interface de programmation permettant aux applications d'interagir avec l'OS via des appels système (comme `read()`).
- **Interface de commande** : interface utilisateur sous forme de commande (ex. `ls`) ou graphique.
 
Le shell (coquille) englobe l'OS.
'tty' = teletype désigne des terminaux virtuels sur linux.

>Les appels vers le shell se font par commandes.
>Les appels vers l'os sont des appels systèmes.

# Virtu vs Conteneurs
La virtualisation : Un OS hôte (hyperviseur) permet d'heberger plusieurs OS.
La conteneurisation : Un OS hôte permet de distinguer plusieurs espaces
utilisateurs isolés mais qui partagent le meme espace noyau (docker etc)

# Kernel (noyau)
C'est le coeur de l'OS. Il gère le matériel (CPU, mémoire, péripériques).
Un OS complet inclut le kernel + des outils et une interface utilisateur.

# POSIX
**UNIX** est une famille de systèmes d'exploitation reconnue pour sa simplicité et son code source libre, crée en 1970 et réécrite en C en 73, avec une standardisation assurée par **POSIX** pour garantir la compatibilité entre systèmes UNIX. Les fichiers jouent un rôle central dans UNIX.

# Fichiers
Dans l'API (interface de prog), un fichier est représenté par un descripteur de fichier, un entier positif (petits entiers, ds des tables du noyau). De plus, chaque fichiers sur disque est associé à un inode, qui contient les métadonnées du fichier (taille, propriétaire, permissions, ...).
**Rappel** :  les descripteurs 0,1 et 2 sont pris par défaut pour _STDIN_, _STDOUT_ et _STDERR_.

| Commande       | Qui ouvre le fichier ?       | Où `cat` lit-il ?       |
|----------------|------------------------------|-------------------------|
| `cat toto`     | `cat` ouvre direct le fich.  | Depuis le fich `toto`.  |
| `cat < toto`   | Le **shell** ouvre le fich et redirige `stdin`. | Depuis `stdin` (redirigé vers `toto`). |

Même si les descripteurs décrivent les memes fichiers, ils ont des pointeurs de lecture/écriture différents.
fork : Le desc de fich est partagé entre le père et le fils après le fork(), ce qui entraine un décalage de pointeur après lecturee par le fils ou père. 
dup : La fonction dup(fd1) ne crée pas un nouveau pointeur de lecture. Elle crée un nouveau descripteur qui partage le même pointeur de lecture que l'original.(TD 2)

la diff entre `dup()` et `dup2()` est que `dup2()` prend un deuxième arg qui est le desc de fich que l'on veut utiliser. Si le desc est déjà utilisé, il est fermé avant d'être utilisé.

# Sémaphores
Un **sémaphore** est un ENTIER partagé entre les proc pr contrôler l'accès à une src partagée. Il est utilisé pr la synchronisation entre proc.
L'**interblocage (deadlock)** est un pb qui survient lorsqu'un grp de proc attend indéfiniment une src qui ne sera jamais libérée.
> Solution : tuer l'un des processus ou faire revenir un proc en arrière.
La **famine** c'est quand un proc attend indéfiniment une ressource.
La **condition de compétition (race condition)**, c'est quand le résultat change avec l'ordre des instructions.
Un **mutex (mutual exclusion)** est un mécanisme de synchronisation utilisé pour protéger une ressource partagée.
Une sémahore(1) est toujours un mutex.
Une **cohorte** est une généralisation de mutex et sémaphores.

primitives associées aux sémaphores :
- **init(X)** : initialiser le sémaphore, avec X le nombre d'accès en simultané autorisés.
- **P()** : prendre le sémaphore (décrémenter) (wait) Si v=0, le proc est bloqué, sinon v=v-1
- **V()** : libérer le sémaphore (incrémenter) (signal) Si v<= un proc est bloqué, sinon v=v+1
Problèmes connus : Producteurs et consommateurs, Lecteurs et rédacteurs, Philosophes et fourchettes

# Processus
L'entité dynamique qui représente l'exec d'un prog sur un processeur.
Un proc est différent d'un progrm (code + données => passif) tandis qu'un processus est un programme en cours d'exec.
On peut uliliser le mm prog mais ne pas le mm processus.

Les processus communiquent via des mécanismes d'**IPC (Inter-Process Communication)** : 
- **Signaux** 
- **Pipes** et **fichiers** 
- **Mémoire partagée** et **sémaphores**
- **Sockets**

le **contexte** d'un processeur est l'ens des données qui le caractérise permettant
de reprendre son exec si interrompu.
-context utilisateur 
-contexte materiel : a l'instant donné, on a quoi dans les registres.
-contexte système : les sd du sys pr la realisation du processus

>pseudo-parallélisme = chacun son tour un petit peu de processus
La solution à la competition des processus est la file de prio FIFO.

Les processus peuvent être dans différents états :
- **Actif** : en exécution.
- **Bloqué** : en attente d'une ressource.
- **Suspendu** : mis en pause volontairement.
- **processus zombie** est un processus terminé dont le parent n'a pss encore récupéré le code de fin, ce qui consomme des ressources alors qu'il n'est plus exec. (le père n'a pas appelé wait)

Bonne pratique :
```c
int main() {
    if (fork() != 0) {
        int statut; pid_t fils;
        printf("Le père (%d) attend.\n", getpid());
        fils = wait(&statut);
        if (WIFEXITED(statut)) {
            printf("%d : fils %d terminé (code %d)\n", getpid(), fils, WEXITSTATUS(statut));   
    };
    exit(0);
    } else { 
        printf("je suis le fils, PID=%d\n",getpid());
        sleep(2) /* blocage pendant 2 secondes */
        printf("fin du fils\n");
        exit(1);
    }
} 
```

Lautodesctuction d'un processeus : exit
destruction par un autre : kill
Certains processus ne se terminent pas avant l'arret de la machine (comme le démons : disk and exec monitor.)

# Threads
Un thread est une unité d'execution à l'intérieur d'un processus. un proc peut etre
divisé en pls threads, qui partagent la mm memoire et les mm ressources, mais
qui peuvent exec différentes parties du prog en parallèle.
Si le thread principal est mort, tous les threads sont morts.
>L’espace d’adressage est passif ; le thread est actif.
Les communications inter-threads sont rapides. Pas besoin d’appels systèmes
Cause de terminaison de thread:
Il termine sa fonction initiale, il appelle pthread_exit, 
il est tué par un thread appelant pthread_cancel ou tout le proc se termine.

# Coeur
Un coeur est une partie physique du processeur (CPU)
qui exec des threads.

Chaque cœur peut gérer 2 threads dans les processeurs modernes, 
donc un processeur à 8 cœurs peut gérer 16 threads. C'est de l'hyperthreading.ca

L'hyperthreading est une technologie qui permet à un seul cœur de traiter
deux threads en même temps.

la lib des appels système/primitives est dans <unistd.h>
Ce sont des fonctions qui permet à un programme
d'interagir avec le noyau du système d'exploitation.
Ex : `fork()`, `getpid()`, ...

# Fonctions
## Fork
La commande fork() crée un processus "fils" du processus appelant, le "père",
avec le même programme que le "père".
>On peut pas savoir si c'est le fils ou le père qui s'exec en premier.
La valeur renvoyée par fork() est:
- le pid du processus fils pour le processus père
- 0 pour le processus fils 
On peut retrouver le pid du processus père avec la commande/primitive `getppid()`.

On peut cloner un  processus avec & : $prog1 & prog2
Ou alors l'exec en tache de fond : $commande &
La tache de fond est pratique car le parent ne gère pas l'attente du fils.
1er plan -> Supendu : Ctr-Z
Suspendu ou Arrière plan -> 1er plan : fg %<job>
Suspendu -> Arrière plan : bg %<job>

En cas d'échec, aucun processus n'est créée et la valeur -1 est renvoyée.
Tout processus dont le père est mort sont adoptés par le processus init (pid = 1).

>Le fils fait une copie exacte du parent au moment du fork, y compris les variables, mais il ne rééxécute pas le code avant le fork. Le père reprend l'exécution après le fork et le fils commence. Le code du fils, c'est comme le code du père mais après le fork. 

>Les variables du fils sont des copies des variables du père au moment du fork, dans des **nouveaux emplacements mémoire**. On duplique l'espace d'adressage.

>Si dans un programme, il y a n fork, il y a **2^n** processus créés.

```c
int main(){
    fork();
    fork();
    printf("Hello\n");
    exit(0);
}
``` 
affiche la même chose que le programme suivant:

```c
int main(){
    int i;
    for (i=0; i<2; i++){
        fork();
    }
    printf("Hello\n");
    exit(0);
}
```


## Boucle for et while
>Lorsque fork est executé dans une boucle for, le processus hérite de l'état de la boucle dans laquelle le parent était au moment du fork. 

Ex: si la parent était à l'itération i = 1 dans la boucle for, le fils commencera aussi à la deuxième itération de la boucle for, c'est à dire i = 1.

## Pipes
`pipe()` crée un canal de communication unidirectionnel entre 2 proc (généralement un proc parent et un proc enfant).
C'est comme un tube virtuel.

cat fich | grep mot connecte le stdout du fich dans l'entrée standard de grep.

Les deux extrémités du pipe sont représentées par deux descripteurs de fichier :
    fd[0] : pour lire. (stdin)
    fd[1] : pour écrire. (stdout)

write(fd, "hello", 5) : écrit 5 caractères dans le pipe
read(fd, buffer, 5) : lit 5 caractères du pipe

Il y a aussi des pipes nommés (FIFO)

## Exec
La famille de primitives **exec** permet de créer un processus pour éxécuter un programme déterminé.
>On peut remplacer un processus par un autre avec exec().
>Il n'est pas possible avec fork de lancer un sous programme, pour cela il faut utiliser exec().

execl, execle, execlp, execv etc.
-l : la fonction prend une liste d'arguments
-v : la fonction prend un tableau d'arguments
-p : le programme est cherché dans le PATH
exemple :
execv(const char *file, char *const argv[])
En fait avec execv, le code du processus est remplacé par le code du programme passé en argument.
Conclusion : c'est un **recouvrement** de code.

## Autres
>Il suffit de faire un man dans le terminal pour avoir la documentation d'une fonction.

`yield()` :  Lorsqu'un thread appelle yield, il indique au système d'exploitation qu'il est prêt à laisser d'autres threads s'exécuter.
`getpid()` :
`getppid()` : pour le père
`getuid()` : obtenir le numéro d'usager (auquel appartient le processus)
`pause()` : bloquer jusqua reception du signal
`sleep()` : stop n secondes

`wait(int *ptr_etat)` : le père attend un de ses fils. code de fin stoqué dans la var
`waitpid(pid_t pid, int *ptr_etat)` : le père attend un fils spécifique de pid pid, -1 pour n'importe quel fils

`popen()` : ouvre un pipe pour exec une commande shell permettant de lire l'output.
`read()` : lire dans un fichier
`write()` : ecrire dans un fichier
`lockf()` : verrouiller un fichier

Les appels système comme write ou read peuvent être interrompus par des signaux ou des événements système, ce qui entraîne un traitement partiel.
fread et fwrite gèrent automatiquement les buffers et positions dans les fichiers.

`lseek() `: changer la position du pointeur de lecture : lseek(fd, 30, SEEK_CUR pour faire + 30 ou SEEK_SET pour mettre en 30)
`dup()` : dupliquer un descripteur de fichier
`dup2()` : dupliquer un descripteur de fichier dans un autre descripteur
`system()` : exécuter une commande shell

`getenv()` : obtenir la valeur d'une variable d'environnement
`setevn()` : modifier la valeur d'une variable d'environnement
`exit(0)` : 0 si pas d'erreur 
`sigaction()` : installer un gestionnaire de signal
`memset()` 

`phthread_create()` : créer un thread
`pthread_exit()` : terminer un thread
`pthread_join()` : attendre la fin d'un thread
`pthread_self()` : obtenir le numéro du thread courant
`pthread_equal()` : comparer deux threads

# Erreurs
la librairie <errno.h> contient la variable errno qui est une variable globale qui contient le code d'erreur de la dernière fonction qui a échoué. 
0 : tout est ok
2 : ENOENT (No such file or directory).

# Signaux 
Les **signaux** sont des messages **asynchrones** pour contrôler les processus, pouvant survenir à tout moment. Ils sont envoyés par le noyau ou par un autre processus.:
- **SIGINT** : interrompt un processus (ex. : <Ctrl+C>).
- **SIGSTP** : suspend un proc (<Ctrl+z>)
- **SIGTERM** : demande de terminaison.
- **SIGKILL** : termine immédiatement un processus (non interceptable).
- **SIGCHILD** : signal envoyé au parent lors de la fin d'un fils.

Les signaux facilitent la synchronisation entre processus, permettant, par exemple, à un processus fils de signaler au parent la fin de son exécution pour libérer les ressources.

La structure à utiliser pour un gestionnaire associé à un signal est:
```c
struct sigaction {
    void (*sa handler)(int);
    void (* sa_sigaction) (int, siginfo_t *, void *);
    sigset t sa mask;
    int sa flags;
}
```


## Résumé
## 1 : OS
Son rôle est : d'offrir une interface du matériel unifiée et plus adaptée,
d'assurer la gestion et la protection des ressources.

Les techniques classiques de protection des ressources sont:
-Préemption (l'os peut interrompre un proc en cours pour en exec un autre)
-Interposition (un composant peut s'insérer entre 2 autres pour controler)
-Mode d’exécution (protégé ou non) (mode utilisateur ou mode noyau)
Le mode noyau est activé lors des appels systèmes.

## 2
Utilité des processus :
-Simplicité (séparation entre les activités)
-Sécurité (séparation entre les activités)
-Efficacité (quand l’un est bloqué, on passe à autre chose)

Création : fork()
-résultat=0 → je suis le fils
-résultat>0 → je suis le père (résultat=pid du fils)
-résultat<0 → erreurc

## 3 : Signaux
-pgid : groupe de processus ; un par processus lancé par shell (et tous ses fils)

($)kill -nom victime    API : kill(pid t victime, int sig)

victime > 0 : numéro du pid visé
victime = 0 : tous les process de ce groupe (pgid)
victime = -1 : tous les process accessibles

Envoyer le signal 9 à tous les processus : kill -9 -1
Pour installer un nouveau gestionnaire:
int sigaction(int sig, const struct sigaction *newaction, struct sigaction *oldaction);
int alarm(unsigned int seconds) : envoie un signal SIGALRM au processus après un certain temps

## 4
Interface de bas niveau (syscalls) : open, read, write, close
Problèmes : I/O tronquée, perfs par manque de tampon

## 5
Déclaration de variables externes:
-utiliser les .h, éviter les variables globales, bon usage des fonctions.

## 6 : Lib
Exercice : quelle(s) commande(s) fonctionne(nt)/échoue(nt) ? Pourquoi ?
prog.c utilise machin() de libmachin.a, et machin() utilise truc() de libtruc.a
I gcc -o prog prog.c -ltruc -lmachin : référence à truc() indéfinie
I gcc -o prog prog.c -lmachin -ltruc : fonctionne

Exercice : que faire en cas de dépendances circulaires ?
faire figurer une bibliothèque deux fois pour casser le cycle

>Les bibliothèque statiques sont liée aux programmes lors de la compilation. Son code est directement intégré dans l'executable, ce qui le rend autonome mais plus volumineux. Sur unix (.a = archive) et sur windows (.lib = library).

>Les bibliothèque dynamique (.so = shared object) sur Unix et DLL (Dynamically Linkable Library) sur Windows, sont chargée au moment de l'exec. Cela rend plus léger mais il depend de la lib sur le système.

## 7 : Verouillage de fichiers
`int lockf(int fd, int commande, off_t taille);`
cmd : F_ULOCK : deverouiller, 
F_LOCK : Verouillage exclusif,
F_TLOCK : verouillage exclusi avec test, 
F_TEST : test seulement, 
taile : =0 pour le fichier complet

## 8 : interblocage
Condition de compétition (race condition)
Définition : le résultat change avec l’ordre des instructions
Difficile à corriger car difficile à reproduire (ordre aléatoire)
Par exemple, deux proc qui vont lire une var et la modifier en même temps.

Interblocage (deadlock)
Définition : un groupe de processus bloqués en attente mutuelle
Évitement parfois difficile (correction de l’algorithme)
Détection assez simple, mais pas de guérison sans perte

## 9 : Sémaphores
Exclusion mutuelle : Ressource utilisée par au plus un utilisateur
Cohorte : Ressource partagée entre au plus N utilisateurs
Rendez-vous : un processus attend un autre, ou attente mutuelle
Producteurs/consommateurs : utilisation après création
Lecteurs/rédacteurs : concurrence entre catégories d’utilisateurs

## 10 Threads
Un code est rentrant si il peut etre appelé récursivement depuis plusieurs endroits.
Si un code ne l'est pas, il faut le protéger par verrouillage.