#include <stdio.h>
#include <fcntl.h> 
#include <errno.h>


// EX 1
/*
P1 et P2 utilsient 2 sémaphores (inventés par Dijkstra), a et b, 
initialisés à 1 et 0 respectivement. 
P(x) ("wait") verifie si x>0 puis x-- sinon bloqué jusqu'à ce que x>0.
V(x) ("signal") x++.

P consomme une ressource, V la libère.

Dans l'exercice 1, on fait un diagramme de transition d'états pour les processus P1 et P2.
On ne sait pas dans quel ordre, P1 et P2 vont s'exécuter, donc on considère tous les cas possibles.
C'est à dire P1(a), P2(a), P1(b), P2(b), P1(a), P2(b), P1(b), P2(a).

On représente les actions des deux processus sur deux axes :

    Ligne (axe Y) : Les actions du Processus 1 dans l'ordre.
    Colonne (axe X) : Les actions du Processus 2 dans l'ordre.
*/


/*
Si on a P1 : Pa, Va, Pb, Vb et P2 : Pa, Va, Pb, Vb
On remarque qu'il n'existe pas d'ordre dans lequel les deux
sémaphores sont positifs ou nuls en même temps, peu importe le chemin

*/

// EX 2
/*
Les paires de sémaphores utilisés, c'est quand (a,b,c) avec des sémaphores consommé en meme temps.
Ensuite, on nous demande  de discuter les reservations selon l'ordre a<b<c.
P1 est safe avec ce mode,
P2  viole l'ordre mais aucun autre processus ne "verrouille" à la fois b et c, donc il est safe.
P3 viole l'ordre  pour (a,b) en demandant b puis a mais c'est pas safe car P1 demande a puis b?
Ce qui pourrait conduire a un interblocage ou deadlock car si P1 verouille a et attend b,
et de l'autre coté P3 verouille b et attend a, on a un interblocage.
*/