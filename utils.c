/* ========= utils.c ========= 
File: utils.c
Scopo:
Funzioni di supporto utilizzate nel progetto.
*/

#include <stdio.h>
#include "utils.h"

void clearInputBuffer() {

    int c;

    while ((c = getchar()) != '\n' && c != EOF);

}

void pauseProgram() {

    printf("\nPress Enter to continue...");
    getchar();

}
