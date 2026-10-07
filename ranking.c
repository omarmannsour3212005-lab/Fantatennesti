/* ========= ranking.c ========= 
File: ranking.c
Scopo:
Calcolo punteggi e classifiche del Fantatennis.
*/

#include <stdio.h>
#include <string.h>

#include "models.h"
#include "ranking.h"
#include "files.h"

/** Cerca il vincitore di un torneo nel file tornei.dat. */
int getTournamentWinner(int torneoId) {
    FILE *fp = fopen("tornei.dat", "rb");
    Torneo t;

    if(fp == NULL) return -1;

    while(fread(&t, sizeof(Torneo), 1, fp)) {
        if(t.id == torneoId) {
            fclose(fp);
            return t.winnerId;
        }
    }

    fclose(fp);
    return -1;
}

/** Calcola i punti di un tennista in un match. */
float calculatePlayerPoints(Match match, int playerId) {
    float points = 0;

    if(playerId != match.player1Id && playerId != match.player2Id) return 0;

    if(match.winnerId == playerId) {
        points += 10;

        if(playerId == match.player1Id && match.setPlayer1 == 2 && match.setPlayer2 == 0) points += 3;
        if(playerId == match.player2Id && match.setPlayer2 == 2 && match.setPlayer1 == 0) points += 3;
    } else {
        points -= 3;
    }

    if(match.player1Id == playerId) {
        if(match.ace1 >= 5) points += 2;
        points += match.break1 * 0.5;
        points -= match.doppiFalli1;
    } else {
        if(match.ace2 >= 5) points += 2;
        points += match.break2 * 0.5;
        points -= match.doppiFalli2;
    }

    return points;
}

/** Calcola tutti i punti di un giocatore in un torneo. */
float calculatePlayerPointsInTournament(int playerId, int torneoId) {
    FILE *fp = fopen("matches.dat", "rb");
    Match match;
    float total = 0;

    if(fp == NULL) return 0;

    while(fread(&match, sizeof(Match), 1, fp)) {
        if(match.torneoId == torneoId) {
            total += calculatePlayerPoints(match, playerId);
        }
    }

    fclose(fp);
    return total;
}

/** Calcola il punteggio totale di una squadra. */
float calculateTeamPoints(Squadra team) {
    int i;
    int winnerId = getTournamentWinner(team.torneoId);
    float total = 0;

    for(i = 0; i < MAX_PLAYERS; i++) {
        total += calculatePlayerPointsInTournament(team.playerIds[i], team.torneoId);
        if(team.playerIds[i] == winnerId) total += 10;
    }

    return total;
}

/** Aggiorna i punti delle squadre nel file. */
void updateAllTeamPoints() {
    FILE *fp = fopen("squadre.dat", "rb");
    FILE *temp = fopen("temp_squadre.dat", "wb");
    Squadra team;

    if(fp == NULL || temp == NULL) return;

    while(fread(&team, sizeof(Squadra), 1, fp)) {
        team.totalPoints = calculateTeamPoints(team);
        fwrite(&team, sizeof(Squadra), 1, temp);
    }

    fclose(fp);
    fclose(temp);
    remove("squadre.dat");
    rename("temp_squadre.dat", "squadre.dat");
}

/** Mostra la classifica di un torneo basata sui punti delle squadre. */
void showTournamentRanking() {
    FILE *fp;
    Squadra teams[100];
    Squadra temp;
    int torneoId, count = 0, i, j;

    updateAllTeamPoints();

    printf("Inserisci ID torneo: ");
    scanf("%d", &torneoId);

    fp = fopen("squadre.dat", "rb");
    if(fp == NULL) {
        printf("Nessuna squadra trovata.\n");
        return;
    }

    while(fread(&teams[count], sizeof(Squadra), 1, fp)) {
        if(teams[count].torneoId == torneoId) count++;
    }
    fclose(fp);

    for(i = 0; i < count - 1; i++) {
        for(j = i + 1; j < count; j++) {
            if(teams[j].totalPoints > teams[i].totalPoints) {
                temp = teams[i];
                teams[i] = teams[j];
                teams[j] = temp;
            }
        }
    }

    printf("\n===== CLASSIFICA TORNEO =====\n");
    for(i = 0; i < count; i++) {
        printf("%d) %s - %.2f punti\n", i + 1, teams[i].coachNickname, teams[i].totalPoints);
    }

    if(count == 0) printf("Nessuna squadra per questo torneo.\n");
}

