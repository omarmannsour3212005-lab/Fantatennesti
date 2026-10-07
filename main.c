/* ========= main.c ========= 
File: main.c
Scopo:
Gestione del menu principale della piattaforma Fantatennis.
Permette l'accesso alle funzionalità Admin, Fanta-Coach e Ranking.
*/



#include <stdio.h>
#include <stdlib.h>

#include "tennista.h"
#include "tornei.h"
#include "ranking.h"
#include "coach.h"
#include "squadra.h"

// ================= ADMIN MENU =================

void menuAdmin() {

    int scelta;

    do {

        printf("\n========== MENU ADMIN ==========\n");
        printf("1. Gestione Tennista\n");
        printf("2. Gestione Tornei\n");
        printf("3. Ranking\n");
        printf("0. Torna indietro\n");

        printf("Scelta: ");

        if(scanf("%d", &scelta) != 1) {

            printf("Input non valido!\n");

            while(getchar() != '\n');

            scelta = -1;
            continue;
        }

        switch(scelta) {

            case 1:
                menuTennista();
                break;

            case 2:
                menuTornei();
                break;

            case 3:
                menuRanking();
                break;

            case 0:
                printf("Ritorno...\n");
                break;

            default:
                printf("Scelta non valida!\n");
        }

    } while(scelta != 0);
}


// ================= COACH MENU =================

void menuCoach() {

    int scelta;

    do {

        printf("\n======= MENU FANTA COACH =======\n");

        printf("1. Registrazione Coach\n");
        printf("2. Creazione Squadra\n");
        printf("3. Modifica Squadra\n");
        printf("4. Visualizza Squadre\n");
        printf("5. Visualizza Tornei\n");
        printf("6. Ricerca Tennista\n");
        printf("0. Torna indietro\n");

        printf("Scelta: ");

        if(scanf("%d", &scelta) != 1) {

            printf("Input non valido!\n");

            while(getchar() != '\n');

            scelta = -1;
            continue;
        }

        switch(scelta) {

            case 1:
                registerCoach();
                break;

            case 2:
                createTeam();
                break;

            case 3:
                modifyTeam();
                break;

            case 4:
                viewTeams();
                break;

            case 5:
                viewTornei();
                break;

            case 6:
                cercaTennista();
                break;

            case 0:
                printf("Ritorno...\n");
                break;

            default:
                printf("Scelta non valida!\n");
        }

    } while(scelta != 0);
}


// ================= MAIN =================

int main() {

    int scelta;

    do {

        printf("\n====================================\n");
        printf("         FANTA TENNIS GAME          \n");
        printf("====================================\n");

        printf("1. Admin\n");
        printf("2. Fanta Coach\n");
        printf("3. Ranking Generale\n");
        printf("0. Esci\n");

        printf("Scelta: ");

        if(scanf("%d", &scelta) != 1) {

            printf("Input non valido!\n");

            while(getchar() != '\n');

            scelta = -1;
            continue;
        }

        switch(scelta) {

            case 1:
                menuAdmin();
                break;

            case 2:
                menuCoach();
                break;

            case 3:
                showGeneralRanking();
                break;

            case 0:
                printf("Uscita dal programma...\n");
                break;

            default:
                printf("Scelta non valida!\n");
        }

    } while(scelta != 0);

    return 0;
}
