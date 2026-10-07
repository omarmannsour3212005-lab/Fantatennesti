#ifndef SQUADRA_H
#define SQUADRA_H

/* ========= squadra.h ========= 
Header contenente le funzioni del modulo squadra.
*/
void createTeam();
void modifyTeam();
void displayTeam();
void viewTeams();
int teamExistsForTournament(char nickname[], int torneoId);
int tournamentHasMatches(int torneoId);

#endif
