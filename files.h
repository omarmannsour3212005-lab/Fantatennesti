#ifndef FILES_H
#define FILES_H

/* ========= files.h ========= 
Header contenente le funzioni per gestione file.
*/
#include "tennista.h"

void salvaTennista(Tennista t);

void leggiTennisti();

int trovaTennistaByID(int id, Tennista *t);

void aggiornaTennista(Tennista t);

void eliminaTennistaFile(int id);

int esisteID(int id);

#endif
