/**
 * Programma con l'utilizzo di fork.
 * Il padre fa una cosa, il figlio fa un'altra.
 * 
 * Il figlio esegue ls
 * Il padre legge l'output
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include <fcntl.h>

int main () {

    int fd[2];
    pid_t c_pid;

    if (pipe(fd) == -1) {
        fprintf(stderr, "Error to set pipe\n");
        exit(-1);
    }

    if ((c_pid = fork()) < 0)
        fprintf(stderr, "Error to fork\n");

    if (c_pid == 0) {
        char *ex[] = {"ls", "-l", NULL};

        // reindirizza lo standard output nel file descripor collegato al pipe
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]); // non ci serve, chiudi. il canale del pipe funziona ugualmente.
        close(fd[0]); // non ci serve, chiudi.
        execv("/usr/bin/ls", ex);

        exit(errno); // se fallisce exec() ritorna il valore di errno.
    } else {
        char ch[1024];
        int bytesRead;
        int status; // prendi il valore di uscita dal processo.
        int fp_out; // scrivi in un file l'output di ls (che passa per il pipe)
        
        waitpid(c_pid, &status, WUNTRACED); // aspetta la fine del processo figlio.
        close(fd[1]); // non ci serve. chiudi.

        if (WEXITSTATUS(status)) {
            // controlla il valore di errno se è diverso da 0 e stampa il tipo di errore.
            fprintf(stderr, "Failed for: %s\n", strerror(WEXITSTATUS(status)));
            exit(0);
        }

        bytesRead = read(fd[0], ch, 1024); // leggi tramite il pipe.
        /**
         * metti un valore di fine stringa quando finisce di leggere.
         * evita gli errori strlen con la stampa.
         */
        ch[bytesRead] = '\0';
        printf("\n\n%s\n\n", ch);
        
        fp_out = open("file.txt", O_WRONLY | O_CREAT, 00700);

        if (write(fp_out, ch, bytesRead) == bytesRead)
            printf("Scrittura su file andata completamente a buon fine\n");
        else
            fprintf(stderr, "Qualche errore nella scrittura su file\n");

        close(fp_out);
        close(fd[0]); // abbiamo finito di usarlo. chiudi.
    }

    return 0;
}