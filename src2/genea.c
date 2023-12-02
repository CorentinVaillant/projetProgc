#include "genea.h"
#include <stdlib.h>

#define ROUGE "\x1b[31m"
#define BLANC   "\x1b[0m"


typedef  struct sFiche * pFiche;



tArbre ArbreCreer(void){
    return malloc(sizeof(struct sArbre));
}

void ArbreAfficher(tArbre Arbre){

    if(!Arbre->pPremiere || !Arbre->pDerniere){
        fprintf(stderr,"%sERREUR%s : Pointeur invalide :\n\tArbre->pPremiere = %p \n\tArbre->pDerniere = %p\n",
        ROUGE,
        BLANC,
        Arbre->pPremiere,
        Arbre->pDerniere);
        return ;
    }
        
    pFiche id = Arbre->pPremiere;
    pFiche dern = Arbre->pDerniere;

    if(!(id->pSuivante))printf("AZAAAAAAAAH§!!!");

    while (id->pSuivante){
        //affichage de l'identité
        if(!id->Identite) printf("inconnu");
        else IdentiteAfficher(id->Identite);
        
        //affichage du père
        printf("\n\tPère : ");
        if(!(id->pPere) || !(id->pPere->Identite)) printf("inconnu");
        else IdentiteAfficher(id->pPere->Identite);

        //affichage de la mère
        printf("\n\tMère : ");
        if(!(id->pMere) || !(id->pMere->Identite)) printf("inconnu");
        else IdentiteAfficher(id->pMere->Identite);
        printf("\n");

        if(id == id->pSuivante){
            fprintf(stderr,"%sERREUR%s : l'identité pointe vers elle même",ROUGE,BLANC);
            return ;}

        id = id->pSuivante;
        fflush(stdout);
    }
}

void ArbreAjouterPersonne(tArbre Arbre, tIdentite Identite){
    
    if(!Identite){
        fprintf(stderr,"%sERREUR%s : Identite = NULL",ROUGE,BLANC);
        return ;
    }
    
    pFiche pId = calloc(1,sizeof(struct sFiche));

    if(!pId){
        fprintf(stderr,"%sERREUR%s : echec dans l'allouage de struct sFiche* pId !",ROUGE,BLANC);
        return ;
    }
    pId->Identite = Identite;

    Arbre->pDerniere->pSuivante = pId;
    Arbre->pDerniere = pId;
};