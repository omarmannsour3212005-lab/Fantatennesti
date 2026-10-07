/* ========= squadra.c ========= 
File: squadra.c
Scopo:
Creazione e gestione delle squadre dei Fanta-Coach.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "models.h"
#include "config.h"
#include "squadra.h"
#include "coach.h"
#include "files.h"
#include "tornei.h"

/** Controlla se un torneo ha gia match inseriti. */
int tournamentHasMatches(int torneoId) {
    FILE *fp = fopen("matches.dat", "rb");
    Match m;

    if(fp == NULL) return 0;

    while(fread(&m, sizeof(Match), 1, fp)) {
        if(m.torneoId == torneoId) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

/** Controlla se un coach ha gia creato una squadra per quel torneo. */
int teamExistsForTournament(char nickname[], int torneoId) {
    FILE *fp = fopen("squadre.dat", "rb");
    Squadra team;

    if(fp == NULL) return 0;

    while(fread(&team, sizeof(Squadra), 1, fp)) {
        if(strcmp(team.coachNickname, nickname) == 0 && team.torneoId == torneoId) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

/** Crea una squadra di 6 tennisti diversi e disponibili con budget massimo 100. */
void createTeam() {
    FILE *fpPlayers;
    FILE *fpTeams;
    Tennista player;
    Squadra team;
    int selectedId, found, duplicate, i, j;
    float totalCredits = 0;

    printf("\n===== CREATE TEAM =====\n");
    printf("Coach nickname: ");
    scanf("%49s", team.coachNickname);

    if(!coachExists(team.coachNickname)) {
        printf("Coach non registrato. Prima registrati.\n");
        return;
    }

    printf("Tournament ID: ");
    scanf("%d", &team.torneoId);

    if(!torneoExists(team.torneoId)) {
        printf("Torneo non trovato. Prima crea il torneo.\n");
        return;
    }

    if(tournamentHasMatches(team.torneoId)) {
        printf("Non puoi creare la squadra: il torneo ha gia match.\n");
        return;
    }

    if(teamExistsForTournament(team.coachNickname, team.torneoId)) {
        printf("Hai gia una squadra per questo torneo.\n");
        return;
    }

    fpPlayers = fopen("tennisti.dat", "rb");
    if(fpPlayers == NULL) {
        printf("Error opening players file.\n");
        return;
    }

    printf("\n===== AVAILABLE PLAYERS =====\n");
    while(fread(&player, sizeof(Tennista), 1, fpPlayers)) {
        if(player.disponibile == 1) {
            printf("ID: %d | %s %s | Ranking: %d | Credits: %.2f\n",
                   player.id, player.nome, player.cognome, player.ranking, player.crediti);
        }
    }

    for(i = 0; i < MAX_PLAYERS; i++) {
        duplicate = 0;
        found = 0;

        printf("\nSelect player %d ID: ", i + 1);
        scanf("%d", &selectedId);

        for(j = 0; j < i; j++) {
            if(team.playerIds[j] == selectedId) duplicate = 1;
        }

        if(duplicate) {
            printf("Player already selected.\n");
            i--;
            continue;
        }

        rewind(fpPlayers);
        while(fread(&player, sizeof(Tennista), 1, fpPlayers)) {
            if(player.id == selectedId && player.disponibile == 1) {
                found = 1;
                if(totalCredits + player.crediti > MAX_BUDGET) {
                    printf("Budget exceeded.\n");
                    found = 0;
                    break;
                }
                team.playerIds[i] = player.id;
                totalCredits += player.crediti;
                printf("Added: %s %s\n", player.nome, player.cognome);
                printf("Current credits: %.2f/%d\n", totalCredits, MAX_BUDGET);
                break;
            }
        }

        if(!found) {
            printf("Invalid player.\n");
            i--;
        }
    }

    fclose(fpPlayers);
    team.totalCredits = totalCredits;
    team.totalPoints = 0;

    fpTeams = fopen("squadre.dat", "ab");
    if(fpTeams == NULL) {
        printf("Error opening squadre file.\n");
        return;
    }

    fwrite(&team, sizeof(Squadra), 1, fpTeams);
    fclose(fpTeams);
    printf("\nTeam created successfully!\n");
}

/** Visualizza tutte le squadre. */
void viewTeams() {
    FILE *fp;
    Squadra team;
    int i;

    fp = fopen("squadre.dat", "rb");
    if(fp == NULL) {
        printf("No teams found.\n");
        return;
    }

    printf("\n===== TEAMS =====\n");
    while(fread(&team, sizeof(Squadra), 1, fp)) {
        printf("\nCoach: %s\n", team.coachNickname);
        printf("Tournament ID: %d\n", team.torneoId);
        printf("Players IDs: ");
        for(i = 0; i < MAX_PLAYERS; i++) printf("%d ", team.playerIds[i]);
        printf("\nTotal Credits: %.2f\n", team.totalCredits);
        printf("Total Points: %.2f\n", team.totalPoints);
    }

    fclose(fp);
}

void displayTeam() {
    viewTeams();
}

/** Modifica una squadra solo se il torneo non ha match inseriti. */
void modifyTeam() {
    FILE *fp = fopen("squadre.dat", "rb");
    FILE *temp = fopen("temp_squadre.dat", "wb");
    FILE *fpPlayers;
    Squadra team;
    Tennista player;
    char nickname[50];
    int torneoId, foundTeam = 0, i, j, selectedId, found, duplicate;
    float totalCredits;

    if(fp == NULL || temp == NULL) {
        printf("No teams found.\n");
        return;
    }

    printf("Coach nickname: ");
    scanf("%49s", nickname);
    printf("Tournament ID: ");
    scanf("%d", &torneoId);

    if(!torneoExists(torneoId)) {
        printf("Torneo non trovato.\n");
        fclose(fp);
        fclose(temp);
        remove("temp_squadre.dat");
        return;
    }

    if(tournamentHasMatches(torneoId)) {
        printf("Non puoi modificare la squadra: il torneo ha gia match.\n");
        fclose(fp);
        fclose(temp);
        remove("temp_squadre.dat");
        return;
    }

    while(fread(&team, sizeof(Squadra), 1, fp)) {
        if(strcmp(team.coachNickname, nickname) == 0 && team.torneoId == torneoId) {
            foundTeam = 1;
            fpPlayers = fopen("tennisti.dat", "rb");
            if(fpPlayers == NULL) {
                printf("Tennisti file non trovato.\n");
                fwrite(&team, sizeof(Squadra), 1, temp);
                continue;
            }

            totalCredits = 0;
            for(i = 0; i < MAX_PLAYERS; i++) {
                duplicate = 0;
                found = 0;
                printf("New player %d ID: ", i + 1);
                scanf("%d", &selectedId);

                for(j = 0; j < i; j++) if(team.playerIds[j] == selectedId) duplicate = 1;
                if(duplicate) {
                    printf("Player already selected.\n");
                    i--;
                    continue;
                }

                rewind(fpPlayers);
                while(fread(&player, sizeof(Tennista), 1, fpPlayers)) {
                    if(player.id == selectedId && player.disponibile == 1) {
                        found = 1;
                        if(totalCredits + player.crediti > MAX_BUDGET) {
                            printf("Budget exceeded.\n");
                            found = 0;
                            break;
                        }
                        team.playerIds[i] = selectedId;
                        totalCredits += player.crediti;
                        break;
                    }
                }

                if(!found) {
                    printf("Invalid player.\n");
                    i--;
                }
            }
            fclose(fpPlayers);
            team.totalCredits = totalCredits;
            team.totalPoints = 0;
        }
        fwrite(&team, sizeof(Squadra), 1, temp);
    }

    fclose(fp);
    fclose(temp);
    remove("squadre.dat");
    rename("temp_squadre.dat", "squadre.dat");

    if(foundTeam) printf("Team modified successfully.\n");
    else printf("Team not found.\n");
}
