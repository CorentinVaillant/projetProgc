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
extern tIdentite IdentiteCreer(int Id, char *Nom, char *Prenom, char Sexe, char DateNais[]);
extern int IdentiteIdentifiant(tIdentite Identite);
extern char *IdentiteNom(tIdentite Identite);
extern char *IdentitePrenom(tIdentite Identite);
extern char IdentiteSexe(tIdentite Identite);
extern char *IdentiteDateNaissance(tIdentite Identite);
extern void IdentiteAfficher(tIdentite Identite);
extern void IdentiteLiberer(tIdentite Identite);
extern tIdentite IdentiteLiref(FILE *f);