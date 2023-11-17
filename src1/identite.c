#include "identite.h"
#include <stdlib.h>

//prive
unsigned int LongueurString(char String[]){
    char cara = String[0];
    int longueur=0;
    while (cara!='\0'){
        longueur++;
        cara = String[longueur];
    }
    return longueur;
}


//publique
tIdentite IdentiteCreer(int Id, char *Nom, char *Prenom, char Sexe, char DateNais[]){
    //copie de Nom Prenom :
    char *pNom = malloc(sizeof(char)*(LongueurString(Nom)+1));
    char *pPrenom = malloc(sizeof(char)*(LongueurString(Prenom)+1));
    if(!pNom || !pPrenom){ //vérification de la réussite du malloc
        return NULL;
    }
    *pNom = *Nom;
    *pPrenom = *Prenom;

    //création de tIdentite renvoyé:
    tIdentite nouvIdentite = malloc(sizeof(struct sIdentite));

    nouvIdentite->Identifiant=Id;
    nouvIdentite->Nom=pNom;
    nouvIdentite->Prenom=pPrenom;
    if (Sexe != 'M' && Sexe != 'F'){ //vérification du sexe
        return NULL;
    }
    nouvIdentite->Sexe=Sexe;
    for(int i=0 ; i<LG_DATE+1 ; i++){
        nouvIdentite->DateNaissance[i]=DateNais[i];
    }
    return nouvIdentite;
}

char *IdentiteNom(tIdentite Identite){
    return Identite->Nom;
}
char *IdentitePrenom(tIdentite Identite){
    return Identite->Prenom;
}
char IdentiteSexe(tIdentite Identite){
    return Identite->Sexe;
}
char *IdentiteDateNaissance(tIdentite Identite){
    return Identite->DateNaissance;
}