/** Mostra la classifica generale sommando i punti dei coach nei tornei. */
void showGeneralRanking() {
    FILE *fp;
    Squadra team;
    char nicknames[100][50];
    float points[100];
    int count = 0, found, i, j;
    char tempName[50];
    float tempPoints;

    updateAllTeamPoints();

    fp = fopen("squadre.dat", "rb");
    if(fp == NULL) {
        printf("Nessuna squadra trovata.\n");
        return;
    }

    while(fread(&team, sizeof(Squadra), 1, fp)) {
        found = 0;
        for(i = 0; i < count; i++) {
            if(strcmp(nicknames[i], team.coachNickname) == 0) {
                points[i] += team.totalPoints;
                found = 1;
            }
        }
        if(!found) {
            strcpy(nicknames[count], team.coachNickname);
            points[count] = team.totalPoints;
            count++;
        }
    }
    fclose(fp);

    for(i = 0; i < count - 1; i++) {
        for(j = i + 1; j < count; j++) {
            if(points[j] > points[i]) {
                tempPoints = points[i]; points[i] = points[j]; points[j] = tempPoints;
                strcpy(tempName, nicknames[i]); strcpy(nicknames[i], nicknames[j]); strcpy(nicknames[j], tempName);
            }
        }
    }

    printf("\n===== CLASSIFICA GENERALE =====\n");
    for(i = 0; i < count; i++) {
        printf("%d) %s - %.2f punti\n", i + 1, nicknames[i], points[i]);
    }
}

/** Report dei tennisti ordinati per punteggio totale. */
void showTennistiReport() {
    FILE *fp = fopen("tennisti.dat", "rb");
    Tennista players[100], tempPlayer;
    float points[100], tempPoints;
    int count = 0, i, j;

    if(fp == NULL) {
        printf("Nessun tennista trovato.\n");
        return;
    }

    while(fread(&players[count], sizeof(Tennista), 1, fp)) {
        points[count] = 0;
        count++;
    }
    fclose(fp);

    for(i = 0; i < count; i++) {
        FILE *fm = fopen("matches.dat", "rb");
        Match match;
        if(fm != NULL) {
            while(fread(&match, sizeof(Match), 1, fm)) {
                points[i] += calculatePlayerPoints(match, players[i].id);
            }
            fclose(fm);
        }
    }

    for(i = 0; i < count - 1; i++) {
        for(j = i + 1; j < count; j++) {
            if(points[j] > points[i]) {
                tempPlayer = players[i]; players[i] = players[j]; players[j] = tempPlayer;
                tempPoints = points[i]; points[i] = points[j]; points[j] = tempPoints;
            }
        }
    }

    printf("\n===== REPORT TENNISTI =====\n");
    for(i = 0; i < count; i++) {
        printf("%d) %s %s - %.2f punti\n", i + 1, players[i].nome, players[i].cognome, points[i]);
    }
}

/** Menu ranking e statistiche. */
void menuRanking() {
    int scelta;

    do {
        printf("\n========== RANKING MENU ==========\n");
        printf("1. Classifica torneo\n");
        printf("2. Classifica generale\n");
        printf("3. Report tennisti\n");
        printf("0. Torna indietro\n");
        printf("Scelta: ");
        scanf("%d", &scelta);

        switch(scelta) {
            case 1: showTournamentRanking(); break;
            case 2: showGeneralRanking(); break;
            case 3: showTennistiReport(); break;
            case 0: break;
            default: printf("Scelta non valida!\n");
        }
    } while(scelta != 0);
}
