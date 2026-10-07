#ifndef MODELS_H
#define MODELS_H
/* ========= models.h ========= 
File: models.h
Scopo:
Definizione delle strutture dati del progetto.
*/

#include "config.h"
#include "tennista.h"

/** Struttura che rappresenta un match del torneo. */
typedef struct {
    int id;
    int torneoId;
    int player1Id;
    int player2Id;
    int setPlayer1;
    int setPlayer2;
    int ace1;
    int ace2;
    int doppiFalli1;
    int doppiFalli2;
    int break1;
    int break2;
    int winnerId;
} Match;

/** Struttura che rappresenta un torneo. */
typedef struct {
    int id;
    char nome[MAX_NAME];
    int winnerId;
} Torneo;

/** Struttura che rappresenta un Fanta-Coach. */
typedef struct {
    char nickname[MAX_NAME];
} Coach;

/** Struttura che rappresenta una squadra. */
typedef struct {
    char coachNickname[MAX_NAME];
    int torneoId;
    int playerIds[MAX_PLAYERS];
    float totalCredits;
    float totalPoints;
} Squadra;

#endif
