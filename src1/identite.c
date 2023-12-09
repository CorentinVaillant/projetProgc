#include "identite.h"
#include <stdlib.h>
#include <stdio.h>

#define LONGUEURMOTMAX 80
#define ERREUR "\x1b[31mERREUR\x1b[0m"
#define FINSTR '\0'

//struture

// Identité d'une personne
struct sIdentite{
    int Identifiant ; // Identifiant unique
    char *Nom; // Nom (chaîne de caractères allouée dynamiquement)
    char *Prenom; // Prénom (chaîne de caractères allouée dynamiquement)
    char Sexe; // ’F ’ ou ’M ’
    char DateNaissance [LG_DATE +1]; // Date de naissance sous la forme jj/mm/aaaa
};


//prive
static unsigned int LongueurString(char String[]){
    char cara = String[0];
    int longueur=0;
    while (cara!=FINSTR){
        longueur++;
        cara = String[longueur];
    }
    return longueur;
}
//retire tout les espaces ou retours à la ligne présent après les caractères autres
static void raccourcirStr(char Str[]){
    int i = 0;
    while (Str[i] != ' ' && Str[i] != '\n' && Str[i] != FINSTR)
        i++;
    Str[i] = FINSTR;
    
}


//publique

//fonction de creation d'une variable tIdentite
tIdentite IdentiteCreer(int Id, char *Nom, char *Prenom, char Sexe, char DateNais[]){
    //copie de Nom Prenom :
    char *pNom = malloc(sizeof(char)*(LongueurString(Nom)+1));
    char *pPrenom = malloc(sizeof(char)*(LongueurString(Prenom)+1));
    if(!pNom || !pPrenom){ //vérification de la réussite du malloc
        free(pPrenom);
        free(pNom);
        return NULL;
    }
    //copie caractère par caractère
    for(int i = 0 ; i<LongueurString(Nom)+1 ; i++){
        pNom[i] = Nom[i];
    }
    for(int i = 0 ; i<LongueurString(Prenom)+1 ; i++){
        pPrenom[i] = Prenom[i];
    }

    //création de la variable tIdentite renvoyé:
    tIdentite nouvIdentite = malloc(sizeof(struct sIdentite));

    nouvIdentite->Identifiant=Id;
    nouvIdentite->Nom=pNom;
    nouvIdentite->Prenom=pPrenom;
    if (Sexe != 'M' && Sexe != 'F'){ //vérification du sexe
        IdentiteLiberer(nouvIdentite);
        return NULL;
    }
    nouvIdentite->Sexe=Sexe;
    for(int i=0 ; i<LG_DATE+1 ; i++){
        nouvIdentite->DateNaissance[i]=DateNais[i];
    }
    return nouvIdentite;
}
//renvoie l'identifiant associe a Identite
int IdentiteIdentifiant(tIdentite Identite){
    return Identite->Identifiant;
}

//renvoie le nom associe a Identite sous forme d'un pointeur vers la chaine de caracteres
char *IdentiteNom(tIdentite Identite){
    return Identite->Nom;
}
//renvoie le prenom associe a Identitesous forme d'un pointeur vers la chaine de caracteres
char *IdentitePrenom(tIdentite Identite){
    return Identite->Prenom;
}
//renvoie le sexe associe a Identite
char IdentiteSexe(tIdentite Identite){
    return Identite->Sexe;
}
//renvoie la date de naissance associe a Identite sous forme d'un pointeur vers la chaine de caracteres
char *IdentiteDateNaissance(tIdentite Identite){
    return Identite->DateNaissance;
}
//affichage de l'identité
void IdentiteAfficher(tIdentite Identite){
    printf("[%d] %s %s, %c, %s ",IdentiteIdentifiant(Identite),IdentiteNom(Identite),IdentitePrenom(Identite),IdentiteSexe(Identite),IdentiteDateNaissance(Identite));
    fflush(stdout);
}

//libere l'espace memoire occupe par Identite
void IdentiteLiberer(tIdentite Identite){
    free(Identite->Nom);
    free(Identite->Prenom);
    free(Identite);
}

//lis le contenue d'un fichier pour les associe avec une variable tIdentite
tIdentite IdentiteLiref(FILE *f){
    int Identifiant ; 
    char Nom[LONGUEURMOTMAX +1],Prenom[LONGUEURMOTMAX +1], DateNaissance [LG_DATE +1];
    char Sexe;


    if(!fscanf(f,"%d",&Identifiant))
        return NULL;

    while (fgetc(f) == ' '); //passe tout les espace
    
    if(!fgets(Nom,LONGUEURMOTMAX+1,f)) //Nom<-L2 + vérification de ligne non vide/feof
        return NULL;
    fseek(f,-1,SEEK_CUR);
    if(fgetc(f)!='\n') //vérification du nombre de caractères max par ligne sur L2
        return NULL;
    

    if(!fgets(Prenom,LONGUEURMOTMAX+1,f))//Prenom<-L3 + vérification de ligne non vide/feof
        return NULL;
    fseek(f,-1,SEEK_CUR);

    if(fgetc(f)!='\n'){ //vérification du nombre de caractères max par ligne sur L3
        fprintf(stderr,"%s : format de fichier invalide",ERREUR);
        return NULL;
    }
    
    
    Sexe = (char) fgetc(f);
    
    if(!Sexe){
        fprintf(stderr,"%s : format de fichier invalide",ERREUR);
        return NULL;
    }
    
    
    while (fgetc(f) == (int)' '); //passe tout les espace
    
    if(!fgets(DateNaissance,LG_DATE+1,f)){
        fprintf(stderr,"%s : format de fichier invalide",ERREUR);
        return NULL;
    }

    raccourcirStr(Nom);
    raccourcirStr(Prenom);
    
    
    return IdentiteCreer(Identifiant,Nom,Prenom,Sexe,DateNaissance);
}