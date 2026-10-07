/* ========= files.c ========= 
File: files.c
Scopo:
Gestione dei file binari del progetto.
Salvataggio e lettura dei dati.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "files.h"

void salvaTennista(Tennista t) {

    FILE *fp = fopen(FILE_PLAYERS, "ab");

    if(fp == NULL) {

        printf("Errore apertura file!\n");
        return;
    }

    fwrite(&t, sizeof(Tennista), 1, fp);

    fclose(fp);

    printf("Tennista salvato correttamente!\n");
}

void leggiTennisti() {

    FILE *fp = fopen(FILE_PLAYERS, "rb");

    Tennista t;

    if(fp == NULL) {

        printf("Nessun file trovato.\n");
        return;
    }

    while(fread(&t, sizeof(Tennista), 1, fp)) {

        printf("\nID: %d\n", t.id);
        printf("Nome: %s\n", t.nome);
        printf("Cognome: %s\n", t.cognome);
        printf("Ranking: %d\n", t.ranking);
        printf("Crediti: %.2f\n", t.crediti);
        printf("Disponibile: %s\n",
               t.disponibile ? "SI" : "NO");
    }

    fclose(fp);
}

int trovaTennistaByID(int id, Tennista *t) {

    FILE *fp = fopen(FILE_PLAYERS, "rb");

    if(fp == NULL)
        return 0;

    while(fread(t, sizeof(Tennista), 1, fp)) {

        if(t->id == id) {

            fclose(fp);
            return 1;
        }
    }

    fclose(fp);

    return 0;
}

void aggiornaTennista(Tennista updated) {

    FILE *fp = fopen(FILE_PLAYERS, "rb");
    FILE *temp = fopen("temp.dat", "wb");

    Tennista t;

    if(fp == NULL || temp == NULL) {

        printf("Errore file.\n");
        return;
    }

    while(fread(&t, sizeof(Tennista), 1, fp)) {

        if(t.id == updated.id)
            fwrite(&updated, sizeof(Tennista), 1, temp);
        else
            fwrite(&t, sizeof(Tennista), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove(FILE_PLAYERS);
    rename("temp.dat", FILE_PLAYERS);

    printf("Tennista aggiornato!\n");
}

void eliminaTennistaFile(int id) {

    FILE *fp = fopen(FILE_PLAYERS, "rb");
    FILE *temp = fopen("temp.dat", "wb");

    Tennista t;

    if(fp == NULL || temp == NULL) {

        printf("Errore file.\n");
        return;
    }

    while(fread(&t, sizeof(Tennista), 1, fp)) {

        if(t.id != id)
            fwrite(&t, sizeof(Tennista), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove(FILE_PLAYERS);
    rename("temp.dat", FILE_PLAYERS);

    printf("Tennista eliminato!\n");
}

int esisteID(int id) {

    FILE *fp = fopen(FILE_PLAYERS, "rb");

    Tennista t;

    if(fp == NULL)
        return 0;

    while(fread(&t, sizeof(Tennista), 1, fp)) {

        if(t.id == id) {

            fclose(fp);
            return 1;
        }
    }

    fclose(fp);

    return 0;
}
