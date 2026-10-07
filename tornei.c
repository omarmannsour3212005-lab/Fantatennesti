/* ========= tornei.c ========= 
File: tornei.c
Scopo:
Gestione tornei e incontri.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "models.h"
#include "tornei.h"

/** Controlla se un torneo esiste nel file binario. */
int torneoExists(int torneoId) {
    FILE *fp = fopen("tornei.dat", "rb");
    Torneo t;

    if(fp == NULL) return 0;

    while(fread(&t, sizeof(Torneo), 1, fp)) {
        if(t.id == torneoId) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}


/** Controlla se esiste almeno un match inserito.
    Serve per non modificare ranking e stato dei tennisti durante un torneo. */
int esistonoMatch() {
    FILE *fp = fopen("matches.dat", "rb");
    Match m;

    if(fp == NULL) return 0;

    if(fread(&m, sizeof(Match), 1, fp)) {
        fclose(fp);
        return 1;
    }

    fclose(fp);
    return 0;
}

/** Conta quante squadre partecipano ad un torneo. */
int contaSquadreTorneo(int torneoId) {
    FILE *fp = fopen("squadre.dat", "rb");
    Squadra s;
    int count = 0;

    if(fp == NULL) return 0;

    while(fread(&s, sizeof(Squadra), 1, fp)) {
        if(s.torneoId == torneoId) count++;
    }

    fclose(fp);
    return count;
}

/** Aggiunge un torneo nel file binario. */
void addTorneo() {
    FILE *fp;
    Torneo t;

    fp = fopen("tornei.dat", "ab");
    if(fp == NULL) {
        printf("Errore apertura file!\n");
        return;
    }

    printf("\n===== AGGIUNGI TORNEO =====\n");
    printf("Inserisci ID torneo: ");
    scanf("%d", &t.id);
    printf("Inserisci nome torneo: ");
    scanf("%49s", t.nome);
    printf("Inserisci ID vincitore (-1 se non ancora deciso): ");
    scanf("%d", &t.winnerId);

    fwrite(&t, sizeof(Torneo), 1, fp);
    fclose(fp);

    printf("Torneo salvato correttamente!\n");
}

/** Visualizza i tornei e il numero di squadre partecipanti. */
void viewTornei() {
    FILE *fp;
    Torneo t;

    fp = fopen("tornei.dat", "rb");
    if(fp == NULL) {
        printf("Nessun torneo trovato!\n");
        return;
    }

    printf("\n===== LISTA TORNEI =====\n");

    while(fread(&t, sizeof(Torneo), 1, fp)) {
        printf("\nID: %d", t.id);
        printf("\nNome: %s", t.nome);
        printf("\nWinner ID: %d", t.winnerId);
        printf("\nSquadre partecipanti: %d\n", contaSquadreTorneo(t.id));
    }

    fclose(fp);
}

/** Modifica nome e vincitore del torneo. */
void modifyTorneo() {
    FILE *fp = fopen("tornei.dat", "rb");
    FILE *temp = fopen("temp_tornei.dat", "wb");
    Torneo t;
    int id, found = 0;

    if(fp == NULL || temp == NULL) {
        printf("Errore file.\n");
        return;
    }

    printf("Inserisci ID torneo da modificare: ");
    scanf("%d", &id);

    while(fread(&t, sizeof(Torneo), 1, fp)) {
        if(t.id == id) {
            printf("Nuovo nome: ");
            scanf("%49s", t.nome);
            printf("Winner ID (-1 se non ancora deciso): ");
            scanf("%d", &t.winnerId);
            found = 1;
        }
        fwrite(&t, sizeof(Torneo), 1, temp);
    }

    fclose(fp);
    fclose(temp);
    remove("tornei.dat");
    rename("temp_tornei.dat", "tornei.dat");

    if(found) printf("Torneo modificato.\n");
    else printf("Torneo non trovato.\n");
}

/** Elimina un torneo dal file. */
void deleteTorneo() {
    FILE *fp = fopen("tornei.dat", "rb");
    FILE *temp = fopen("temp_tornei.dat", "wb");
    Torneo t;
    int id, found = 0;

    if(fp == NULL || temp == NULL) {
        printf("Errore apertura file!\n");
        return;
    }

    printf("Inserisci ID torneo da eliminare: ");
    scanf("%d", &id);

    while(fread(&t, sizeof(Torneo), 1, fp)) {
        if(t.id == id) found = 1;
        else fwrite(&t, sizeof(Torneo), 1, temp);
    }

    fclose(fp);
    fclose(temp);
    remove("tornei.dat");
    rename("temp_tornei.dat", "tornei.dat");

    if(found) printf("Torneo eliminato correttamente!\n");
    else printf("Torneo non trovato!\n");
}

/** Aggiunge un incontro relativo ad un torneo. */
void addMatch() {
    FILE *fp;
    Match match;

    fp = fopen("matches.dat", "ab");
    if(fp == NULL) {
        printf("Errore apertura file!\n");
        return;
    }

    printf("\n===== ADD MATCH =====\n");
    printf("Inserisci Match ID: ");
    scanf("%d", &match.id);
    printf("Inserisci ID Torneo: ");
    scanf("%d", &match.torneoId);
    printf("Player 1 ID: ");
    scanf("%d", &match.player1Id);
    printf("Player 2 ID: ");
    scanf("%d", &match.player2Id);
    printf("Set Player 1: ");
    scanf("%d", &match.setPlayer1);
    printf("Set Player 2: ");
    scanf("%d", &match.setPlayer2);
    printf("Ace Player 1: ");
    scanf("%d", &match.ace1);
    printf("Ace Player 2: ");
    scanf("%d", &match.ace2);
    printf("Break Player 1: ");
    scanf("%d", &match.break1);
    printf("Break Player 2: ");
    scanf("%d", &match.break2);
    printf("Doppi Falli Player 1: ");
    scanf("%d", &match.doppiFalli1);
    printf("Doppi Falli Player 2: ");
    scanf("%d", &match.doppiFalli2);
    printf("Winner ID: ");
    scanf("%d", &match.winnerId);

    fwrite(&match, sizeof(Match), 1, fp);
    fclose(fp);

    printf("Match salvato correttamente!\n");
}

/** Menu di gestione tornei. */
void menuTornei() {
    int scelta;

    do {
        printf("\n===== MENU TORNEI =====\n");
        printf("1. Add Torneo\n");
        printf("2. View Tornei\n");
        printf("3. Modify Torneo\n");
        printf("4. Delete Torneo\n");
        printf("5. Add Match\n");
        printf("0. Exit\n");
        printf("Scelta: ");
        scanf("%d", &scelta);

        switch(scelta) {
            case 1: addTorneo(); break;
            case 2: viewTornei(); break;
            case 3: modifyTorneo(); break;
            case 4: deleteTorneo(); break;
            case 5: addMatch(); break;
            case 0: break;
            default: printf("Scelta non valida!\n");
        }
    } while(scelta != 0);
}
