/* ========= coach.c ========= 
File: coach.c
Scopo:
Gestione dei Fanta-Coach.
Permette registrazione e gestione dei coach.
*/

#include <stdio.h>
#include <string.h>

#include "coach.h"
#include "utils.h"

#define COACH_FILE "coach.dat"


// ==================== CHECK EXIST ====================

int coachExists(char nickname[]) {

    FILE *fp;

    Coach coach;

    fp = fopen(COACH_FILE, "rb");

    if(fp == NULL) {
        return 0;
    }

    while(fread(&coach, sizeof(Coach), 1, fp)) {

        if(strcmp(coach.nickname, nickname) == 0) {

            fclose(fp);

            return 1;
        }
    }

    fclose(fp);

    return 0;
}


// ==================== REGISTER ====================

void registerCoach() {

    FILE *fp;

    Coach coach;

    printf("\n=== REGISTER COACH ===\n");

    printf("Enter nickname: ");
    scanf("%s", coach.nickname);

    clearInputBuffer();

    // Check duplicate nickname
    if(coachExists(coach.nickname)) {

        printf("Nickname already exists!\n");

        return;
    }

    fp = fopen(COACH_FILE, "ab");

    if(fp == NULL) {

        printf("Error opening file!\n");

        return;
    }

    fwrite(&coach, sizeof(Coach), 1, fp);

    fclose(fp);

    printf("Coach registered successfully!\n");
}


// ==================== VIEW COACHES ====================

void viewCoaches() {

    FILE *fp;

    Coach coach;

    fp = fopen(COACH_FILE, "rb");

    if(fp == NULL) {

        printf("No coaches found!\n");

        return;
    }

    printf("\n=== COACH LIST ===\n");

    while(fread(&coach, sizeof(Coach), 1, fp)) {

        printf("Nickname: %s\n", coach.nickname);
    }

    fclose(fp);
}
