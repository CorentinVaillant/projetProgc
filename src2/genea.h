#ifndef GENEA_H
#define GENEA_H

#include "../src1/identite.h"

// Arbre généalogique
struct sArbre{
    struct sFiche * pPremiere ; // Adresse de la première fiche
    struct sFiche * pDerniere ; // Adresse de la dernière fiche
};
// Fiche associ é e à chaque individu pr é sent dans l ’ arbre
struct sFiche{
    tIdentite Identite ; // Accès aux informations de l’identité de la personne
    struct sFiche * pPere ; // Adresse de la fiche du père
    struct sFiche * pMere ; // Adresse de la fiche de la mère
    struct sFiche * pSuivante ; // Adresse de la fiche suivante
};

// Type permettant de manipuler un arbre généalogique
typedef struct sArbre * tArbre ;
typedef  struct sFiche * pFiche;

extern tArbre ArbreCreer(void);
extern pFiche CreerFiche(tIdentite Identite, pFiche pPere, pFiche pMere, pFiche pSuivante);
extern void ArbreAfficher(tArbre Arbre);
extern void ArbreAjouterPersonne(tArbre Arbre, tIdentite Identite);
extern void ArbreLiberer(tArbre Arbre);
extern tArbre ArbreLirePersonnesFichier(char Fichier[]);
extern void ArbreAjouterLienParente(tArbre Arbre, int IdEnfant, int IdParent, char Parente);

#endif//GENA_H