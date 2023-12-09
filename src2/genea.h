#ifndef GENEA_H
#define GENEA_H

#include "../src1/identite.h"


// Type permettant de manipuler un arbre généalogique
typedef struct sArbre * tArbre ;
typedef struct sFiche * pFiche;

extern tArbre ArbreCreer(void); //créer un arbre vide
extern void ArbreAfficher(tArbre Arbre); //Affiche l'arbre Arbre
extern void ArbreAjouterPersonne(tArbre Arbre, tIdentite Identite); //Ajoute une personne à partir d'une variable Identite
extern void ArbreLiberer(tArbre Arbre); //Libère la heap de l'arbre
extern tArbre ArbreLirePersonnesFichier(char Fichier[]); //Lis des identitées contenus dans un fichier, et les places dans un arbres
extern void ArbreAjouterLienParente(tArbre Arbre, int IdEnfant, int IdParent, char Parente); //ajoute un lien de parente entre deux personne dans un arbre, grâce aux indice de l'enfant et du parent
extern int ArbreLireLienParentef(FILE *f, int *pIdEnfant, int *pIdParent, char *pParente); //lis un lien de parenté contenus dans un fichier
extern tArbre ArbreLireLienParenteFichier(tArbre Arbre, char Fichier[]); //lis les liens de parentés contenue dans un fichier, et les ajoute un l'arbre Arbre

//niveau 3 :

extern void ArbreEcrireGV(tArbre Arbre, char Fichier[]); //retranscrit l'arbre dans un fichier .dot
extern void ArbreAfficherAscendants(tArbre Arbre, int Identifiant); //affiche tout les ascendants d'une personne présente dans l'arbre Arbre
extern void ArbreEcrireAscendantsGV(tArbre Arbre, int Identifiant, char Fichier[]); //retranscrit un arbre composer uniquement d'une personne et de ses ascendant en .dot

#endif//GENA_H