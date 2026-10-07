/* ========= tennista.c ========= 
File: tennista.c
Scopo:
Gestione dei tennisti:
aggiunta, modifica, eliminazione e ricerca.
*/

#include <stdio.h>
#include <string.h>

#include "tennista.h"
#include "files.h"
#include "tornei.h"

/** Calcola i crediti in modo semplice in base al ranking. */
float calcolaCrediti(int ranking) {
    if(ranking == 1) return 35;
    if(ranking <= 5) return 33;
    if(ranking <= 10) return 30;
    if(ranking <= 20) return 25;
    if(ranking <= 50) return 20;
    if(ranking <= 100) return 15;
    return 10;
}

/** Stampa i dati di un tennista. */
void stampaTennista(Tennista t) {
    printf("\nID: %d", t.id);
    printf("\nNome: %s", t.nome);
    printf("\nCognome: %s", t.cognome);
    printf("\nRanking: %d", t.ranking);
    printf("\nCrediti: %.2f", t.crediti);
    printf("\nDisponibile: %s\n", t.disponibile ? "SI" : "NO");
}

/** Aggiunge un tennista nel file binario. */
void aggiungiTennista() {
    Tennista t;

    printf("\n===== AGGIUNGI TENNISTA =====\n");
    printf("ID: ");
    scanf("%d", &t.id);

    if(t.id <= 0 || esisteID(t.id)) {
        printf("ID non valido o gia esistente.\n");
        return;
    }

    printf("Nome: ");
    scanf("%49s", t.nome);
    printf("Cognome: ");
    scanf("%49s", t.cognome);
    printf("Ranking: ");
    scanf("%d", &t.ranking);

    if(t.ranking <= 0) {
        printf("Ranking non valido.\n");
        return;
    }

    t.crediti = calcolaCrediti(t.ranking);
    printf("Disponibile (1=SI, 0=NO): ");
    scanf("%d", &t.disponibile);

    salvaTennista(t);
}

/** Visualizza tutti i tennisti salvati. */
void visualizzaTennisti() {
    leggiTennisti();
}

/** Ricerca per ID, nome/cognome, costo o stato. Usa anche sottostringhe con strstr. */
void cercaTennista() {
    FILE *fp;
    Tennista t;
    int scelta, id, stato, trovato = 0;
    float costo;
    char testo[50];

    fp = fopen(FILE_PLAYERS, "rb");
    if(fp == NULL) {
        printf("Nessun file trovato.\n");
        return;
    }

    printf("\n===== RICERCA TENNISTA =====\n");
    printf("1. Per ID\n");
    printf("2. Per nome o cognome (anche sottostringa)\n");
    printf("3. Per costo massimo\n");
    printf("4. Per stato disponibile/non disponibile\n");
    printf("Scelta: ");
    scanf("%d", &scelta);

    if(scelta == 1) {
        printf("ID: ");
        scanf("%d", &id);
    } else if(scelta == 2) {
        printf("Testo da cercare: ");
        scanf("%49s", testo);
    } else if(scelta == 3) {
        printf("Costo massimo: ");
        scanf("%f", &costo);
    } else if(scelta == 4) {
        printf("Stato (1=disponibile, 0=non disponibile): ");
        scanf("%d", &stato);
    }

    while(fread(&t, sizeof(Tennista), 1, fp)) {
        int ok = 0;

        if(scelta == 1 && t.id == id) ok = 1;
        if(scelta == 2 && (strstr(t.nome, testo) != NULL || strstr(t.cognome, testo) != NULL)) ok = 1;
        if(scelta == 3 && t.crediti <= costo) ok = 1;
        if(scelta == 4 && t.disponibile == stato) ok = 1;

        if(ok) {
            stampaTennista(t);
            trovato = 1;
        }
    }

    fclose(fp);

    if(!trovato) printf("Nessun tennista trovato.\n");
}

/** Modifica i dati di un tennista.
    Ranking e stato non vengono modificati se ci sono match inseriti,
    per rispettare la regola: non durante un torneo. */
void modificaTennista() {
    int id;
    Tennista t;

    printf("Inserisci ID da modificare: ");
    scanf("%d", &id);

    if(trovaTennistaByID(id, &t)) {
        printf("Nuovo nome: ");
        scanf("%49s", t.nome);
        printf("Nuovo cognome: ");
        scanf("%49s", t.cognome);

        if(esistonoMatch()) {
            printf("Ranking e stato non modificati: ci sono match inseriti.\n");
        } else {
            printf("Nuovo ranking: ");
            scanf("%d", &t.ranking);
            t.crediti = calcolaCrediti(t.ranking);
            printf("Disponibile? (1/0): ");
            scanf("%d", &t.disponibile);
        }

        aggiornaTennista(t);
    } else {
        printf("Tennista non trovato.\n");
    }
}

/** Elimina un tennista dal file. */
void eliminaTennista() {
    int id;
    printf("Inserisci ID da eliminare: ");
    scanf("%d", &id);
    eliminaTennistaFile(id);
}

/** Menu di gestione tennisti. */
void menuTennista() {
    int scelta;

    do {
        printf("\n===== MENU TENNISTI =====\n");
        printf("1. Aggiungi tennista\n");
        printf("2. Visualizza tennisti\n");
        printf("3. Ricerca tennista\n");
        printf("4. Modifica tennista\n");
        printf("5. Elimina tennista\n");
        printf("0. Indietro\n");
        printf("Scelta: ");
        scanf("%d", &scelta);

        switch(scelta) {
            case 1: aggiungiTennista(); break;
            case 2: visualizzaTennisti(); break;
            case 3: cercaTennista(); break;
            case 4: modificaTennista(); break;
            case 5: eliminaTennista(); break;
            case 0: break;
            default: printf("Scelta non valida.\n");
        }
    } while(scelta != 0);
}

void menuPlayers() {
    menuTennista();
}
