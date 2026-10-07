#ifndef TENNISTA_H
#define TENNISTA_H
/* ========= tennista.h ========= 
Header contenente le funzioni del modulo tennista.
*/
#define FILE_PLAYERS "tennisti.dat"

typedef struct {
    int id;
    char nome[50];
    char cognome[50];
    int ranking;
    float crediti;
    int disponibile;
} Tennista;

float calcolaCrediti(int ranking);
void aggiungiTennista();
void visualizzaTennisti();
void cercaTennista();
void modificaTennista();
void eliminaTennista();
void menuTennista();
void menuPlayers();

#endif
