#include <stdio.h>
# define LG_DATE 10 // Nombre de caractères d’une date sous la forme jj/mm/aaaa

// Identité d'une personne
struct sIdentite{
    int Identifiant ; // Identifiant unique
    char *Nom; // Nom (chaîne de caractères allouée dynamiquement)
    char *Prenom; // Prénom (chaîne de caractères allouée dynamiquement)
    char Sexe; // ’F ’ ou ’M ’
    char DateNaissance [LG_DATE +1]; // Date de naissance sous la forme jj/mm/aaaa
};

// Type permettant de manipuler l’identité d’une personne
typedef struct sIdentite * tIdentite;

//fonctions
extern tIdentite IdentiteCreer(int Id, char *Nom, char *Prenom, char Sexe, char DateNais[]); //créer un structure sIdentite et renvoie sont pointeur
extern int IdentiteIdentifiant(tIdentite Identite); //retourne identifiant d'une struct sIdentite grâce à sont pointeur Identite
extern char *IdentiteNom(tIdentite Identite); //retourne le nom d'une struct sIdentite grâce à sont pointeur Identite
extern char *IdentitePrenom(tIdentite Identite); //retourne le prénom d'une struct sIdentite grâce à sont pointeur Identite
extern char IdentiteSexe(tIdentite Identite); //retourne le sexe d'une struct sIdentite grâce à sont pointeur Identite
extern char *IdentiteDateNaissance(tIdentite Identite); //retourne la date de naissance d'une struct sIdentite grâce à sont pointeur Identite
extern void IdentiteAfficher(tIdentite Identite); // affiche une structure tIdentite grâce à son pointeur, de la manière suivante : [identifiant] nom prenom sexe date de naissance
extern void IdentiteLiberer(tIdentite Identite); //libère les varible Nom est prénom qui sont stocké dans la heap
extern tIdentite IdentiteLiref(FILE *f); //lis une identité dans un fichiers 