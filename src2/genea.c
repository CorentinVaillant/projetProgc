#include "genea.h"
#include <stdlib.h>

typedef  struct sFiche * pFiche;

tArbre ArbreCreer(void){
    return malloc(sizeof(struct sArbre));
}

void ArbreAfficher(tArbre Arbre){

    if(!Arbre->pPremiere || !Arbre->pDerniere){
        printf("!%d\n", (int) Arbre->pPremiere);
        printf("!%d\n", (int) Arbre->pDerniere);
        perror("aaaah");
        return ;
    }
        
    
    pFiche id = Arbre->pPremiere;
    pFiche dern = Arbre->pDerniere;

    while (id!=dern){
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

        if(id == id->pSuivante)
            return ;

        id = id->pSuivante;
    }

        //affichage de l'identité
        if(!dern->Identite) printf("inconnu");
        else IdentiteAfficher(dern->Identite);
        
        //affichage du père
        printf("\n\tPère : ");
        if(!(dern->pPere) || !(dern->pPere->Identite)) printf("inconnu");
        else IdentiteAfficher(dern->pPere->Identite);

        //affichage de la mère
        printf("\n\tMère : ");
        if(!(dern->pMere) || !(dern->pMere->Identite)) printf("inconnu");
        else IdentiteAfficher(dern->pMere->Identite);
        printf("\n");
    
    
}