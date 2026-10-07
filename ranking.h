#ifndef RANKING_H
#define RANKING_H
/* ========= ranking.h ========= 
Header delle funzioni per gestione ranking.
*/
#include "models.h"

float calculatePlayerPoints(Match match, int playerId);
float calculateTeamPoints(Squadra team);
void updateAllTeamPoints();
void showTournamentRanking();
void showGeneralRanking();
void showTennistiReport();
void menuRanking();

#endif
