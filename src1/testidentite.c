#include <stdio.h>
#include <stdlib.h>
#include "identite.h"

void testIdfile(char Str[]){
    tIdentite IdTest;
    FILE *f = fopen(Str,"rt");
    if(!f) perror("could not open file !");
    IdTest = IdentiteLiref(f);
    if(!IdTest){
        printf("\x1b[31m IdTest NULL !\n\x1b[0m");
        return ;
    }
    IdentiteAfficher(IdTest);
    printf("\n");
    IdentiteLiberer(IdTest);
    free(IdTest);
}

int main(void){
    tIdentite IdTest = IdentiteCreer(1,"Anger","Eiden",'M',"29/07/2005");
    IdentiteAfficher(IdTest);
    printf("\n");

    //test de libération de la variable
    IdentiteLiberer(IdTest);
    free(IdTest);

    //test de lecture de fichier
    printf("test personne normal :\n");
    testIdfile("./IDs/coolPersonne.ind");
    printf("\tOK!\n");

    printf("test personne avec espaces :\n");
    testIdfile("./IDs/spacePersonne.ind");
    printf("\tOK!\n");

    printf("test personne \x1b[31m NULL \x1b[0m (lignes manquantes):\n");
    testIdfile("./IDs/missingNULLPersonne.ind");
    printf("\tOK!\n");

    printf("test personne \x1b[31m NULL \x1b[0m (trop de caractères):\n");
    testIdfile("./IDs/tooMuchNULLPersonne.ind");
    printf("\tOK!\n");
    return 0;
}