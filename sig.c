#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <X11/Xlib.h>

extern void initrec();
extern int attendreclic();

int count_pere = 0;
int count_fils = 0;

void captpere(int sig_num) {
    count_pere++;
    printf("Signal SIGINT reçu par le père (total: %d)\n", count_pere);
    if (count_pere == 3) {
        exit(0);
    }
}

void captfils(int sig_num) {
    count_fils++;
    printf("Signal SIGINT reçu par le fils (total: %d)\n", count_fils);
    if (count_fils == 3) {
        XCloseDisplay(XOpenDisplay(NULL));
        exit(0);
    } else {
        // Mettre le rectangle en vert pendant 5 secondes
    }
}

int main() {
    pid_t pid;
    int status;

    signal(SIGINT, captpere);

    pid = fork();

    if (pid == 0) { // Processus fils
        signal(SIGINT, captfils);

        initrec();

        int i = 0;
        while (i != -1) {
            i = attendreclic();

            if (i == 0) {
                kill(getppid(), SIGINT);
            }
        }
    } else { // Processus père
        while (1) {
            unsigned int remaining_sleep = sleep(10);
            if (remaining_sleep > 0) {
                printf("Temps d'attente restant : %u secondes\n", remaining_sleep);
            }
        }
    }

    return 0;
}
