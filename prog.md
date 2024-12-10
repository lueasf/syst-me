# Programmes C


## cp
```bash
$ cp src dest
```

```
while((nb_read = read(src, buf, sizeof(buf)))>0){
    nb_written = 0;
    while (nb_written < nb_read) {
        tmp = write(dest, buf + nb_written, nb_read - nb_written);
        if (tmp == -1) break; /* erreur */
        nb_written += tmp;
    }
}
```
>read() lit un maximum de sizeof(buf) octets à partir de src dans le tampon buf. nb_read contient le nombre d'octets lus.
>write() ecrit des octets depuis buf dans dest, à partir de l'offset nb_written et spécifie le nombre d'octets qu'il reste à écrire.


## wait
```
int statut; pid_t fils;
fils = wait(&statut);
if (WIFEXITED(statut)) {
    printf("fils %d (code %d)\n", fils, WEXITSTATUS(statut));
};
```
>wait(&statut) attend la fin d'un processus fils et récupère son statut de sortie. Renvoie le pid du processus fils si tout s'est bien passé ou -1 si erreur. Pareil pour wait(NULL), sans récupération du statut.
>WIFEXITED(statut)  vérifie si un proc s'est terminé normalement. Renvoie une valeur non nulle(vrai) si le processus s'est terminé normalement, faux sinon.


## waitpid
```
int status;
pid_t pid;

fprintf(stderr, "Hello\n");
pid = fork();
fprintf(stderr, "%d\n", !pid);

if (pid != 0) {
    if (waitpid(-1, &status, 0) > 0) {
        if (WIFEXITED(status) != 0) {
            fprintf(stderr, "%d\n", WEXITSTATUS(status));
        }
    }
}
```
>waitpid(-1, &status, 0) attend la fin de n'importe quel processus fils (-1). Son status sera stoqué dans status. 0 indique que waitpid se comporte de manière bloquante (=pas d'option). Renvoie le pid du proc fils si tout s'est bien passé ou -1 si erreur. 
>waitpid(-1, &status, WNOHANG) est non bloquant et renvoie 0 si aucun fils n'est terminé.
>WIFEXITED(status) vérifie si un proc s'est terminé normalement.
>WEXITSTATUS(status) retourne le code de sortie du processus fils (0-255), 0 = sans erreur, qui existe que quand WIFEXITED(status) est vrai. 




## sigaction
```
struct sigaction nvt,old;
int reponse,restant;
memset(&nvt, 0, sizeof(nvt));
nvt.sa_handler = handler;
sigaction(SIGALRM, &nvt, &old);
```

## ls -i
```
int main() {
    DIR *dirp;
    struct dirent *dp;

    dirp = opendir(".");
    while ((dp = readdir(dirp)) != NULL) {
        printf ("%s\t%d\n", dp->d name, dp->d ino);
    }
    closedir(dirp);
}
```
